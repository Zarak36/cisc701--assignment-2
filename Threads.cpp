#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

// Structure to hold data for each thread
struct ThreadData {
  int start_index;
  int end_index;
  vector<vector<int>>& arrA;
  vector<vector<int>>& arrB;
  vector<vector<int>>& arrC;
  mutex& mtx;

  // Constructor for ThreadData
  ThreadData(int start, int end, vector<vector<int>>& a,
vector<vector<int>>& b, vector<vector<int>>& c, mutex& m)
    : start_index(start), end_index(end), arrA(a), arrB(b), arrC(c), mtx(m) {}
};

// Function for each thread to perform summation of a portion of the arrays
void sumArrays(ThreadData* data) {
  for (int i = data->start_index; i < data->end_index; ++i) {
    for (int j = 0; j < data->arrA[i].size(); ++j) {
      // Use mutex to ensure thread-safe access to result array
      lock_guard<mutex> lock(data->mtx);
      data->arrC[i][j] = data->arrA[i][j] + data->arrB[i][j];
    }
  }
}

int main(int argc, char* argv[]) {
  // Check for correct arguments
  if (argc != 9) {
    cerr << "Usage: " << argv[0] << " -a fileA -b fileB -c fileC -n num_threads" << endl;
    return 1;
  }

  // Get filenames and number of threads from arguments
  string filenameA, filenameB, filenameC;
  int num_threads;
  for (int i = 1; i < argc; i++) {
    if (string(argv[i]) == "-a") {
      filenameA = argv[++i];
    } else if (string(argv[i]) == "-b") {
      filenameB = argv[++i];
    } else if (string(argv[i]) == "-c") {
      filenameC = argv[++i];
    } else if (string(argv[i]) == "-n") {
      num_threads = stoi(argv[++i]);
    }
  }

  // Open input files
  ifstream infileA(filenameA);
  ifstream infileB(filenameB);
  if (!infileA.is_open() || !infileB.is_open()) {
    cerr << "Error opening input files." << endl;
    return 1;
  }

  // Read array dimensions
  int n, m;
  infileA >> n >> m;
  infileB >> n >> m;

  // Create arrays
  vector<vector<int>> arrA(n, vector<int>(m));
  vector<vector<int>> arrB(n, vector<int>(m));
  vector<vector<int>> arrC(n, vector<int>(m));

  // Read array elements from files
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      infileA >> arrA[i][j];
      infileB >> arrB[i][j];
    }
  }

  // Close input files
  infileA.close();
  infileB.close();

  // Create threads and thread data
  vector<thread> threads;
  vector<ThreadData> threadData;
  mutex mtx; // Mutex for thread-safe access to result array

  // Calculate work distribution for each thread
  int chunk_size = n / num_threads;
  int remaining = n % num_threads;

  for (int i = 0; i < num_threads; ++i) {
    int start_index = i * chunk_size;
    int end_index = start_index + chunk_size;
    if (remaining > 0) {
      end_index++;
      remaining--;
    }
    threadData.emplace_back(start_index, end_index, arrA, arrB, arrC, mtx);
    threads.emplace_back(sumArrays, &threadData.back());
  }

  // Join threads
  for (auto& t : threads) {
    t.join();
  }

  // Write result to output file
  ofstream outfileC(filenameC);
  if (!outfileC.is_open()) {
    cerr << "Error opening output file." << endl;
    return 1;
  }

  for (const auto& row : arrC) {
    for (const auto& elem : row) {
      outfileC << elem << " ";
    }
    outfileC << endl;
  }

  outfileC.close();

  return 0;
}

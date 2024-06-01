#include <iostream>
#include <fstream>
#include <random>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
  // Check for correct arguments
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " filename" << endl;
    return 1;
  }

  // Get filename from argument
  string filename = argv[1];

  // Seed random number generator
  random_device rd;
  mt19937 gen(rd());

  // Get array dimensions from user
  int n, m;
  cout << "Enter number of rows (n): ";
  cin >> n;
  cout << "Enter number of columns (m): ";
  cin >> m;

  // Create arrays
  vector<vector<int>> arr1(n, vector<int>(m));
  vector<vector<int>> arr2(n, vector<int>(m));

  // Generate random numbers for arrays
  uniform_int_distribution<int> dis(1, 100); // Modify range as needed
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      arr1[i][j] = dis(gen);
      arr2[i][j] = dis(gen);
    }
  }

  // Open output file
  ofstream outfile(filename);
  if (!outfile.is_open()) {
    cerr << "Error opening file: " << filename << endl;
    return 1;
  }

  // Write arrays to file
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      outfile << arr1[i][j] << " ";
    }
    outfile << endl;
  }
  outfile << endl;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      outfile << arr2[i][j] << " ";
    }
    outfile << endl;
  }

  outfile.close();

  // Success message
  cout << "SUCCESS" << endl;

  return 0;
}
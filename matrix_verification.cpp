#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

const int MAX_ROWS = 1000;
const int MAX_COLS = 1000;

bool verifyMatrixAddition(const vector<vector<int>>& A, const
vector<vector<int>>& B, const vector<vector<int>>& C) {
    for (int i = 0; i < A.size(); ++i) {
        for (int j = 0; j < A[i].size(); ++j) {
            if (A[i][j] + B[i][j] != C[i][j]) {
                return false;
            }
        }
    }
    return true;
}

int main(int argc, char *argv[]) {
    const char* fileA = "arrayA.txt";
    const char* fileB = "arrayB.txt";
    const char* fileC = "arrayC.txt";

    ifstream inputA(fileA), inputB(fileB), inputC(fileC);

    int n, m;
    inputA >> n >> m; // Assuming the dimensions are the same for all matrices
    vector<vector<int>> A(n, vector<int>(m));
    vector<vector<int>> B(n, vector<int>(m));
    vector<vector<int>> C(n, vector<int>(m));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            inputA >> A[i][j];
            inputB >> B[i][j];
            inputC >> C[i][j];
        }
    }

    if (verifyMatrixAddition(A, B, C)) {
        cout << "CORRECT" << endl;
    } else {
        cout << "INCORRECT" << endl;
    }

    return EXIT_SUCCESS;
}
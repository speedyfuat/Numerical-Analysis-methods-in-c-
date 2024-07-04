#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>

using namespace std;

// Function to multiply a matrix by a vector
vector<double> matrixVectorMultiply(const vector<vector<double>> &matrix, const vector<double> &vec) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    vector<double> result(rows, 0.0);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result[i] += matrix[i][j] * vec[j];
        }
    }

    return result;
}

// Function to normalize a vector
void normalize(vector<double> &vec) {
    double norm = 0.0;
    for (double val : vec) {
        norm += val * val;
    }
    norm = sqrt(norm);
    for (double &val : vec) {
        val /= norm;
    }
}

// Function to calculate the determinant of a matrix (assumes square matrix)
double determinant(const vector<vector<double>> &matrix) {
    if (matrix.size() != matrix[0].size()) {
        throw runtime_error("Matrix must be square.");
    }

    int n = matrix.size();
    vector<vector<double>> temp(matrix);
    double det = 1.0;

    for (int i = 0; i < n; ++i) {
        int pivot = i;
        for (int j = i + 1; j < n; ++j) {
            if (fabs(temp[j][i]) > fabs(temp[pivot][i])) {
                pivot = j;
            }
        }

        if (fabs(temp[pivot][i]) < 1e-10) {
            return 0.0;
        }

        if (i != pivot) {
            swap(temp[i], temp[pivot]);
            det = -det;
        }

        det *= temp[i][i];

        for (int j = i + 1; j < n; ++j) {
            temp[j][i] /= temp[i][i];
            for (int k = i + 1; k < n; ++k) {
                temp[j][k] -= temp[j][i] * temp[i][k];
            }
        }
    }

    return det;
}

// Function to calculate the inverse of a matrix (assumes square matrix)
vector<vector<double>> inverse(const vector<vector<double>> &matrix) {
    if (matrix.size() != matrix[0].size()) {
        throw runtime_error("Matrix must be square.");
    }

    int n = matrix.size();
    vector<vector<double>> result(n, vector<double>(n, 0.0));
    vector<vector<double>> temp(matrix);

    for (int i = 0; i < n; ++i) {
        result[i][i] = 1.0;
    }

    for (int i = 0; i < n; ++i) {
        int pivot = i;
        for (int j = i + 1; j < n; ++j) {
            if (fabs(temp[j][i]) > fabs(temp[pivot][i])) {
                pivot = j;
            }
        }

        if (fabs(temp[pivot][i]) < 1e-10) {
            throw runtime_error("Matrix is singular and cannot be inverted.");
        }

        swap(temp[i], temp[pivot]);
        swap(result[i], result[pivot]);

        double div = temp[i][i];
        for (int j = 0; j < n; ++j) {
            temp[i][j] /= div;
            result[i][j] /= div;
        }

        for (int j = 0; j < n; ++j) {
            if (j != i) {
                double factor = temp[j][i];
                for (int k = 0; k < n; ++k) {
                    temp[j][k] -= factor * temp[i][k];
                    result[j][k] -= factor * result[i][k];
                }
            }
        }
    }

    return result;
}

// Function to calculate the largest eigenvalue using power iteration
double powerIteration(const vector<vector<double>> &matrix, vector<double> &vec, double epsilon) {
    vector<double> oldVec = vec;
    vector<double> newVec = matrixVectorMultiply(matrix, vec);
    normalize(newVec);

    int iterations = 0;
    while (true) {
        vec = newVec;
        newVec = matrixVectorMultiply(matrix, vec);
        normalize(newVec);

        double diff = 0.0;
        for (int i = 0; i < vec.size(); ++i) {
            diff += fabs(newVec[i] - vec[i]);
        }

        iterations++;
        if (diff < epsilon || iterations >= 1000) { // safety cap on iterations
            break;
        }
    }

    vector<double> result = matrixVectorMultiply(matrix, vec);
    double eigenvalue = 0.0;
    for (int i = 0; i < vec.size(); ++i) {
        eigenvalue += vec[i] * result[i];
    }
    return eigenvalue;
}

int main() {
    int n;
    cout << "Enter the size of the matrix: ";
    cin >> n;

    vector<vector<double>> A(n, vector<double>(n));
    vector<double> v1(n);
    double epsilon;

    cout << "Enter the elements of the matrix (row-wise):\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> A[i][j];
        }
    }

    cout << "Enter the initial vector:\n";
    for (int i = 0; i < n; ++i) {
        cin >> v1[i];
    }

    cout << "Enter the epsilon value: ";
    cin >> epsilon;

    double largestEigenvalue = powerIteration(A, v1, epsilon);

    // Reset the vector v1 for smallest eigenvalue calculation
    cout << "Enter the initial vector again for smallest eigenvalue calculation:\n";
    for (int i = 0; i < n; ++i) {
        cin >> v1[i];
    }

    // Inverse of matrix A
    vector<vector<double>> A_inv;
    try {
        A_inv = inverse(A);
    } catch (runtime_error &e) {
        cerr << e.what() << endl;
        return -1;
    }

    double smallestEigenvalue = 1.0 / powerIteration(A_inv, v1, epsilon);

    cout << "Approximate largest eigenvalue: " << largestEigenvalue << endl;
    cout << "Approximate smallest eigenvalue: " << smallestEigenvalue << endl;

    return 0;
}

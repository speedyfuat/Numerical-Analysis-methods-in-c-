#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

// Function to perform the Jacobi method
void jacobi(vector<vector<double>>& A, vector<double>& b, vector<double>& x, int maxIterations, double tolerance) {
    int n = A.size();
    vector<double> x_old(n);
    for (int iteration = 0; iteration <= maxIterations; iteration++) {
        x_old = x;  // Use the previous iteration values for all updates

        for (int i = 0; i < n; i++) {
            double sum = b[i];
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    sum -= A[i][j] * x_old[j];
                }
            }
            x[i] = sum / A[i][i];
        }

        // Print the current state of x (including the 0th iteration)
        cout << "Iteration " << iteration << ": ";
        for (int i = 0; i < n; i++) {
            cout << setprecision(6) << fixed << x[i] << " ";
        }
        cout << endl;

        // Check for convergence if past the 0th iteration
        if (iteration > 0) {
            double norm = 0.0;
            for (int i = 0; i < n; i++) {
                norm += pow(x[i] - x_old[i], 2);
            }
            norm = sqrt(norm);

            if (norm < tolerance) {
                cout << "Converged after " << iteration << " iterations." << endl;
                return;
            }
        }
    }
    cout << "Reached maximum iterations without converging." << endl;
}

int main() {
    int n;  // Size of the matrix
    cout << "Enter the size of the matrix (n): ";
    cin >> n;

    vector<vector<double>> A(n, vector<double>(n));
    vector<double> b(n);
    vector<double> x(n, 0.0);  // Initial guess (can be zeros)

    cout << "Enter the elements of the matrix A row-wise:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> A[i][j];
        }
    }

    cout << "Enter the elements of the vector b:" << endl;
    for (int i = 0; i < n; i++) {
        cin >> b[i];
    }

    double tolerance;
    cout << "Enter the tolerance value: ";
    cin >> tolerance;

    int maxIterations = 100;

    jacobi(A, b, x, maxIterations, tolerance);

    cout << "Solution: " << endl;
    for (int i = 0; i < n; i++) {
        cout << "x[" << i << "] = " << setprecision(6) << fixed << x[i] << endl;
    }

    return 0;
}

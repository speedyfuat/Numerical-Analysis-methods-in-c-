#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

// Matris ve vektörler için typedef
typedef std::vector<double> Vector;
typedef std::vector<Vector> Matrix;

// Gauss eliminasyonu ile üçgenleştirme ve geriye yerine koyma
Vector gaussElimination(Matrix A, Vector b) {
    int n = A.size();

    // Üçgenleştirme
    for (int i = 0; i < n; i++) {
        // Maksimum elemanı bulma
        int maxRow = i;
        for (int k = i + 1; k < n; k++) {
            if (fabs(A[k][i]) > fabs(A[maxRow][i])) {
                maxRow = k;
            }
        }

        // Satır değişimi
        std::swap(A[maxRow], A[i]);
        std::swap(b[maxRow], b[i]);

        // Satırları eleminasyon yapma
        for (int k = i + 1; k < n; k++) {
            double c = -A[k][i] / A[i][i];
            for (int j = i; j < n; j++) {
                if (i == j) {
                    A[k][j] = 0;
                } else {
                    A[k][j] += c * A[i][j];
                }
            }
            b[k] += c * b[i];
        }
    }

    // Geriye yerine koyma
    Vector x(n);
    for (int i = n - 1; i >= 0; i--) {
        x[i] = b[i] / A[i][i];
        for (int k = i - 1; k >= 0; k--) {
            b[k] -= A[k][i] * x[i];
        }
    }
    return x;
}

int main() {
    int n;
    std::cout << "Enter the number of variables: ";
    std::cin >> n;

    Matrix A(n, Vector(n));
    Vector b(n);

    std::cout << "Enter the coefficients of the equations:\n";
    for (int i = 0; i < n; i++) {
        std::cout << "Equation " << i + 1 << ":\n";
        for (int j = 0; j < n; j++) {
            std::cout << "Coefficient of x" << j + 1 << ": ";
            std::cin >> A[i][j];
        }
        std::cout << "Enter the constant term: ";
        std::cin >> b[i];
    }

    Vector result = gaussElimination(A, b);

    std::cout << "Solution:\n";
    for (int i = 0; i < result.size(); i++) {
        std::cout << "x" << i + 1 << " = " << result[i] << std::endl;
    }

    return 0;
}

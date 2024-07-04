#include <iostream>
#include <vector>
#include <iomanip> // for std::setprecision

using namespace std;

// Function to calculate the forward differences
vector<vector<double>> forwardDifferences(const vector<double>& y, int n) {
    vector<vector<double>> diffTable(n, vector<double>(n));
    for (int i = 0; i < n; ++i) {
        diffTable[i][0] = y[i];
    }

    for (int j = 1; j < n; ++j) {
        for (int i = 0; i < n - j; ++i) {
            diffTable[i][j] = diffTable[i + 1][j - 1] - diffTable[i][j - 1];
        }
    }

    return diffTable;
}

// Function to calculate the factorial of a number
int factorial(int n) {
    int fact = 1;
    for (int i = 2; i <= n; ++i) {
        fact *= i;
    }
    return fact;
}

// Function to perform Newton's forward interpolation
double newtonForwardInterpolation(const vector<double>& x, const vector<vector<double>>& diffTable, double value, int n) {
    double result = diffTable[0][0];
    double h = x[1] - x[0];
    double u = (value - x[0]) / h;
    double u_term = u;

    for (int i = 1; i < n; ++i) {
        result += (u_term * diffTable[0][i]) / factorial(i);
        u_term *= (u - i);
    }

    return result;
}

int main() {
    int n;
    cout << "Enter the number of data points: ";
    cin >> n;

    vector<double> x(n);
    vector<double> y(n);

    cout << "Enter the x values: ";
    for (int i = 0; i < n; ++i) {
        cin >> x[i];
    }

    cout << "Enter the y values: ";
    for (int i = 0; i < n; ++i) {
        cin >> y[i];
    }

    vector<vector<double>> diffTable = forwardDifferences(y, n);

    int m;
    cout << "Enter the number of f(x) values to calculate: ";
    cin >> m;

    for (int i = 0; i < m; ++i) {
        double value;
        cout << "Enter the value of x for f(x): ";
        cin >> value;

        double result = newtonForwardInterpolation(x, diffTable, value, n);
        cout << "f(" << value << ") = " << setprecision(6) << result << endl;
    }

    return 0;
}

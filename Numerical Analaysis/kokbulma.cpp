#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

// Function to evaluate the given function f(x) = ax^3 + bx^2 + cx + d
double f(double a, double b, double c, double d, double x) {
    return a * pow(x, 3) + b * pow(x, 2) + c * x + d;
}

// Function to check if the function has a root in the interval (a, b)
bool hasRoot(double a, double b, double c, double d, double lowerBound, double upperBound) {
    return f(a, b, c, d, lowerBound) * f(a, b, c, d, upperBound) < 0;
}

// Bisection method to find the root
double bisection(double a, double b, double c, double d, double lowerBound, double upperBound, double epsilon) {
    if (!hasRoot(a, b, c, d, lowerBound, upperBound)) {
        cerr << "The function does not have a root in the given interval." << endl;
        return NAN;
    }

    double left = lowerBound;
    double right = upperBound;
    double middle;
    while ((right - left) / 2 > epsilon) {
        middle = (left + right) / 2;
        if (f(a, b, c, d, middle) == 0) {
            return middle;
        } else if (f(a, b, c, d, middle) * f(a, b, c, d, left) < 0) {
            right = middle;
        } else {
            left = middle;
        }
    }

    return (left + right) / 2;
}

int main() {
    double a, b, c, d;
    double lowerBound, upperBound;
    double epsilon;

    cout << "Enter the coefficients of the polynomial (a, b, c, d): ";
    cin >> a >> b >> c >> d;

    cout << "Enter the lower and upper bounds of the interval: ";
    cin >> lowerBound >> upperBound;

    cout << "Enter the epsilon value: ";
    cin >> epsilon;

    if (hasRoot(a, b, c, d, lowerBound, upperBound)) {
        double root = bisection(a, b, c, d, lowerBound, upperBound, epsilon);
        cout << "Root found: x = " << setprecision(10) << root << endl;
    } else {
        cout << "The function does not have a root in the interval (" << lowerBound << ", " << upperBound << ")." << endl;
    }

    return 0;
}

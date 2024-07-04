#include <iostream>
#include <cmath>
#include <iomanip>
#include <string>
#include <functional>

using namespace std;

// Function to evaluate a given expression at x
double evaluate_expression(const string& expr_str, double x) {
    // Simple parsing for demonstration, assuming basic arithmetic operations and functions
    // You may need to enhance this for more complex expressions
    string expr = expr_str;
    size_t pos = expr.find("x");
    while (pos != string::npos) {
        expr.replace(pos, 1, to_string(x));
        pos = expr.find("x", pos + 1);
    }
    // Evaluate the expression using std::function
    auto func = [expr](double x) {
        return stod(expr);
    };
    return func(x);
}

int main() {
    double x0, epsilon;
    int maxIterations;
    string function_str, derivative_str;

    cout << "Enter the function f(x): ";
    cin.ignore(); // to clear the buffer
    getline(cin, function_str);
    cout << "Enter the derivative f'(x): ";
    getline(cin, derivative_str);
    cout << "Enter the initial guess x0: ";
    cin >> x0;
    cout << "Enter the epsilon value: ";
    cin >> epsilon;
    cout << "Enter the maximum number of iterations: ";
    cin >> maxIterations;

    double x = x0;
    double fx = evaluate_expression(function_str, x);
    int iteration = 0;

    while (fabs(fx) > epsilon && iteration < maxIterations) {
        double fpx = evaluate_expression(derivative_str, x);
        if (fabs(fpx) < 1e-10) {
            cerr << "Error: Derivative is too small, division by zero risk." << endl;
            return -1;
        }
        x = x - fx / fpx;
        fx = evaluate_expression(function_str, x);
        iteration++;
    }

    if (fabs(fx) <= epsilon) {
        cout << "Root found: x = " << setprecision(10) << x << endl;
        cout << "Number of iterations: " << iteration << endl;
    } else {
        cout << "Root not found within the maximum number of iterations." << endl;
    }

    return 0;
}

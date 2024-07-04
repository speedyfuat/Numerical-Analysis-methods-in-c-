#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <vector>
#include <functional>
#include <map>
#include <cctype>

using namespace std;

// Function to evaluate an expression
double evaluateExpression(const string& expr, double x, double y) {
    map<string, double> variables = { {"x", x}, {"y", y} };

    auto parseTokens = [](const string& str) -> vector<string> {
        istringstream iss(str);
        vector<string> tokens;
        string token;
        while (iss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    };

    auto evalSimpleExpr = [&variables](const vector<string>& tokens) -> double {
        double result = 0;
        double current = 0;
        char operation = '+';
        for (const auto& token : tokens) {
            if (token == "+" || token == "-" || token == "*" || token == "/" || token == "^") {
                operation = token[0];
            } else {
                double value = 0;
                if (variables.find(token) != variables.end()) {
                    value = variables[token];
                } else {
                    value = stod(token);
                }
                switch (operation) {
                    case '+': result += current; current = value; break;
                    case '-': result += current; current = -value; break;
                    case '*': current *= value; break;
                    case '/': current /= value; break;
                    case '^': current = pow(current, value); break;
                }
            }
        }
        return result + current;
    };

    vector<string> tokens = parseTokens(expr);
    return evalSimpleExpr(tokens);
}

// Define the partial derivatives for the Jacobian matrix (numerical approximation)
double df1dx(const function<double(double, double)>& f1, double x, double y, double h) {
    return (f1(x + h, y) - f1(x, y)) / h;
}

double df1dy(const function<double(double, double)>& f1, double x, double y, double h) {
    return (f1(x, y + h) - f1(x, y)) / h;
}

double df2dx(const function<double(double, double)>& f2, double x, double y, double h) {
    return (f2(x + h, y) - f2(x, y)) / h;
}

double df2dy(const function<double(double, double)>& f2, double x, double y, double h) {
    return (f2(x, y + h) - f2(x, y)) / h;
}

// Function to calculate the determinant of the Jacobian matrix
double determinant(double J[2][2]) {
    return J[0][0] * J[1][1] - J[0][1] * J[1][0];
}

// Newton-Raphson method
void newtonRaphson(const function<double(double, double)>& f1, const function<double(double, double)>& f2, double x0, double y0, double tol, int maxIter) {
    double x = x0, y = y0;
    for (int i = 0; i < maxIter; ++i) {
        double J[2][2] = {
            { df1dx(f1, x, y, tol), df1dy(f1, x, y, tol) },
            { df2dx(f2, x, y, tol), df2dy(f2, x, y, tol) }
        };

        double detJ = determinant(J);
        if (fabs(detJ) < tol) {
            cerr << "Jacobian determinant is too close to zero, solution may not be accurate." << endl;
            break;
        }

        double invJ[2][2] = {
            { J[1][1] / detJ, -J[0][1] / detJ },
            { -J[1][0] / detJ, J[0][0] / detJ }
        };

        double F[2] = { f1(x, y), f2(x, y) };

        double dx = -(invJ[0][0] * F[0] + invJ[0][1] * F[1]);
        double dy = -(invJ[1][0] * F[0] + invJ[1][1] * F[1]);

        x += dx;
        y += dy;

        if (sqrt(dx * dx + dy * dy) < tol) {
            break;
        }
    }

    cout << "Newton-Raphson result: x = " << x << ", y = " << y << endl;
}

// Accelerated Newton method
void acceleratedNewton(const function<double(double, double)>& f1, const function<double(double, double)>& f2, double x0, double y0, double tol, int maxIter) {
    double x = x0, y = y0;
    double alpha = 1.0;  // Acceleration factor
    for (int i = 0; i < maxIter; ++i) {
        double J[2][2] = {
            { df1dx(f1, x, y, tol), df1dy(f1, x, y, tol) },
            { df2dx(f2, x, y, tol), df2dy(f2, x, y, tol) }
        };

        double detJ = determinant(J);
        if (fabs(detJ) < tol) {
            cerr << "Jacobian determinant is too close to zero, solution may not be accurate." << endl;
            break;
        }

        double invJ[2][2] = {
            { J[1][1] / detJ, -J[0][1] / detJ },
            { -J[1][0] / detJ, J[0][0] / detJ }
        };

        double F[2] = { f1(x, y), f2(x, y) };

        double dx = -(invJ[0][0] * F[0] + invJ[0][1] * F[1]);
        double dy = -(invJ[1][0] * F[0] + invJ[1][1] * F[1]);

        x += alpha * dx;
        y += alpha * dy;

        if (sqrt(dx * dx + dy * dy) < tol) {
            break;
        }
    }

    cout << "Accelerated Newton result: x = " << x << ", y = " << y << endl;
}

int main() {
    string eq1, eq2;
    double x0, y0, tol;
    int maxIter = 100;

    int choice;
    cout << "Choose method:\n1. Newton-Raphson\n2. Accelerated Newton\n";
    cin >> choice;
    cin.ignore();  // Ignore the newline character after the choice input

    cout << "Enter the first equation f1(x, y) = 0: ";
    getline(cin, eq1);

    cout << "Enter the second equation f2(x, y) = 0: ";
    getline(cin, eq2);

    cout << "Enter initial guess for x (x0): ";
    cin >> x0;

    cout << "Enter initial guess for y (y0): ";
    cin >> y0;

    cout << "Enter tolerance (epsilon): ";
    cin >> tol;

    function<double(double, double)> f1 = [=](double x, double y) { return evaluateExpression(eq1, x, y); };
    function<double(double, double)> f2 = [=](double x, double y) { return evaluateExpression(eq2, x, y); };

    switch (choice) {
        case 1:
            newtonRaphson(f1, f2, x0, y0, tol, maxIter);
            break;
        case 2:
            acceleratedNewton(f1, f2, x0, y0, tol, maxIter);
            break;
        default:
            cout << "Invalid choice" << endl;
    }

    return 0;
}

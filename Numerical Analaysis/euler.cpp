#include <iostream>
#include <functional>
#include <string>
#include <cmath>

// Simple parser to evaluate the function (limited to specific expressions for simplicity)
double evaluateFunction(const std::function<double(double, double)>& func, double x, double y) {
    return func(x, y);
}

std::function<double(double, double)> parseFunction(const std::string& expression) {
    return [expression](double x, double y) {
        // This is a very basic and limited parser, only for demonstration purposes.
        // You can expand this parser or use a library like muParser for more complex expressions.
        if (expression == "3*x - x*y") {
            return 3*x - x*y;
        }
        // Add more cases for different expressions if needed.
        return 0.0; // Default case
    };
}

// Euler's Method
double eulerMethod(double x0, double y0, double h, int steps, const std::function<double(double, double)>& func) {
    double x = x0, y = y0;
    for (int i = 0; i < steps; i++) {
        y = y + h * evaluateFunction(func, x, y);
        x = x + h;
    }
    return y;
}

// Second-order Runge-Kutta Method (Heun's Method)
double rungeKutta2(double x0, double y0, double h, int steps, const std::function<double(double, double)>& func) {
    double x = x0, y = y0;
    for (int i = 0; i < steps; i++) {
        double k1 = evaluateFunction(func, x, y);
        double k2 = evaluateFunction(func, x + h, y + h * k1);
        y = y + (h/2) * (k1 + k2);
        x = x + h;
    }
    return y;
}

// Fourth-order Runge-Kutta Method
double rungeKutta4(double x0, double y0, double h, int steps, const std::function<double(double, double)>& func) {
    double x = x0, y = y0;
    for (int i = 0; i < steps; i++) {
        double k1 = evaluateFunction(func, x, y);
        double k2 = evaluateFunction(func, x + h/2, y + h*k1/2);
        double k3 = evaluateFunction(func, x + h/2, y + h*k2/2);
        double k4 = evaluateFunction(func, x + h, y + h*k3);
        y = y + (h/6) * (k1 + 2*k2 + 2*k3 + k4);
        x = x + h;
    }
    return y;
}

int main() {
    std::string expression;
    std::cout << "Enter the differential equation in the form of f(x, y) = ";
    std::cin >> expression;

    auto func = parseFunction(expression);

    double x0 = 0.0, y0 = 4.0, h = 0.1;
    int steps = 10; // Since we want y(1) and h = 0.1, steps = (1 - 0)/0.1 = 10

    double y_euler = eulerMethod(x0, y0, h, steps, func);
    double y_rk2 = rungeKutta2(x0, y0, h, steps, func);
    double y_rk4 = rungeKutta4(x0, y0, h, steps, func);

    std::cout << "Using Euler's Method: y(1) = " << y_euler << std::endl;
    std::cout << "Using Second-order Runge-Kutta Method: y(1) = " << y_rk2 << std::endl;
    std::cout << "Using Fourth-order Runge-Kutta Method: y(1) = " << y_rk4 << std::endl;

    return 0;
}

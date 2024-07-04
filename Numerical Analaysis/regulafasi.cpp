#include <iostream>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <cmath>
#include <string>
#include <iomanip>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;

// Kullanıcıdan alınan ifadeyi çözümleyen bir parser fonksiyonu
struct ExpressionParser : qi::grammar<std::string::const_iterator, double(double), ascii::space_type> {
    ExpressionParser() : ExpressionParser::base_type(expression) {
        using qi::_1;
        using qi::_val;
        using qi::double_;
        using qi::char_;
        using phoenix::ref;

        expression =
            term[_val = _1] >>
            *(   ('+' >> term[_val += _1])
               | ('-' >> term[_val -= _1])
             );

        term =
            factor[_val = _1] >>
            *(   ('*' >> factor[_val *= _1])
               | ('/' >> factor[_val /= _1])
             );

        factor =
            double_[_val = _1]
            |   '(' >> expression[_val = _1] >> ')'
            |   ('-' >> factor[_val = -_1])
            |   ('+' >> factor[_val = _1])
            |   (qi::lit("cos") >> '(' >> expression[_val = phoenix::bind(&std::cos, _1)] >> ')');
    }

    qi::rule<std::string::const_iterator, double(double), ascii::space_type> expression, term, factor;
};

// f(x) fonksiyonu, kullanıcıdan alınan denklemi değerlendirir
double f(const std::string& expr, double x) {
    ExpressionParser parser;
    std::string::const_iterator iter = expr.begin();
    std::string::const_iterator end = expr.end();
    double result;

    bool r = phrase_parse(iter, end, parser(x), ascii::space, result);
    if (r && iter == end) {
        return result;
    } else {
        std::cerr << "Parsing failed\n";
        return 0.0;
    }
}

// Kesen Kök Yöntemi (Secant Method)
void secantMethod(const std::string& expr, double x0, double x1, double epsilon, int maxIterations) {
    double x2;
    int iteration = 0;
    while (fabs(f(expr, x1)) > epsilon && iteration < maxIterations) {
        x2 = x1 - f(expr, x1) * (x1 - x0) / (f(expr, x1) - f(expr, x0));
        x0 = x1;
        x1 = x2;
        iteration++;
    }
    std::cout << "Secant Method: Root = " << x1 << ", Iterations = " << iteration << std::endl;
}

// Regula Falsi Yöntemi
void regulaFalsi(const std::string& expr, double x0, double x1, double epsilon, int maxIterations) {
    double x2 = x0;
    int iteration = 0;
    while (fabs(f(expr, x2)) > epsilon && iteration < maxIterations) {
        x2 = x0 - f(expr, x0) * (x1 - x0) / (f(expr, x1) - f(expr, x0));
        if (f(expr, x0) * f(expr, x2) < 0) {
            x1 = x2;
        } else {
            x0 = x2;
        }
        iteration++;
    }
    std::cout << "Regula Falsi Method: Root = " << x2 << ", Iterations = " << iteration << std::endl;
}

// Bolzano Yöntemi (Bisection Method)
void bisectionMethod(const std::string& expr, double x0, double x1, double epsilon, int maxIterations) {
    double x2;
    int iteration = 0;
    while ((x1 - x0) / 2 > epsilon && iteration < maxIterations) {
        x2 = (x0 + x1) / 2;
        if (f(expr, x2) == 0.0) {
            break;
        } else if (f(expr, x0) * f(expr, x2) < 0) {
            x1 = x2;
        } else {
            x0 = x2;
        }
        iteration++;
    }
    std::cout << "Bisection Method: Root = " << x2 << ", Iterations = " << iteration << std::endl;
}

int main() {
    std::string expr;
    double x0, x1, epsilon;
    int maxIterations;

    std::cout << "Enter the equation f(x) = 0 (e.g., cos(x) - x): ";
    std::getline(std::cin, expr);
    std::cout << "Enter initial guess x0: ";
    std::cin >> x0;
    std::cout << "Enter initial guess x1: ";
    std::cin >> x1;
    std::cout << "Enter epsilon (tolerance): ";
    std::cin >> epsilon;
    std::cout << "Enter maximum iterations: ";
    std::cin >> maxIterations;

    // a) Kesen Kök
    secantMethod(expr, x0, x1, epsilon, maxIterations);

    // b) Regula Falsi
    regulaFalsi(expr, x0, x1, epsilon, maxIterations);

    // c) Bolzano (Bisection)
    bisectionMethod(expr, x0, x1, epsilon, maxIterations);

    return 0;
}

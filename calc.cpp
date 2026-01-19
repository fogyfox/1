#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>

using namespace std;

string expr;
size_t pos = 0;

void skipSpaces() {
    while (pos < expr.length() && isspace(expr[pos])) {
        pos++;
    }
}

long long parseExpression();

long long parseNumber() {
    skipSpaces();
    
    if (pos >= expr.length() || !isdigit(expr[pos])) {
        throw runtime_error("-1");
    }

    long long res = 0;
    while (pos < expr.length() && isdigit(expr[pos])) {
        res = res * 10 + (expr[pos] - '0');
        pos++;
    }
    return res;
}

long long parseFactor() {
    skipSpaces();
    
    if (pos >= expr.length()) {
        throw runtime_error("-1");
    }

    if (expr[pos] == '(') {
        pos++;
        long long res = parseExpression();
        skipSpaces();
        if (pos >= expr.length() || expr[pos] != ')') {
            throw runtime_error("-1");
        }
        pos++;
        return res;
    }
    
    if (expr[pos] == '-') {
        pos++;
        return -parseFactor();
    }
    
    if (expr[pos] == '+') {
        pos++;
        return parseFactor();
    }

    return parseNumber();
}

long long parseTerm() {
    long long lhs = parseFactor();

    skipSpaces();
    while (pos < expr.length() && (expr[pos] == '*' || expr[pos] == '/')) {
        char op = expr[pos];
        pos++;
        long long rhs = parseFactor();
        if (op == '*') {
            lhs *= rhs;
        } else {
            if (rhs == 0) {
                throw runtime_error("-1");
            }
            lhs /= rhs;
        }
        skipSpaces();
    }
    return lhs;
}

long long parseExpression() {
    long long lhs = parseTerm();

    skipSpaces();
    while (pos < expr.length() && (expr[pos] == '+' || expr[pos] == '-')) {
        char op = expr[pos];
        pos++;
        long long rhs = parseTerm();

        if (op == '+') {
            lhs += rhs;
        } else {
            lhs -= rhs;
        }
        skipSpaces();
    }
    return lhs;
}

int main() {
    getline(cin, expr);

    try {
        if (expr.empty()) {
            throw runtime_error("-1");
        }

        pos = 0;
        long long result = parseExpression();
        
        skipSpaces();
        if (pos != expr.length()) {
            throw runtime_error("-1");
        }

        cout << result << endl;
    } catch (const exception& e) {
        cout << "Error" << endl;
    }

    return 0;
}

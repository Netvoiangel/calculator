#include "ExpressionParser.h"
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <cctype>
#include <cmath>

ExpressionParser::ExpressionParser(const std::map<std::string, std::function<double(double)>>& functions)
    : functions_(functions) {}

double ExpressionParser::parseAndEvaluate(const std::string& expression) {
    std::istringstream stream(expression);
    return parseExpression(stream);
}

char getNextChar(std::istringstream& stream) {
    char ch;
    while (stream >> ch) {
        if (!isspace(ch)) {
            return ch;
        }
    }
    return '\0'; 
}

double parseNumber(std::istringstream& stream) {
    double value;
    stream >> value;
    return value;
}

double ExpressionParser::parseFactor(std::istringstream& stream) {
    char ch = getNextChar(stream);

    if (ch == '(') { 
        double value = parseExpression(stream);
        if (getNextChar(stream) != ')') {
            throw std::invalid_argument("The parenthesis is missing");
        }
        return value;
    } else if (isalpha(ch)) {
        stream.putback(ch);
        return parseFunction(stream);
    } else { 
        stream.putback(ch);
        return parseNumber(stream);
    }
}

double ExpressionParser::parseFunction(std::istringstream& stream) {
    std::string funcName;
    char ch;

    while (stream >> ch && isalpha(ch)) {
        funcName += ch;
    }
    stream.putback(ch);

    if (getNextChar(stream) != '(') {
        throw std::invalid_argument("Expected an opening parenthesis after the function name");
    }

    double arg = parseExpression(stream);

    if (getNextChar(stream) != ')') {
        throw std::invalid_argument("A closing parenthesis was expected after the function argument");
    }

    return applyFunction(funcName, arg);
}

double ExpressionParser::applyFunction(const std::string& funcName, double arg) {
    auto it = functions_.find(funcName);
    if (it != functions_.end()) {
        return it->second(arg);
    }
    throw std::invalid_argument("Unknown error: " + funcName);
}

double ExpressionParser::parseTerm(std::istringstream& stream) {
    double value = parseFactor(stream);

    while (true) {
        char op = getNextChar(stream);
        if (op == '*' || op == '/') {
            double nextValue = parseFactor(stream);
            if (op == '*') {
                value *= nextValue;
            } else {
                value /= nextValue;
            }
        } else {
            stream.putback(op);
            break;
        }
    }

    return value;
}

double ExpressionParser::parseExpression(std::istringstream& stream) {
    double value = parseTerm(stream);

    while (true) {
        char op = getNextChar(stream);
        if (op == '+' || op == '-') {
            double nextValue = parseTerm(stream);
            if (op == '+') {
                value += nextValue;
            } else {
                value -= nextValue;
            }
        } else {
            stream.putback(op);
            break;
        }
    }

    return value;
}

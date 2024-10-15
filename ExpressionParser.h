#pragma once
#include <string>
#include <map>
#include <functional>
#include <sstream> // std::istringstream

class ExpressionParser {
public:
    ExpressionParser(const std::map<std::string, std::function<double(double)>>& functions);
    double parseAndEvaluate(const std::string& expression);

private:
    std::map<std::string, std::function<double(double)>> functions_;

    double parseExpression(std::istringstream& stream);
    double parseTerm(std::istringstream& stream);
    double parseFactor(std::istringstream& stream);
    double parseFunction(std::istringstream& stream);
    double applyFunction(const std::string& funcName, double arg);
};

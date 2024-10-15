#pragma once
#include <string>

class Plugin {
public:
    virtual ~Plugin() = default;
    virtual std::string getName() const = 0;
    virtual double execute(double x) const = 0;
};

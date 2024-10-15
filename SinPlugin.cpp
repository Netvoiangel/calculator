#include "Plugin.h"
#include <cmath>

class SinPlugin : public Plugin {
public:
    std::string getName() const override {
        return "sin";
    }

    double execute(double x) const override {
        return std::sin(x);
    }
};

extern "C" Plugin* createPlugin() {
    return new SinPlugin();
}

#include <iostream>
#include <string>
#include <filesystem>
#include <dlfcn.h> // dll for macos
#include <map>
#include <functional>
#include "ExpressionParser.h"
#include "Plugin.h"

namespace fs = std::filesystem;

void loadPlugins(std::map<std::string, std::function<double(double)>>& functions) {
    std::string pluginDir = "./plugins";

    for (const auto& entry : fs::directory_iterator(pluginDir)) {
        if (entry.path().extension() == ".dylib") {
            void* handle = dlopen(entry.path().c_str(), RTLD_LAZY);
            if (!handle) {
                std::cerr << "The plugin could not be loaded: " << entry.path() << "\n";
                continue;
            }

            using createFuncType = Plugin* (*)();
            createFuncType createPlugin = (createFuncType)dlsym(handle, "createPlugin");

            if (createPlugin) {
                Plugin* plugin = createPlugin();
                functions[plugin->getName()] = [plugin](double x) { return plugin->execute(x); };
            } else {
                std::cerr << "The createPlugin function could not be found in the plugin: " << entry.path() << "\n";
            }
        }
    }
}

int main() {
    std::map<std::string, std::function<double(double)>> functions;
    loadPlugins(functions);

    ExpressionParser parser(functions);
    std::string expression;

    std::cout << "Enter expression: ";
    std::getline(std::cin, expression);

    try {
        double result = parser.parseAndEvaluate(expression);
        std::cout << "Result: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

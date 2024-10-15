# Calculator Application

A console-based calculator application written in C++ that supports basic arithmetic operations (`+`, `-`, `*`, `/`) and parentheses. Additionally, it supports dynamic loading of plugins for extended functionality (e.g., `sin`, `cos`) using dynamic libraries (`.dylib` for macOS and `.dll` for Windows).

## Features

- Supports basic arithmetic operations: `+`, `-`, `*`, `/`
- Handles parentheses for complex expressions
- Dynamically loads plugins from a `./plugins` directory to support additional functions (e.g., `sin`, `cos`, `deg`)
- Extendable through plugins that contain one function per dynamic library

## Program Structure

```
project-root/
│
├── calc.cpp                 # The main file for the calculator application
├── ExpressionParser.h       # Header file for the expression parser class
├── ExpressionParser.cpp     # Implementation of the expression parser
├── Plugin.h                 # Abstract base class for plugins
├── plugins/                 # Directory containing plugins (.dylib for macOS or .dll for Windows)
│   ├── funcsin.dylib        # Example plugin providing the sin function (macOS)
│   └── ...                  # Additional plugins can be placed here
└── README.md                # This README file
```

## Compilation and Running

### macOS

#### Prerequisites
- Ensure you have the `g++` compiler installed (comes with Xcode Command Line Tools).
- Dynamic libraries (`.dylib`) should be used for plugins on macOS.

#### Compilation

1. Compile the main program:

   ```bash
   g++ -std=c++17 -o calc calc.cpp ExpressionParser.cpp -ldl
   ```

2. Compile a plugin (e.g., `SinPlugin`):

   ```bash
   g++ -std=c++17 -shared -o plugins/funcsin.dylib -fPIC SinPlugin.cpp
   ```

#### Running the Program

```bash
./calc
```

- The program will prompt you to enter an expression, e.g., `16 + 4 * (3 - 1)`.
- The result will be displayed after evaluation.
- You can also use functions from plugins, e.g., `2^4 + sin(90)`.

### Windows

#### Prerequisites
- Ensure you have `g++` from MinGW or another compatible C++ compiler installed.
- Dynamic libraries (`.dll`) should be used for plugins on Windows.

#### Compilation

1. Compile the main program:

   ```bash
   g++ -std=c++17 -o calc.exe calc.cpp ExpressionParser.cpp
   ```

2. Compile a plugin (e.g., `SinPlugin`):

   ```bash
   g++ -std=c++17 -shared -o plugins/funcsin.dll -fPIC SinPlugin.cpp
   ```

#### Running the Program

```bash
calc.exe
```

- Similar to macOS, the program will prompt you to enter an expression, and you can use functions from plugins like `sin`.

## Creating Plugins

Each plugin must:
- Implement a single function and derive from the `Plugin` base class.
- Export a function `createPlugin` that returns a pointer to the plugin instance.

Example `SinPlugin.cpp`:

```cpp
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
```

Compile and place it in the `./plugins` directory as shown above.

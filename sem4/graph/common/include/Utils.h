#pragma once

#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <cstdlib>

namespace graph {

inline int readInt(const char* prompt) {
    int value;
    for (;;) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Ошибка ввода\n";
    }
}

template<typename T>
bool checkAndRun(const std::unique_ptr<T>& ptr,
    std::function<void()> action, const char* errorMsg)
{
    if (ptr) {
        action();
        return true;
    }
    std::cout << "[FAIL] " << errorMsg << "\n";
    return false;
}

inline void runPythonScript(const std::string& scriptName, const std::vector<std::string>& args) {
    std::string fullScriptName = scriptName;
    if (scriptName.find("plot_") == 0) {
        fullScriptName = "visualization/" + scriptName;
    }
    std::string module = "scripts." + fullScriptName;
    size_t dotPos = module.find_last_of('.');
    if (dotPos != std::string::npos) {
        module = module.substr(0, dotPos);
    }
    std::replace(module.begin(), module.end(), '/', '.');
    std::string cmd = "./venv/bin/python -m " + module;
    for (const auto& arg : args) {
        cmd += " " + arg;
    }
    int res = system(cmd.c_str());
    if (res != 0) {
        std::cerr << "[FAIL] Не удалось запустить Python-скрипт: " << fullScriptName << "\n";
    }
}

} // namespace graph

#pragma once

#include <cstdlib>
#include <functional>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

namespace graph {

inline int readInt(char const* prompt) {
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

template <typename T>
bool checkAndRun(std::unique_ptr<T> const& ptr, std::function<void()> action,
                 char const* errorMsg) {
    if (ptr) {
        action();
        return true;
    }
    std::cout << "[FAIL] " << errorMsg << "\n";
    return false;
}

inline std::string venvPath = "/Users/ivan/myvenv/bin/python";
inline void runPythonScript(std::string const& scriptName, std::vector<std::string> const& args) {
    std::string full_script_name = scriptName;
    if (scriptName.find("plot_") == 0) {
        full_script_name = "visualization/" + scriptName;
    }
    std::string module = "scripts." + full_script_name;
    size_t dot_pos = module.find_last_of('.');
    if (dot_pos != std::string::npos) {
        module = module.substr(0, dot_pos);
    }
    std::replace(module.begin(), module.end(), '/', '.');
    std::string cmd = venvPath + " -m " + module;
    for (auto const& arg : args) {
        cmd += " " + arg;
    }
    int res = system(cmd.c_str());
    if (res != 0) {
        std::cerr << "[FAIL] Не удалось запустить Python-скрипт: " << full_script_name << "\n";
    }
}

}  // namespace graph

#include <iostream>
#include <cstdlib>
#include <string>

#include <Menu.h>

using graph::Menu;
using graph::readInt;

int main() {
    Menu menu;
    bool running = true;
    bool web_mode = false;

    if (const char* web_env = std::getenv("GRAPH_WEB_MODE"); web_env != nullptr) {
        web_mode = std::string(web_env) == "1";
    }

    while (running) {
        if (!web_mode) {
            menu.show();
        }

        int choice = readInt(web_mode ? "" : "Ваш выбор: ");
        if (choice == 0) {
            running = false;
        } else {
            menu.handleChoice(choice);
        }
    }

    return 0;
}

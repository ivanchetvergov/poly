#include <iostream>

#include <Menu.h>

using graph::Menu;
using graph::readInt;

int main() {
    Menu menu;
    bool running = true;

    while (running) {
        menu.show();
        int choice = readInt("Ваш выбор: ");
        if (choice == 0) {
            running = false;
        } else {
            menu.handleChoice(choice);
        }
    }

    return 0;
}

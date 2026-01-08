#include <iostream>
#include <Menu.h>

using namespace graph;

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

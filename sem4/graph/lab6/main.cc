#include "Runner.h"
#include <iostream>

int main() {
    dict::lab6::Runner runner;

    std::cout << "Lab6 Demo\n";
    std::cout << "1. HashTable Demo\n";
    std::cout << "2. RBTree Demo\n";
    std::cout << "3. TextGenerator Demo\n";
    std::cout << "Choose: ";

    int choice;
    std::cin >> choice;

    switch (choice) {
        case 1:
            runner.runHashTableDemo();
            break;
        case 2:
            runner.runRBTreeDemo();
            break;
        case 3:
            runner.runTextGeneratorDemo();
            break;
        default:
            std::cout << "Invalid choice\n";
    }

    return 0;
}

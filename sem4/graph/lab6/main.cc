#include "HashTable.h"
#include "RBTree.h"
#include "TextGenerator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <locale>
#include <clocale>

using namespace dict;

void showMenu() {
    std::cout << "\n=== Словарь ===" << std::endl;
    std::cout << "1. Использовать хеш-таблицу" << std::endl;
    std::cout << "2. Использовать красно-черное дерево" << std::endl;
    std::cout << "3. Сгенерировать текст" << std::endl;
    std::cout << "0. Выход" << std::endl;
    std::cout << "Выбор: ";
}

void hashTableMenu() {
    HashTable table;
    std::string choice, word, filename;
    
    while (true) {
        std::cout << "\n--- Хеш-таблица ---" << std::endl;
        std::cout << "1. Добавить слово" << std::endl;
        std::cout << "2. Найти слово" << std::endl;
        std::cout << "3. Удалить слово" << std::endl;
        std::cout << "4. Очистить" << std::endl;
        std::cout << "5. Загрузить из файла" << std::endl;
        std::cout << "6. Сохранить в файл" << std::endl;
        std::cout << "7. Показать содержимое" << std::endl;
        std::cout << "8. Размер" << std::endl;
        std::cout << "0. Назад" << std::endl;
        std::cout << "Выбор: ";
        std::cin >> choice;
        
        if (choice == "1") {
            std::cout << "Слово: ";
            std::cin >> word;
            table.insert(word);
            std::cout << "Добавлено!" << std::endl;
        } else if (choice == "2") {
            std::cout << "Слово: ";
            std::cin >> word;
            std::cout << (table.search(word) ? "Найдено" : "Не найдено") << std::endl;
        } else if (choice == "3") {
            std::cout << "Слово: ";
            std::cin >> word;
            std::cout << (table.remove(word) ? "Удалено" : "Не найдено") << std::endl;
        } else if (choice == "4") {
            table.clear();
            std::cout << "Очищено!" << std::endl;
        } else if (choice == "5") {
            std::cout << "Имя файла: ";
            std::cin >> filename;
            std::cout << (table.loadFromFile(filename) ? "Загружено!" : "Ошибка") << std::endl;
        } else if (choice == "6") {
            std::cout << "Имя файла: ";
            std::cin >> filename;
            std::cout << (table.saveToFile(filename) ? "Сохранено!" : "Ошибка") << std::endl;
        } else if (choice == "7") {
            table.printTable();
        } else if (choice == "8") {
            std::cout << "Размер: " << table.size() << std::endl;
        } else if (choice == "0") {
            break;
        }
    }
}

void rbTreeMenu() {
    RBTree tree;
    std::string choice, word, filename;
    
    while (true) {
        std::cout << "\n--- Красно-черное дерево ---" << std::endl;
        std::cout << "1. Добавить слово" << std::endl;
        std::cout << "2. Найти слово" << std::endl;
        std::cout << "3. Удалить слово" << std::endl;
        std::cout << "4. Очистить" << std::endl;
        std::cout << "5. Загрузить из файла" << std::endl;
        std::cout << "6. Сохранить в файл" << std::endl;
        std::cout << "7. Показать дерево" << std::endl;
        std::cout << "0. Назад" << std::endl;
        std::cout << "Выбор: ";
        std::cin >> choice;
        
        if (choice == "1") {
            std::cout << "Слово: ";
            std::cin >> word;
            tree.insert(word);
            std::cout << "Добавлено!" << std::endl;
        } else if (choice == "2") {
            std::cout << "Слово: ";
            std::cin >> word;
            std::cout << (tree.search(word) ? "Найдено" : "Не найдено") << std::endl;
        } else if (choice == "3") {
            std::cout << "Слово: ";
            std::cin >> word;
            std::cout << (tree.remove(word) ? "Удалено" : "Не найдено") << std::endl;
        } else if (choice == "4") {
            tree.clear();
            std::cout << "Очищено!" << std::endl;
        } else if (choice == "5") {
            std::cout << "Имя файла: ";
            std::cin >> filename;
            std::cout << (tree.loadFromFile(filename) ? "Загружено!" : "Ошибка") << std::endl;
        } else if (choice == "6") {
            std::cout << "Имя файла: ";
            std::cin >> filename;
            std::cout << (tree.saveToFile(filename) ? "Сохранено!" : "Ошибка") << std::endl;
        } else if (choice == "7") {
            tree.printTree();
        } else if (choice == "0") {
            break;
        }
    }
}

void generatorMenu() {
    TextGenerator gen;
    std::string choice, filename;
    size_t length;
    
    std::cout << "\n--- Генератор текста ---" << std::endl;
    std::cout << "Длина текста (по умолчанию 10000): ";
    std::cin >> length;
    if (length == 0) length = 10000;
    
    std::cout << "Имя файла для сохранения: ";
    std::cin >> filename;
    
    if (gen.saveToFile(filename, length)) {
        std::cout << "Текст сгенерирован и сохранен в " << filename << std::endl;
    } else {
        std::cout << "Ошибка сохранения файла" << std::endl;
    }
}

int main() {
    // Установка UTF-8 локали для корректной работы с кириллицей
    setlocale(LC_ALL, "ru_RU.UTF-8");
    std::ios_base::sync_with_stdio(false);
    
    std::string choice;
    
    while (true) {
        showMenu();
        std::cin >> choice;
        
        if (choice == "1") {
            hashTableMenu();
        } else if (choice == "2") {
            rbTreeMenu();
        } else if (choice == "3") {
            generatorMenu();
        } else if (choice == "0") {
            std::cout << "До свидания!" << std::endl;
            break;
        } else {
            std::cout << "Неверный выбор" << std::endl;
        }
    }
    
    return 0;
}

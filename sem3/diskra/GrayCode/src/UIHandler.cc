// UIHandler.cc
#include "UIHandler.h"
#include "MultisetArithmetic.h"
#include "Multiset.h"
#include "IO.h"
#include "Exceptions.h"
#include <iostream>
#include <stdexcept>

using std::cout;
using std::cerr;

UIHandler::UIHandler(Multiset& universe, Multiset& A, Multiset& B)
    : universe_(universe), A_(A), B_(B) {}

void UIHandler::showMenu() {
    cout << "\n--- главное меню ---\n";
    cout << "1. сгенерировать универсум мультимножеств\n";
    if (!universe_.isEmpty()) {
        cout << "2. заполнить множества A и B вручную\n";
        cout << "3. заполнить множества A и B автоматически\n";
    }
    if (universe_.letUserUseOp_){
        cout << "4. выполнить операции над множествами\n";
    }
    cout << "5. вывести множество\n";
    cout << "6. выход\n";
    cout << "---------------------\n";
}

void UIHandler::showDisplayMenu() {
    cout << "\n--- подменю вывода ---\n";
    cout << "1. вывести универсум\n";
    cout << "2. вывести множество A\n";
    cout << "3. вывести множество B\n";
    cout << "4. вернуться в главное меню\n";
    cout << "-----------------------\n";
}

void UIHandler::showOperationsMenu() {
    cout << "\n-------- подменю операций ---------\n\n";
    cout << "    1. объединение            (A | B)\n";
    cout << "    2. пересечение            (A & B)\n";
    cout << "    3. дополнение к A         (U \\ A)\n";
    cout << "    4. дополнение к B         (U \\ B)\n";
    cout << "    5. разность               (A \\ B)\n";
    cout << "    6. разность               (B \\ A)\n";
    cout << "    7. симм. разность         (A ^ B)\n";
    cout << "    8. сумма                  (A + B)\n";
    cout << "    9. арифм. разность        (A - B)\n";
    cout << "    10. арифм. разность       (B - A)\n";
    cout << "    11. произведение          (A * B)\n";
    cout << "    12. деление               (A / B)\n";
    cout << "    13. деление               (B / A)\n";
    cout << "    14. в главное меню\n";
    cout << "-------------------------------------\n";
}

void UIHandler::handleGenerateUniverse() {
    int n = readInteger("введите разрядность (n): ");
    universe_.fillRandomly(n);
    cout << "универсум успешно сгенерирован.\n";
    universe_.print();
}

void UIHandler::handleManualFill() {
    if (universe_.isEmpty()) {
        throw InvalidOperationException("универсум пуст. сначала сгенерируйте его (опция 1).");
    }
    cout << "--- заполнение множества A ---\n";
    A_.fillManually(universe_);
    cout << "--- заполнение множества B ---\n";
    B_.fillManually(universe_);
}

void UIHandler::handleAutomaticFill() {
    if (universe_.isEmpty()) {
        throw InvalidOperationException("универсум пуст. сначала сгенерируйте его (опция 1).");
    }
    cout << "--- автоматическое заполнение множества A ---\n";
    int cardinalityA = readInteger("введите желаемую мощность для A: ");
    if (cardinalityA > universe_.getCardinality()) {
        throw InvalidValueException("желаемая мощность для A превышает мощность универсума.");
    }
    A_.fillAutomatically(universe_, cardinalityA);
    cout << "--- автоматическое заполнение множества B ---\n";
    int cardinalityB = readInteger("введите желаемую мощность для B: ");
    if (cardinalityB > universe_.getCardinality()) {
        throw InvalidValueException("желаемая мощность для B превышает мощность универсума.");
    }
    B_.fillAutomatically(universe_, cardinalityB);
}


void UIHandler::handleOperationsMenu() {
    while (true) {
        try {
            showOperationsMenu();
            MultisetArithmetic arithmeticHandler(universe_);
            int choice = readInteger("выберите операцию: ");
            Multiset result;

            switch (choice) {
                case 1: 
                    result = A_ | B_;
                    cout << "\n--- результат объединения ---\n";
                    result.print();
                    break;
                case 2:
                    result = A_ & B_;
                    cout << "\n--- результат пересечения ---\n";
                    result.print();
                    break;
                case 3:
                    result = universe_ - A_;
                    cout << "\n--- результат дополнения ---\n";
                    result.print();
                    break;
                case 4:
                    result = universe_ - B_;
                    cout << "\n--- результат дополнения ---\n";
                    result.print();
                    break;
                case 5:
                    result = A_ - B_;
                    cout << "\n--- результат разности ---\n";
                    result.print();
                    break;
                case 6:
                    result = B_ - A_;
                    cout << "\n--- результат разности ---\n";
                    result.print();
                    break;
                case 7:
                    result = A_ ^ B_;
                    cout << "\n--- результат симметрической разности ---\n";
                    result.print();
                    break;
                case 8:
                    result = arithmeticHandler.sum(A_, B_);
                    cout << "\n--- результат арифметической суммы ---\n";
                    result.print();
                    break;
                case 9: 
                    result = arithmeticHandler.difference(A_, B_);
                    cout << "\n--- результат арифметической разности (A - B) ---\n";
                    result.print();
                    break;
                case 10: 
                    result = arithmeticHandler.difference(B_, A_);
                    cout << "\n--- результат арифметической разности (B - A) ---\n";
                    result.print();
                    break;
                case 11:
                    result = arithmeticHandler.product(A_, B_);
                    cout << "\n--- результат арифметического произведения ---\n";
                    result.print();
                    break;
                case 12:
                    result = arithmeticHandler.division(A_, B_);
                    cout << "\n--- результат арифметического деления (A / B) ---\n";
                    result.print();
                    break;
                case 13:
                    result = arithmeticHandler.division(B_, A_);
                    cout << "\n--- результат арифметического деления (B / A) ---\n";
                    result.print();
                    break;
                case 14:
                    return;
                default:
                    throw InvalidValueException("неверный выбор.");
            }
        }
        catch (const InvalidOperationException& e) {
            cerr << e.what() << '\n';
        }

    }
}

void UIHandler::handleMainMenu() {
    try {
        this->showMenu();
        int choice = readInteger("выберите операцию: ");

        switch (choice) {
            case 1:
                this->handleGenerateUniverse();
                break;
            case 2:
                this->handleManualFill();
                break;
            case 3:
                this->handleAutomaticFill();
                break;
            case 4:
                this->handleOperationsMenu();
                break;
            case 5: 
                this->handleDisplayMenu();
                break;
            case 6:
                cout << "выход из программы.\n";
                exit(0);
            default:
                throw InvalidValueException("неверный выбор.");
        }
    }
    catch (const InvalidOperationException& e) {
        cerr << e.what() << '\n';
    }
    catch (const InvalidValueException& e) {
        cerr << e.what() << '\n';
    }
    catch (const MultisetException& e) {
        cerr << e.what() << '\n';
    }
    catch (const std::out_of_range& e) { 
        cerr << "\nerror: " << e.what() << '\n';
    }
    catch (const std::exception& e) {
        cerr << "произошла непредвиденная ошибка: " << e.what() << '\n';
    }
}

void UIHandler::handleDisplayMenu() {
    while (true) {
        try {
            showDisplayMenu();
            int choice = readInteger("выберите множество для вывода: ");
            switch (choice) {
                case 1:
                    if (universe_.isEmpty()) {
                        throw InvalidOperationException("универсум пуст. сначала сгенерируйте его.");
                    }
                    cout << "\n--- универсум ---\n";
                    universe_.print();
                    break;
                case 2:
                    if (A_.isEmpty()) {
                        throw InvalidOperationException("множество A пусто. сначала заполните его.");
                    }
                    cout << "\n--- множество A ---\n";
                    A_.print();
                    break;
                case 3:
                    if (B_.isEmpty()) {
                        throw InvalidOperationException("множество B пусто. сначала заполните его.");
                    }
                    cout << "\n--- множество B ---\n";
                    B_.print();
                    break;
                case 4:
                    return;
                default:
                    throw InvalidValueException("неверный выбор.");
            }
        }
        catch (const std::exception& e) {
            cerr << e.what() << '\n';
        }
    }
}

void UIHandler::run() {
    while (true) {
        this->handleMainMenu();
    }
}
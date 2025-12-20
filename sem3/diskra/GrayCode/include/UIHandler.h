#pragma once

#include "Multiset.h"

class UIHandler {
public:
    UIHandler(Multiset& universe, Multiset& A, Multiset& B);
    void run();

private:
    Multiset& universe_;
    Multiset& A_;
    Multiset& B_;


    void handleDisplayMenu();
    void handleMainMenu();
    void handleGenerateUniverse();
    void handleManualFill();
    void handleAutomaticFill();
    void handleOperationsMenu();
    void showMenu();
    void showOperationsMenu();
    void showDisplayMenu();
};
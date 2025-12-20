// main.cc
#include "UIHandler.h"
#include "Multiset.h"

int main() {
    Multiset universe;
    Multiset A, B;

    UIHandler handler(universe, A, B);
    handler.run();

    return 0;
}
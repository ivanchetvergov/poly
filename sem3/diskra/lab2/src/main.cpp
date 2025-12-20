#include "bdd.h"
#include "config.h"
#include "ui.h"
#include "zheg.h"

int main() {
    auto vars = config::variables();
    auto truth = config::truthVector();

    ZhegalkinPolynomial polynomial(static_cast<int>(vars.size()));
    polynomial.setVariableNames(vars);
    polynomial.setTruthTableFromVector(truth);
    polynomial.buildTriangle();

    BDDGraph bdd(vars, truth);
    ConsoleUi ui(vars, truth, polynomial, bdd);
    ui.run();

    return 0;
}

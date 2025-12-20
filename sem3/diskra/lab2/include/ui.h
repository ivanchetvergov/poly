#pragma once

#include <vector>
#include <string>

class ZhegalkinPolynomial;
class BDDGraph;

class ConsoleUi {
public:
    ConsoleUi(std::vector<std::string> variables,
              std::vector<int> truthVector,
              ZhegalkinPolynomial& polynomial,
              BDDGraph& bdd);

    void run();

private:
    std::vector<std::string> variables_;
    std::vector<int> truthVector_;
    ZhegalkinPolynomial& polynomial_;
    BDDGraph& bdd_;

    void showInfo();
    std::vector<int> requestValues() const;
};

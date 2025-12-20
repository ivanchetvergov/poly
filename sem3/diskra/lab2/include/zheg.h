#pragma once

#include <string>
#include <vector>

class ZhegalkinPolynomial {
public:
    explicit ZhegalkinPolynomial(int numVars);

    void setTruthTableFromVector(const std::vector<int>& values);
    void setVariableNames(std::vector<std::string> names);
    void buildTriangle();
    void printTriangle() const;

    std::string buildPolynomial();
    int evaluatePolynomial(const std::vector<int>& values);

private:
    int numVars_{0};
    std::vector<int> truthTable_;
    std::vector<std::vector<int>> triangle_;
    std::vector<std::string> variableNames_;
    std::vector<std::string> terms_;
    std::vector<int> coefficients_;
    bool triangleReady_{false};
    bool polynomialReady_{false};
    std::string cachedPolynomial_;

    std::string generateTerm(int index) const;
    void ensureTriangle();
    int termValue(int mask, const std::vector<int>& values) const;
};

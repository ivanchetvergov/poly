#include "help.h"

#include <sstream>
#include <stdexcept>

namespace {

std::string buildLiteral(const std::string& name, bool value, bool forDnf) {
    if (forDnf) {
        return value ? name : "~" + name;
    }
    return value ? "~" + name : name;
}

std::string join(const std::vector<std::string>& items, const std::string& glue) {
    if (items.empty()) {
        return {};
    }
    std::ostringstream oss;
    for (size_t i = 0; i < items.size(); ++i) {
        if (i) {
            oss << glue;
        }
        oss << items[i];
    }
    return oss.str();
}

size_t assignmentToIndex(const std::vector<int>& values) {
    size_t index = 0;
    for (int value : values) {
        index = (index << 1) | (value ? 1U : 0U);
    }
    return index;
}

std::string termForRow(const std::vector<std::string>& vars, size_t rowIndex, bool disjunctive) {
    std::vector<std::string> literals;
    literals.reserve(vars.size());
    for (size_t var = 0; var < vars.size(); ++var) {
        const size_t shift = vars.size() - 1 - var;
        const bool bit = (rowIndex >> shift) & 1U;
        literals.push_back(buildLiteral(vars[var], bit, disjunctive));
    }
    return disjunctive ? join(literals, " & ") : "(" + join(literals, " | ") + ")";
}

std::string buildNormalForm(const std::vector<std::string>& vars,
                            const std::vector<int>& truthVector,
                            bool disjunctive) {
    std::vector<std::string> terms;
    for (size_t row = 0; row < truthVector.size(); ++row) {
        const bool pickRow = disjunctive ? static_cast<bool>(truthVector[row]) : !truthVector[row];
        if (pickRow) {
            terms.push_back(termForRow(vars, row, disjunctive));
        }
    }
    if (terms.empty()) {
        return disjunctive ? "0" : "1";
    }
    return join(terms, disjunctive ? " OR " : " AND ");
}

}  // namespace

namespace boolean_help {

std::string truthVectorString(const std::vector<int>& truthVector) {
    std::ostringstream oss;
    for (int bit : truthVector) {
        oss << (bit ? '1' : '0');
    }
    return oss.str();
}

std::string buildSDNF(const std::vector<std::string>& vars, const std::vector<int>& truthVector) {
    return buildNormalForm(vars, truthVector, true);
}

std::string buildSKNF(const std::vector<std::string>& vars, const std::vector<int>& truthVector) {
    return buildNormalForm(vars, truthVector, false);
}

int evaluateDirect(const std::vector<int>& truthVector, const std::vector<int>& values) {
    size_t expected = 0;
    size_t size = truthVector.size();
    while (size > 1 && size % 2 == 0) {
        size /= 2;
        ++expected;
    }
    if (size != 1 || (static_cast<size_t>(1) << expected) != truthVector.size()) {
        throw std::invalid_argument("Некорректный размер таблицы истинности");
    }
    if (values.size() != expected) {
        throw std::invalid_argument("Некорректное число переменных");
    }
    const size_t index = assignmentToIndex(values);
    return truthVector.at(index);
}

}  // namespace boolean_help

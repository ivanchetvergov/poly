#pragma once

#include <string>
#include <vector>

namespace boolean_help {

std::string truthVectorString(const std::vector<int>& truthVector);
std::string buildSDNF(const std::vector<std::string>& vars, const std::vector<int>& truthVector);
std::string buildSKNF(const std::vector<std::string>& vars, const std::vector<int>& truthVector);
int evaluateDirect(const std::vector<int>& truthVector, const std::vector<int>& values);

}  // namespace boolean_help

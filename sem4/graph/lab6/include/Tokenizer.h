#pragma once
#include <string>
#include <vector>

namespace dict {

class Tokenizer {
public:
    // Always tokenize into unigrams (single words)
    static bool tokensFromFile(std::string const& filename, std::vector<std::string>& out);

    static std::vector<std::string> tokensFromContent(std::string const& content);
};

} // namespace dict

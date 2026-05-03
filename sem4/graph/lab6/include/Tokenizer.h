#pragma once
#include <string>
#include <vector>

namespace dict {

class Tokenizer {
public:
    static bool ngramsFromFile(std::string const& filename, int ngramSize, std::vector<std::string>& out);

    static std::vector<std::string> ngramsFromContent(std::string const& content, int ngramSize);
};

} // namespace dict

#include "Tokenizer.h"
#include "FileHandler.h"

#include <array>
#include <cctype>
#include <sstream>

using graph::FileHandler;

namespace dict {

namespace {

std::string normalizeToken(std::string token) {
    static std::array<std::string, 8> const utf8Punct = {
        "—", "–", "…", "«", "»", "“", "”", "„"
    };

    bool changed = true;
    while (changed && !token.empty()) {
        changed = false;

        unsigned char first = static_cast<unsigned char>(token.front());
        if (first < 128 && std::ispunct(first)) {
            token.erase(token.begin());
            changed = true;
            continue;
        }

        unsigned char last = static_cast<unsigned char>(token.back());
        if (last < 128 && std::ispunct(last)) {
            token.pop_back();
            changed = true;
            continue;
        }

        for (auto const& p : utf8Punct) {
            if (token.size() >= p.size() && token.starts_with(p)) {
                token.erase(0, p.size());
                changed = true;
                break;
            }
            if (token.size() >= p.size() && token.ends_with(p)) {
                token.erase(token.size() - p.size(), p.size());
                changed = true;
                break;
            }
        }
    }

    for (char& c : token) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (uc < 128) c = static_cast<char>(std::tolower(uc));
    }
    return token;
}

} // namespace

bool Tokenizer::ngramsFromFile(std::string const& filename, int ngramSize, std::vector<std::string>& out) {
    std::string content;
    if (!FileHandler::loadFromFile(filename, content)) return false;
    out = ngramsFromContent(content, ngramSize);
    return true;
}

std::vector<std::string> Tokenizer::ngramsFromContent(std::string const& content, int ngramSize) {
    std::vector<std::string> words;
    std::istringstream iss(content);
    std::string raw;
    while (iss >> raw) {
        auto token = normalizeToken(raw);
        if (!token.empty()) words.push_back(token);
    }

    if (ngramSize < 1) ngramSize = 1;
    std::vector<std::string> out;
    for (size_t i = 0; i + ngramSize <= words.size(); ++i) {
        std::string ngram = words[i];
        for (int j = 1; j < ngramSize; ++j) {
            ngram += " " + words[i + j];
        }
        out.push_back(ngram);
    }
    return out;
}

} // namespace dict

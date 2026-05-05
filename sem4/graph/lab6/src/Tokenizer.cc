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

    std::string lowered;
    for (size_t i = 0; i < token.size(); ++i) {
        unsigned char uc = static_cast<unsigned char>(token[i]);
        if (uc < 128) {
            lowered += static_cast<char>(std::tolower(uc));
        } else if (uc == 0xD0 && i + 1 < token.size()) {
            unsigned char next = static_cast<unsigned char>(token[i + 1]);
            if (next >= 0x90 && next <= 0x9F) { // А-П
                lowered += static_cast<char>(0xD0);
                lowered += static_cast<char>(next + 0x20);
                i++;
            } else if (next >= 0xA0 && next <= 0xAF) { // Р-Я
                lowered += static_cast<char>(0xD1);
                lowered += static_cast<char>(next - 0x20);
                i++;
            } else if (next == 0x81) { // Ё
                lowered += static_cast<char>(0xD1);
                lowered += static_cast<char>(0x91);
                i++;
            } else {
                lowered += static_cast<char>(uc);
            }
        } else {
            lowered += static_cast<char>(uc);
        }
    }
    return lowered;
}

} // namespace

bool Tokenizer::tokensFromFile(std::string const& filename, std::vector<std::string>& out) {
    std::string content;
    if (!FileHandler::loadFromFile(filename, content)) return false;
    out = tokensFromContent(content);
    return true;
}

std::vector<std::string> Tokenizer::tokensFromContent(std::string const& content) {
    std::vector<std::string> tokens;
    std::istringstream iss(content);
    std::string raw;
    while (iss >> raw) {
        auto token = normalizeToken(raw);
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

} // namespace dict

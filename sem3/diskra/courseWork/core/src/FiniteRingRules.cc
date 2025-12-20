// core/src/FiniteRingRules.cc
#include "FiniteRingRules.h"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <iomanip>

using std::string;
using std::vector;
using std::map;
using std::runtime_error;
using std::cerr;
using std::endl;

FiniteRingRules::FiniteRingRules(const string& config_file,
                                   const string& variant_name) {
    YAML::Node root;
    
    try {
        root = YAML::LoadFile(config_file);
    } catch (const YAML::Exception& e) {
        throw runtime_error("Failed to load config file '" + config_file + "': " + e.what());
    }
    
    if (!root["variants"]) {
        throw runtime_error("Config missing 'variants' section");
    }
    
    bool found = false;
    
    // * ищем вариант в иерархической структуре
    for (const auto& type_node : root["variants"]) {
        for (const auto& var_node : type_node.second) {
            if (var_node.first.as<string>() == variant_name) {
                init(var_node.second); // вызываем инициализацию
                found = true;
                break;
            }
        }
        if (found) break;
    }
    
    if (!found) {
        throw runtime_error("Variant not found: '" + variant_name + "'");
    }
}

void FiniteRingRules::init(const YAML::Node& variant_node) {
    // * 1 --- чтение базовых параметров
    if (!variant_node["size"] || !variant_node["zero_element"] || !variant_node["one_element"]) {
        throw runtime_error("Variant must contain: size, zero_element, one_element");
    }
    
    size_ = variant_node["size"].as<int>();    
    string zero_str = variant_node["zero_element"].as<string>();
    string one_str = variant_node["one_element"].as<string>();
    
    if (zero_str.empty() || one_str.empty()) {
        throw runtime_error("Zero and one elements cannot be empty");
    }
    
    zero_ = zero_str[0];
    one_ = one_str[0];
    
    if (zero_ == one_) {
        throw runtime_error("Zero and one elements must be different");
    }
    
    // * 2 --- чтение правила "плюс один"
    YAML::Node rpo = variant_node["rule_plus_one"];
    
    if (!rpo || !rpo.IsSequence()) {
        throw runtime_error("rule_plus_one must be a sequence");
    }
    
    vector<string> seq_strs = rpo.as<vector<string>>();
    // vector<char> seq_strs = rpo.as<vector<char>>();

    if ((int)seq_strs.size() != size_) {
        throw runtime_error("rule_plus_one size must match ring size");
    }
    
    // * 3 --- формирование последовательности
    vector<char> seq;
    seq.reserve(size_);
    
    for (const auto& s : seq_strs) {
        if (s.empty()) {
            throw runtime_error("Empty string in rule_plus_one");
        }
        seq.push_back(s[0]); // берем первый символ (char)
        // if (!s){
        //     throw runtime_error("Invalid character in rule_plus_one");
        // }
        // seq.push_back(s);
    }
    
    // * 4 --- ищем zero_el в послед
    int zero_position = -1;
    for (int i = 0; i < size_; ++i) {
        if (seq[i] == zero_) {
            zero_position = i;
            break;
        }
    }
    if (zero_position == -1) {
        throw runtime_error("Zero element not found in rule_plus_one sequence");
    }

    // * 5 --- послед начиная с нуля (задаем порядок X -> X + 1)
    
    // итоговоые контейнеры класса 
    values_.clear();        // контейнер значений
    char_to_val_.clear();   // мапа char -> int
    
    // * устанавливаем 0 и 1
    values_.push_back(zero_);
    char_to_val_[zero_] = 0;

    values_.push_back(one_);
    char_to_val_[one_] = 1;

    // * заполняем остальные элементы
    int current_index = 2; // начинаем с индекса 2
    for (char c : seq) {
        // * берем элементы из исходной послед.
        if (c != zero_ && c != one_) {
            values_.push_back(c);
            char_to_val_[c] = current_index;
            current_index++;
        }
    }
}

#ifdef DEBUG
    cerr << "\n--- Ring initialized ---" << endl;
    cerr << "Size: " << size_ << endl;
    cerr << "Zero: " << zero_ << " (index 0)" << endl;
    cerr << "One:  " << one_ << " (index 1)" << endl;
    cerr << "Order: ";
    for (int i = 0; i < size_; ++i) {
        cerr << values_[i];
        if (i < size_ - 1) cerr << " -> ";
    }
    cerr << " -> " << values_[0] << " (cycle)" << endl;
    cerr << "---------------------=\n" << endl;
#endif

const std::vector<char>& FiniteRingRules::getOrderedValues() const {
    return values_; 
}

int FiniteRingRules::getCharValue(char c) const {
    auto it = char_to_val_.find(c);
    
    if (it == char_to_val_.end()) {
        throw runtime_error("Invalid character: '" + string(1, c) + 
                          "' (not in ring)");
    }
    
    return it->second;
}

bool FiniteRingRules::isValidChar(char c) const {
    return char_to_val_.find(c) != char_to_val_.end();
}

void FiniteRingRules::printRules() const {
    std::cout << "\n==========================================" << std::endl;
    std::cout << "          АРИФМЕТИЧЕСКИЕ ПРАВИЛА           " << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Размер кольца (N): Z" << size_ << std::endl;
    std::cout << "Нулевой элемент (0): '" << zero_ << "'" << std::endl;
    std::cout << "Единичный элемент (1): '" << one_ << "'" << std::endl;
    
    // Вывод порядка символов и их индексов
    std::cout << "\n--- Таблица Символ -> Индекс (Отношение порядка a < b < ...) ---" << std::endl;
    
    std::cout << "Индекс (Значение): ";
    for (int i = 0; i < size_; ++i) {
        std::cout << std::setw(3) << i;
    }
    std::cout << std::endl;

    std::cout << "Символ:            ";
    for (int i = 0; i < size_; ++i) {
        char c = getValueChar(i); 
        std::cout << std::setw(3) << c;
    }
    std::cout << std::endl;

    // // Дополнительная проверка на правило +1
    // std::cout << "\n--- Проверка Правила '+1' (X + 1) ---" << std::endl;
    // std::cout << "X  | X + 1" << std::endl;
    // std::cout << "---|------" << std::endl;
    // for (int i = 0; i < size_; ++i) {
    //     char current_char = getValueChar(i);
    //     // Следующий элемент в цикле (использует индексы, как в SmallRingArithmetic)
    //     char next_char = getValueChar((i + 1) % size_); 
    //     std::cout << current_char << "  | " << next_char << std::endl;
    // }
    std::cout << "==========================================" << std::endl;
}

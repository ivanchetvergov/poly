#include "Runner.h"
#include "HashTable.h"
#include "RBTree.h"
#include "TextGenerator.h"
#include "Tokenizer.h"
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <sstream>
#include <filesystem>

#include "../../common/include/Visualizer.h"
#include "../../common/include/FileHandler.h"
#include "../../common/include/DrawDataConfig.h"

using graph::FileHandler;
using graph::DrawDataConfig;

namespace dict {

namespace lab6 {

void Runner::runHashTableDemo() {
    std::cout << "=== HashTable Demo ===\n";
    HashTable ht(18);

    std::vector<std::string> words = {
        "C++", "Python", "Java", "Haskell", "DisMath",
        "TGraph", "DB", "SQL", "ML", "RecSys", "NLP"
    };
    for (auto const& word : words) {
        ht.insert(word);
    }

    std::string txtFile = "../../assets/txt/hashtable_demo.txt";
    std::string pngFile = "../../assets/png/hashtable_demo.png";

    std::string content = ht.serialize();
    auto data = DrawDataConfig::getConfigs().at(61);
    if (FileHandler::saveHashTableData(content, data.txtFile)) {
        std::cout << "Exported to " << data.txtFile << "\n";
        graph::Visualizer::drawHashTable(data);
        std::cout << "Visualization saved to " << data.pngFile << "\n";
    } else {
        std::cout << "Export failed\n";
    }
}

void Runner::runRBTreeDemo() {
    std::cout << "=== RBTree Demo ===\n";
    RBTree tree;

    // std::vector<std::string> words = {"Hesse", "Гете", "Hugo", "Достоевский", "Kafka", "Tolstoy", "Camus", "Пушкин",
    //     "Orwell", "Чехов", "Balzac", "Гессе", "Dostoevsky", "Flaubert", "Булгаков", "Goethe", "Proust", "Шолохов", "Camus"};
    std::vector<std::string> words = {
        "C++", "Python", "Java", "Go", "Rust", "Haskell",
        "DisMath", "TGraph", "Algo", "EvMath", "DB", "SQL",
        "ML", "RecSys", "NLP", "CV", "DL",
    };

    std::shuffle(words.begin(), words.end(), std::mt19937{std::random_device{}()});
    int step = 0;
    auto data = DrawDataConfig::getConfigs().at(65);
    std::string snapshotsFile = data.txtFile;
    FileHandler::saveToFile(snapshotsFile, "");

    for (auto const& word : words) {
        tree.insert(word);
        // Log snapshot
        auto nodes = tree.serialize();
        std::string snapshot = std::to_string(step) + "\n" + nodes + "\n---\n";
        FileHandler::appendToFile(snapshotsFile, snapshot);
        step++;
    }

    if (!tree.validate()) {
        std::cout << "RBTree is invalid!" << std::endl;
    }

    std::string txtFile = "../../assets/txt/rbtree_demo.txt";
    std::string pngFile = "../../assets/png/rbtree_demo.png";
    auto nodes = tree.serialize();
    auto pngData = DrawDataConfig::getConfigs().at(62);
    if (FileHandler::saveRBTreeData(nodes, pngData.txtFile)) {
        std::cout << "Exported to " << pngData.txtFile << "\n";
        graph::Visualizer::drawRBTree(pngData, graph::VisualizationType::Graph);
        std::cout << "Visualization saved to " << pngData.pngFile << "\n";
    } else {
        std::cout << "Export failed\n";
    }

    graph::Visualizer::drawRBTree(data, graph::VisualizationType::Animation);
    std::cout << "Animation saved to " << data.gifFile << "\n";
}

void Runner::runHashTableInteractive() {
    std::cout << "=== HashTable Interactive Mode ===\n";
    HashTable ht(20);
    std::string command;
    while (true) {
        std::cout << "Enter command (insert <word>, remove <word>, search <word>, draw, exit): ";
        std::getline(std::cin >> std::ws, command);
        if (command == "exit") break;
        if (command == "clear") {
            ht.clear();
            std::cout << "Cleared table\n";
            continue;
        }
        if (command.size() >= 4 && command.substr(0, 4) == "load") {
            std::istringstream lss(command);
            std::string cmd, filename;
            lss >> cmd >> filename;
            if (filename.empty()) {
                std::cout << "Usage: load <filename>\n";
            } else if (ht.loadFromFile(filename, true)) {
                std::cout << "Loaded from: " << filename << "\n";
            } else {
                std::cout << "Failed to load: " << filename << "\n";
            }
            continue;
        }
        if (command.substr(0, 6) == "insert") {
            std::string word = command.substr(7);
            ht.insert(word);
            std::cout << "Inserted: " << word << "\n";
        } else if (command.substr(0, 6) == "remove") {
            std::string word = command.substr(7);
            if (ht.remove(word)) {
                std::cout << "Removed: " << word << "\n";
            } else {
                std::cout << "Not found: " << word << "\n";
            }
        } else if (command.substr(0, 6) == "search") {
            std::string word = command.substr(7);
            if (ht.search(word)) {
                std::cout << "Found: " << word << "\n";
            } else {
                std::cout << "Not found: " << word << "\n";
            }
        } else if (command == "draw") {
            std::string content = ht.serialize();
            auto data = DrawDataConfig::getConfigs().at(63);
            if (FileHandler::saveHashTableData(content, data.txtFile)) {
                std::cout << "Exported to " << data.txtFile << "\n";
                graph::Visualizer::drawHashTable(data);
                std::cout << "Visualization saved to " << data.pngFile << "\n";
            } else {
                std::cout << "Export failed\n";
            }
        } else {
            std::cout << "Unknown command\n";
        }
    }
}

void Runner::runRBTreeInteractive() {
    std::cout << "=== RBTree Interactive Mode ===\n";
    RBTree tree;
    std::string command;
    int step = 1;
    auto data = DrawDataConfig::getConfigs().at(65);
    std::string snapshotsFile = data.txtFile;
    FileHandler::saveToFile(snapshotsFile, "");
    while (true) {
        std::cout << "Enter command (insert <word>, remove <word>, search <word>, validate, draw, gif, exit): ";
        std::getline(std::cin >> std::ws, command);
        if (command == "exit") break;
        if (command == "clear") {
            tree.clear();
            std::cout << "Cleared tree\n";
            step = 1;
            FileHandler::saveToFile(snapshotsFile, "");
            auto data = tree.serialize();
            std::string snapshot = std::to_string(step) + "\n" + data + "\n---\n";
            FileHandler::appendToFile(snapshotsFile, snapshot);
            step++;
            continue;
        }
        if (command.size() >= 4 && command.substr(0, 4) == "load") {
            std::istringstream lss(command);
            std::string cmd, filename;
            lss >> cmd >> filename;
            if (filename.empty()) {
                std::cout << "Usage: load <filename>\n";
            } else {
                std::vector<std::string> tokens;
                if (!Tokenizer::tokensFromFile(filename, tokens)) {
                    std::cout << "Failed to load: " << filename << "\n";
                    continue;
                }

                for (auto const& token : tokens) {
                    tree.insert(token);
                    auto data = tree.serialize();
                    std::string snapshot = std::to_string(step) + "\n" + data + "\n---\n";
                    FileHandler::appendToFile(snapshotsFile, snapshot);
                    step++;
                }
                std::cout << "Loaded from: " << filename << " (tokens=" << tokens.size() << ")\n";
            }
            continue;
        }
        if (command.substr(0, 6) == "insert") {
            std::string word = command.substr(7);
            tree.insert(word);
            std::cout << "Inserted: " << word << "\n";

            // Validate tree after operation
            if (!tree.validate()) {
                std::cout << "[WARNING] Tree is invalid after insert!\n";
            }

            auto data = tree.serialize();
            std::string snapshot = std::to_string(step) + "\n" + data + "\n---\n";
            FileHandler::appendToFile(snapshotsFile, snapshot);
            step++;
        } else if (command.substr(0, 6) == "remove") {
            std::string word = command.substr(7);
            if (tree.remove(word)) {
                std::cout << "Removed: " << word << "\n";

                // Validate tree after operation
                if (!tree.validate()) {
                    std::cout << "[WARNING] Tree is invalid after remove!\n";
                }

                auto data = tree.serialize();
                std::string snapshot = std::to_string(step) + "\n" + data + "\n---\n";
                FileHandler::appendToFile(snapshotsFile, snapshot);
                step++;
            } else {
                std::cout << "Not found: " << word << "\n";
            }
        } else if (command.substr(0, 6) == "search") {
            std::string word = command.substr(7);
            if (tree.search(word)) {
                std::cout << "Found: " << word << "\n";
            } else {
                std::cout << "Not found: " << word << "\n";
            }
        } else if (command == "validate") {
            if (tree.validate()) {
                std::cout << "Tree is valid\n";
            } else {
                std::cout << "Tree is invalid\n";
            }
        } else if (command == "draw") {
            auto nodes = tree.serialize();
            auto data = DrawDataConfig::getConfigs().at(64);
            if (FileHandler::saveRBTreeData(nodes, data.txtFile)) {
                std::cout << "Exported to " << data.txtFile << "\n";
                graph::Visualizer::drawRBTree(data, graph::VisualizationType::Graph);
                std::cout << "Visualization saved to " << data.pngFile << "\n";
            } else {
                std::cout << "Export failed\n";
            }
        } else if (command == "gif") {
            if (step > 1) {
                auto data = DrawDataConfig::getConfigs().at(65);
                // Remove previous gif so stale animation is never shown if render fails.
                std::error_code ec;
                std::filesystem::remove(data.gifFile, ec);
                graph::Visualizer::drawRBTree(data, graph::VisualizationType::Animation);
                std::cout << "Animation created and saved to " << data.gifFile << "\n";
            } else {
                std::cout << "[ERROR] No snapshots to animate. Perform some insert/remove operations first.\n";
            }
        } else {
            std::cout << "Unknown command\n";
        }
    }
}

}  // namespace lab6

}  // namespace dict



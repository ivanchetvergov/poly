#include "Runner.h"
#include "HashTable.h"
#include "RBTree.h"
#include "TextGenerator.h"
#include <iostream>
#include <vector>
#include <string>

#include "../../common/include/Visualizer.h"
#include "../../common/include/FileHandler.h"

using graph::FileHandler;

namespace dict {

namespace lab6 {

void Runner::runHashTableDemo() {
    std::cout << "=== HashTable Demo ===\n";
    HashTable ht(20);

    std::vector<std::string> words = {"grape", "вишня", "cherry", "яблоко", "apple", "melon", "peach", "mango"};
    for (auto const& word : words) {
        ht.insert(word);
    }

    ht.printTable();

    std::string txtFile = "../../assets/txt/hashtable_demo.txt";
    std::string pngFile = "../../assets/png/hashtable_demo.png";

    std::string content = ht.getVisualizationData();
    if (FileHandler::saveToFile(txtFile, content)) {
        std::cout << "Exported to " << txtFile << "\n";
        graph::Visualizer::drawHashTable(txtFile, pngFile, "HashTable Demo");
        std::cout << "Visualization saved to " << pngFile << "\n";
    } else {
        std::cout << "Export failed\n";
    }
}

void Runner::runRBTreeDemo() {
    std::cout << "=== RBTree Demo ===\n";
    RBTree tree;

    std::vector<std::string> words =  {"grape", "вишня", "cherry", "яблоко", "apple", "melon", "peach", "mango"};
    for (auto const& word : words) {
        tree.insert(word);
    }

    tree.printTree();

    std::string txtFile = "../../assets/txt/rbtree_demo.txt";
    std::string pngFile = "../../assets/png/rbtree_demo.png";
    auto nodes = tree.getVisualizationData();
    if (FileHandler::saveTreeStructure(txtFile, nodes)) {
        std::cout << "Exported to " << txtFile << "\n";
        graph::Visualizer::drawRBTree(txtFile, pngFile, "RBTree Demo");
        std::cout << "Visualization saved to " << pngFile << "\n";
    } else {
        std::cout << "Export failed\n";
    }
}

void Runner::runTextGeneratorDemo() {
    std::cout << "=== TextGenerator Demo ===\n";
    TextGenerator gen;
    std::string text = gen.generate(100);
    std::cout << "Generated text: " << text.substr(0, 50) << "...\n";

    std::string file = "../../assets/txt/generated_text.txt";
    std::string pngFile = "../../assets/png/text_stats.png";
    if (FileHandler::saveToFile(file, text)) {
        std::cout << "Saved to " << file << "\n";
        graph::Visualizer::drawTextStats(file, pngFile, "Text Statistics");
        std::cout << "Visualization saved to " << pngFile << "\n";
    } else {
        std::cout << "Save failed\n";
    }
}

}  // namespace lab6

}  // namespace dict



#include "Runner.h"
#include "HashTable.h"
#include "RBTree.h"
#include "TextGenerator.h"
#include <iostream>
#include <vector>
#include <string>
#include <subprocess>

namespace dict {

namespace lab6 {

void Runner::runHashTableDemo() {
    std::cout << "=== HashTable Demo ===\n";
    HashTable ht(20);  // Small capacity for demo

    std::vector<std::string> words = {"apple", "banana", "cherry", "apple", "date", "elderberry"};
    for (auto const& word : words) {
        ht.insert(word);
    }

    ht.printTable();

    std::string txtFile = "assets/txt/hashtable_demo.txt";
    std::string pngFile = "assets/png/hashtable_demo.png";
    if (ht.exportForVisualization(txtFile)) {
        std::cout << "Exported to " << txtFile << "\n";

        std::string cmd = "/Users/ivan/myvenv/bin/python -m scripts.visualization.plot_hashtable " + txtFile + " " + pngFile + " \"HashTable Demo\"";
        int result = system(cmd.c_str());
        if (result == 0) {
            std::cout << "Visualization saved to " << pngFile << "\n";
        } else {
            std::cout << "Error in visualization\n";
        }
    } else {
        std::cout << "Export failed\n";
    }
}

void Runner::runRBTreeDemo() {
    std::cout << "=== RBTree Demo ===\n";
    RBTree tree;

    std::vector<std::string> words = {"apple", "banana", "cherry", "apple", "date", "elderberry"};
    for (auto const& word : words) {
        tree.insert(word);
    }

    tree.printTree();

    // Export for visualization
    std::string txtFile = "assets/txt/rbtree_demo.txt";
    std::string pngFile = "assets/png/rbtree_demo.png";
    if (tree.exportForVisualization(txtFile)) {
        std::cout << "Exported to " << txtFile << "\n";

        // Call Python script
        std::string cmd = "/Users/ivan/myvenv/bin/python -m scripts.visualization.plot_rbtree " + txtFile + " " + pngFile + " \"RBTree Demo\"";
        int result = system(cmd.c_str());
        if (result == 0) {
            std::cout << "Visualization saved to " << pngFile << "\n";
        } else {
            std::cout << "Error in visualization\n";
        }
    } else {
        std::cout << "Export failed\n";
    }
}

void Runner::runTextGeneratorDemo() {
    std::cout << "=== TextGenerator Demo ===\n";
    TextGenerator gen;
    std::string text = gen.generate(100);
    std::cout << "Generated text: " << text.substr(0, 50) << "...\n";

    std::string file = "assets/txt/generated_text.txt";
    if (gen.saveToFile(file, 100)) {
        std::cout << "Saved to " << file << "\n";
    } else {
        std::cout << "Save failed\n";
    }
}

}  // namespace lab6

}  // namespace dict



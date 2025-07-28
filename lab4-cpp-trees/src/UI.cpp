#include "AVLTree.h"
#include "BST.h"
#include <map>
#include <string>
#include <iostream>
#include <functional>
#include <sstream>
#include <limits>

enum class TreeType { AVL_INT, AVL_DOUBLE, AVL_STRING, BST_INT, BST_DOUBLE, BST_STRING };

struct AnyTree {
    TreeType type;
    void* tree;
};

std::map<std::string, AnyTree> trees;
std::string currentTreeName;

void printMenu() {
    std::cout << "\n--- Tree Console Menu ---\n";
    std::cout << "1. Create new tree (AVL or BST)\n";
    std::cout << "2. Switch current tree\n";
    std::cout << "3. Append value\n";
    std::cout << "4. Erase value\n";
    std::cout << "5. Search value\n";
    std::cout << "6. Map (x -> x * 2 or x + x for strings)\n";
    std::cout << "7. Where (x >= 0 or even-length strings)\n";
    std::cout << "8. Reduce (sum)\n";
    std::cout << "9. Print tree\n";
    std::cout << "10. Pre-order traversal\n";
    std::cout << "11. In-order traversal\n";
    std::cout << "12. Post-order traversal\n";
    std::cout << "13. Concatenate with another tree\n";
    std::cout << "0. Exit\n";
    std::cout << "Choose option: ";
}

template<typename T>
void appendValue(void* tree, TreeType type) {
    T value;
    std::cout << "Enter value: ";
    std::cin >> value;
    if (type == TreeType::AVL_INT || type == TreeType::AVL_DOUBLE || type == TreeType::AVL_STRING) {
        static_cast<AVLTree<T>*>(tree)->append(value);
    } else {
        static_cast<BST<T>*>(tree)->append(value);
    }
    std::cout << "Value added.\n";
}

template<typename T>
void eraseValue(void* tree, TreeType type) {
    T value;
    std::cout << "Enter value to erase: ";
    std::cin >> value;
    try {
        if (type == TreeType::AVL_INT || type == TreeType::AVL_DOUBLE || type == TreeType::AVL_STRING) {
            static_cast<AVLTree<T>*>(tree)->erase(value);
        } else {
            static_cast<BST<T>*>(tree)->erase(value);
        }
        std::cout << "Value erased.\n";
    } catch (...) {
        std::cout << "Value not found.\n";
    }
}

template<typename T>
void searchValue(void* tree, TreeType type) {
    T value;
    std::cout << "Enter value to search: ";
    std::cin >> value;
    bool found;
    if (type == TreeType::AVL_INT || type == TreeType::AVL_DOUBLE || type == TreeType::AVL_STRING) {
        found = static_cast<AVLTree<T>*>(tree)->search(value);
    } else {
        found = static_cast<BST<T>*>(tree)->search(value);
    }
    std::cout << (found ? "Found.\n" : "Not found.\n");
}

template<typename T>
void mapTree(void*& tree, TreeType type) {
    if (type == TreeType::AVL_INT || type == TreeType::AVL_DOUBLE || type == TreeType::AVL_STRING) {
        AVLTree<T>* newTree = static_cast<AVLTree<T>*>(tree)->map([](const T& x) { return x * 2; });
        delete static_cast<AVLTree<T>*>(tree);
        tree = newTree;
    } else {
        BST<T>* newTree = static_cast<BST<T>*>(tree)->map([](const T& x) { return x * 2; });
        delete static_cast<BST<T>*>(tree);
        tree = newTree;
    }
    std::cout << "Mapped tree (x -> x * 2).\n";
}

template<>
void mapTree<std::string>(void*& tree, TreeType type) {
    if (type == TreeType::AVL_STRING) {
        auto newTree = static_cast<AVLTree<std::string>*>(tree)->map([](const std::string& x) { return x + x; });
        delete static_cast<AVLTree<std::string>*>(tree);
        tree = newTree;
    } else {
        auto newTree = static_cast<BST<std::string>*>(tree)->map([](const std::string& x) { return x + x; });
        delete static_cast<BST<std::string>*>(tree);
        tree = newTree;
    }
    std::cout << "Mapped tree (x -> x + x).\n";
}

template<typename T>
void whereTree(void*& tree, TreeType type) {
    if (type == TreeType::AVL_INT || type == TreeType::AVL_DOUBLE || type == TreeType::AVL_STRING) {
        auto newTree = static_cast<AVLTree<T>*>(tree)->where([](const T& x) { return x >= 0; });
        delete static_cast<AVLTree<T>*>(tree);
        tree = newTree;
    } else {
        auto newTree = static_cast<BST<T>*>(tree)->where([](const T& x) { return x >= 0; });
        delete static_cast<BST<T>*>(tree);
        tree = newTree;
    }
    std::cout << "Filtered tree (x >= 0).\n";
}

template<>
void whereTree<std::string>(void*& tree, TreeType type) {
    if (type == TreeType::AVL_STRING) {
        auto newTree = static_cast<AVLTree<std::string>*>(tree)->where([](const std::string& x) { return x.length() % 2 == 0; });
        delete static_cast<AVLTree<std::string>*>(tree);
        tree = newTree;
    } else {
        auto newTree = static_cast<BST<std::string>*>(tree)->where([](const std::string& x) { return x.length() % 2 == 0; });
        delete static_cast<BST<std::string>*>(tree);
        tree = newTree;
    }
    std::cout << "Filtered tree (even-length strings).\n";
}

template<typename T>
void reduceTree(void* tree, TreeType type) {
    T result;
    if (type == TreeType::AVL_INT || type == TreeType::AVL_DOUBLE || type == TreeType::AVL_STRING) {
        result = static_cast<AVLTree<T>*>(tree)->reduce([](const T& a, const T& b) { return a + b; }, T());
    } else {
        result = static_cast<BST<T>*>(tree)->reduce([](const T& a, const T& b) { return a + b; }, T());
    }
    std::cout << "Reduce result: " << result << "\n";
}

template<typename T>
void printPreOrder(void* tree, TreeType type) {
    DynamicArray<T>* arr;
    if (type == TreeType::AVL_INT || type == TreeType::AVL_DOUBLE || type == TreeType::AVL_STRING) {
        arr = static_cast<AVLTree<T>*>(tree)->preOderTraversal();
    } else {
        arr = static_cast<BST<T>*>(tree)->preOderTraversal();
    }
    for (size_t i = 0; i < arr->GetSize(); ++i)
        std::cout << arr->Get(i) << " ";
    std::cout << "\n";
    delete arr;
}

template<typename T>
void printInOrder(void* tree, TreeType type) {
    DynamicArray<T>* arr;
    if (type == TreeType::AVL_INT || type == TreeType::AVL_DOUBLE || type == TreeType::AVL_STRING) {
        arr = static_cast<AVLTree<T>*>(tree)->inOderTraversal();
    } else {
        arr = static_cast<BST<T>*>(tree)->inOderTraversal();
    }
    for (size_t i = 0; i < arr->GetSize(); ++i)
        std::cout << arr->Get(i) << " ";
    std::cout << "\n";
    delete arr;
}

template<typename T>
void printPostOrder(void* tree, TreeType type) {
    DynamicArray<T>* arr;
    if (type == TreeType::AVL_INT || type == TreeType::AVL_DOUBLE || type == TreeType::AVL_STRING) {
        arr = static_cast<AVLTree<T>*>(tree)->postOderTraversal();
    } else {
        arr = static_cast<BST<T>*>(tree)->postOderTraversal();
    }
    for (size_t i = 0; i < arr->GetSize(); ++i)
        std::cout << arr->Get(i) << " ";
    std::cout << "\n";
    delete arr;
}

template<typename T>
void concatTrees(void* currentTree, TreeType type) {
    std::string otherTreeName;
    std::cout << "Available trees to concatenate from: ";
    bool first = true;
    bool other_trees_exist = false;
    for (const auto& pair : trees) {
        if (pair.first != currentTreeName) {
            if (trees[currentTreeName].type == pair.second.type) { 
                if (!first) std::cout << ", ";
                std::cout << pair.first;
                first = false;
                other_trees_exist = true;
            }
        }
    }
    if (!other_trees_exist) {
        std::cout << "None of the same type available besides current.\n";
        return;
    }
    std::cout << "\n";

    std::cout << "Enter name of the tree to concatenate elements FROM (into current tree '" << currentTreeName << "'): ";
    std::cin >> otherTreeName;

    if (trees.find(otherTreeName) == trees.end()) {
        std::cout << "Tree '" << otherTreeName << "' not found.\n";
        return;
    }
    if (otherTreeName == currentTreeName) {
        std::cout << "Cannot concatenate a tree with itself using this option.\n";
        return;
    }

    AnyTree& otherAnyTree = trees[otherTreeName];
    AnyTree& currentAnyTreeRef = trees[currentTreeName];

    if (otherAnyTree.type != currentAnyTreeRef.type) {
        std::cout << "Cannot concatenate trees of different types.\n";
        return;
    }

    if (type == TreeType::AVL_INT || type == TreeType::AVL_DOUBLE || type == TreeType::AVL_STRING) {
        auto* other_tree_casted = static_cast<AVLTree<T>*>(otherAnyTree.tree);
        static_cast<AVLTree<T>*>(currentTree)->concat(*other_tree_casted); 
    } else {
        auto* other_tree_casted = static_cast<BST<T>*>(otherAnyTree.tree);
        static_cast<BST<T>*>(currentTree)->concat(*other_tree_casted); 
    }
    std::cout << "Concatenated elements from '" << otherTreeName << "' into '" << currentTreeName << "'.\n";
}

void handleCurrentTree(int option) {
    if (currentTreeName.empty() || trees.find(currentTreeName) == trees.end()) {
        std::cout << "No active tree selected.\n";
        return;
    }

    auto& anyTree = trees[currentTreeName];

    switch (anyTree.type) {
        case TreeType::AVL_INT: {
            auto* tree = static_cast<AVLTree<int>*>(anyTree.tree);
            if (option == 3) appendValue<int>(tree, anyTree.type);
            else if (option == 4) eraseValue<int>(tree, anyTree.type);
            else if (option == 5) searchValue<int>(tree, anyTree.type);
            else if (option == 6) {
                mapTree<int>(anyTree.tree, anyTree.type);
            }
            else if (option == 7) {
                whereTree<int>(anyTree.tree, anyTree.type);
            }
            else if (option == 8) reduceTree<int>(tree, anyTree.type);
            else if (option == 9) tree->printTree();
            else if (option == 10) printPreOrder<int>(tree, anyTree.type);
            else if (option == 11) printInOrder<int>(tree, anyTree.type);
            else if (option == 12) printPostOrder<int>(tree, anyTree.type);
            else if (option == 13) concatTrees<int>(tree, anyTree.type);
            break;
        }
        case TreeType::AVL_DOUBLE: {
            auto* tree = static_cast<AVLTree<double>*>(anyTree.tree);
            if (option == 3) appendValue<double>(tree, anyTree.type);
            else if (option == 4) eraseValue<double>(tree, anyTree.type);
            else if (option == 5) searchValue<double>(tree, anyTree.type);
            else if (option == 6) {
                mapTree<double>(anyTree.tree, anyTree.type);
            }
            else if (option == 7) {
                whereTree<double>(anyTree.tree, anyTree.type);
            }
            else if (option == 8) reduceTree<double>(tree, anyTree.type);
            else if (option == 9) tree->printTree();
            else if (option == 10) printPreOrder<double>(tree, anyTree.type);
            else if (option == 11) printInOrder<double>(tree, anyTree.type);
            else if (option == 12) printPostOrder<double>(tree, anyTree.type);
            else if (option == 13) concatTrees<double>(tree, anyTree.type);
            break;
        }
        case TreeType::AVL_STRING: {
            auto* tree = static_cast<AVLTree<std::string>*>(anyTree.tree);
            if (option == 3) appendValue<std::string>(tree, anyTree.type);
            else if (option == 4) eraseValue<std::string>(tree, anyTree.type);
            else if (option == 5) searchValue<std::string>(tree, anyTree.type);
            else if (option == 6) {
                mapTree<std::string>(anyTree.tree, anyTree.type);
            }
            else if (option == 7) {
                whereTree<std::string>(anyTree.tree, anyTree.type);
            }
            else if (option == 8) reduceTree<std::string>(tree, anyTree.type);
            else if (option == 9) tree->printTree();
            else if (option == 10) printPreOrder<std::string>(tree, anyTree.type);
            else if (option == 11) printInOrder<std::string>(tree, anyTree.type);
            else if (option == 12) printPostOrder<std::string>(tree, anyTree.type);
            else if (option == 13) concatTrees<std::string>(tree, anyTree.type);
            break;
        }
        case TreeType::BST_INT: {
            auto* tree = static_cast<BST<int>*>(anyTree.tree);
            if (option == 3) appendValue<int>(tree, anyTree.type);
            else if (option == 4) eraseValue<int>(tree, anyTree.type);
            else if (option == 5) searchValue<int>(tree, anyTree.type);
            else if (option == 6) {
                mapTree<int>(anyTree.tree, anyTree.type);
            }
            else if (option == 7) {
                whereTree<int>(anyTree.tree, anyTree.type);
            }
            else if (option == 8) reduceTree<int>(tree, anyTree.type);
            else if (option == 9) tree->printTree();
            else if (option == 10) printPreOrder<int>(tree, anyTree.type);
            else if (option == 11) printInOrder<int>(tree, anyTree.type);
            else if (option == 12) printPostOrder<int>(tree, anyTree.type);
            else if (option == 13) concatTrees<int>(tree, anyTree.type);
            break;
        }
        case TreeType::BST_DOUBLE: {
            auto* tree = static_cast<BST<double>*>(anyTree.tree);
            if (option == 3) appendValue<double>(tree, anyTree.type);
            else if (option == 4) eraseValue<double>(tree, anyTree.type);
            else if (option == 5) searchValue<double>(tree, anyTree.type);
            else if (option == 6) {
                mapTree<double>(anyTree.tree, anyTree.type);
            }
            else if (option == 7) {
                whereTree<double>(anyTree.tree, anyTree.type);
            }
            else if (option == 8) reduceTree<double>(tree, anyTree.type);
            else if (option == 9) tree->printTree();
            else if (option == 10) printPreOrder<double>(tree, anyTree.type);
            else if (option == 11) printInOrder<double>(tree, anyTree.type);
            else if (option == 12) printPostOrder<double>(tree, anyTree.type);
            else if (option == 13) concatTrees<double>(tree, anyTree.type);
            break;
        }
        case TreeType::BST_STRING: {
            auto* tree = static_cast<BST<std::string>*>(anyTree.tree);
            if (option == 3) appendValue<std::string>(tree, anyTree.type);
            else if (option == 4) eraseValue<std::string>(tree, anyTree.type);
            else if (option == 5) searchValue<std::string>(tree, anyTree.type);
            else if (option == 6) {
                mapTree<std::string>(anyTree.tree, anyTree.type);
            }
            else if (option == 7) {
                whereTree<std::string>(anyTree.tree, anyTree.type);
            }
            else if (option == 8) reduceTree<std::string>(tree, anyTree.type);
            else if (option == 9) tree->printTree();
            else if (option == 10) printPreOrder<std::string>(tree, anyTree.type);
            else if (option == 11) printInOrder<std::string>(tree, anyTree.type);
            else if (option == 12) printPostOrder<std::string>(tree, anyTree.type);
            else if (option == 13) concatTrees<std::string>(tree, anyTree.type);
            break;
        }
    }
}

void createTree() {
    std::string name;
    int type;
    int treeStructure;
    std::cout << "Enter tree name: ";
    std::cin >> name;
    std::cout << "Choose tree structure (1 = AVL, 2 = BST): ";
    std::cin >> treeStructure;
    std::cout << "Choose type (1 = int, 2 = double, 3 = string): ";
    std::cin >> type;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter space-separated initial values (or press enter to skip): ";
    std::string data;
    std::getline(std::cin, data);
    std::istringstream iss(data);

    if (treeStructure == 1) { // AVL Tree
        if (type == 1) {
            std::vector<int> values;
            int val;
            while (iss >> val) {
                values.push_back(val);
            }
            trees[name] = {
                TreeType::AVL_INT,
                values.empty()
                    ? static_cast<void*>(new AVLTree<int>())
                    : static_cast<void*>(new AVLTree<int>(values.data(), values.size()))
            };
        }
        else if (type == 2) {
            std::vector<double> values;
            double val;
            while (iss >> val) {
                values.push_back(val);
            }
            trees[name] = {
                TreeType::AVL_DOUBLE,
                values.empty()
                    ? static_cast<void*>(new AVLTree<double>())
                    : static_cast<void*>(new AVLTree<double>(values.data(), values.size()))
            };
        }
        else if (type == 3) {
            std::vector<std::string> values;
            std::string val;
            while (iss >> val) {
                values.push_back(val);
            }
            if (values.empty()) {
                trees[name] = { TreeType::AVL_STRING, new AVLTree<std::string>() };
            } else {
                DynamicArray<std::string> arr(0);
                for (const auto& s : values) {
                    arr.Append(s);
                }
                trees[name] = {
                    TreeType::AVL_STRING,
                    new AVLTree<std::string>(arr.GetData(), arr.GetSize())
                };
            }
        }
        else {
            std::cout << "Invalid type.\n";
            return;
        }
    } else if (treeStructure == 2) { // BST
        if (type == 1) {
            std::vector<int> values;
            int val;
            while (iss >> val) {
                values.push_back(val);
            }
            trees[name] = {
                TreeType::BST_INT,
                values.empty()
                    ? static_cast<void*>(new BST<int>())
                    : static_cast<void*>(new BST<int>(values.data(), values.size()))
            };
        }
        else if (type == 2) {
            std::vector<double> values;
            double val;
            while (iss >> val) {
                values.push_back(val);
            }
            trees[name] = {
                TreeType::BST_DOUBLE,
                values.empty()
                    ? static_cast<void*>(new BST<double>())
                    : static_cast<void*>(new BST<double>(values.data(), values.size()))
            };
        }
        else if (type == 3) {
            std::vector<std::string> values;
            std::string val;
            while (iss >> val) {
                values.push_back(val);
            }
            if (values.empty()) {
                trees[name] = { TreeType::BST_STRING, new BST<std::string>() };
            } else {
                DynamicArray<std::string> arr(0);
                for (const auto& s : values) {
                    arr.Append(s);
                }
                trees[name] = {
                    TreeType::BST_STRING,
                    new BST<std::string>(arr.GetData(), arr.GetSize())
                };
            }
        }
        else {
            std::cout << "Invalid type.\n";
            return;
        }
    } else {
        std::cout << "Invalid tree structure.\n";
        return;
    }

    currentTreeName = name;
    std::cout << "Tree '" << name << "' created and selected.\n";
}

void switchTree() {
    std::string name;
    std::cout << "Enter tree name to switch to: ";
    std::cin >> name;
    if (trees.find(name) != trees.end()) {
        currentTreeName = name;
        std::cout << "Switched to tree '" << name << "'.\n";
    } else {
        std::cout << "Tree not found.\n";
    }
}

void runUI() {
    while (true) {
        printMenu();
        int option;
        std::cin >> option;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (option == 0) break;
        if (option == 1) createTree();
        else if (option == 2) switchTree();
        else handleCurrentTree(option);
    }

    for (auto& [_, t] : trees) {
        switch (t.type) {
            case TreeType::AVL_INT:
                delete static_cast<AVLTree<int>*>(t.tree);
                break;
            case TreeType::AVL_DOUBLE:
                delete static_cast<AVLTree<double>*>(t.tree);
                break;
            case TreeType::AVL_STRING:
                delete static_cast<AVLTree<std::string>*>(t.tree);
                break;
            case TreeType::BST_INT:
                delete static_cast<BST<int>*>(t.tree);
                break;
            case TreeType::BST_DOUBLE:
                delete static_cast<BST<double>*>(t.tree);
                break;
            case TreeType::BST_STRING:
                delete static_cast<BST<std::string>*>(t.tree);
                break;
        }
    }
}
#pragma once

#include <iostream>
#include <string.h>
#include <functional>
#include <vector>
#include "Sequence\DynamicArray.h"
#include "Sequence\CustomErrors.h"

template <typename T>
class AVLTree {
private:
    struct Node {
        Node* left;
        Node* right;
        T value;
        int height;

        Node(const T& value): value(value), left(nullptr), right(nullptr), height(0) {}
    };

    Node* root;
    size_t count;

    int getHeight(Node* node);
    void updateHeight(Node* node);
    int getBalance(Node* node);
    Node* rightRotate(Node* node);
    Node* leftRotate(Node* node);
    Node* balance(Node* node);
    Node* findMin(Node* node);
    Node* insert(Node* node, const T& value);
    Node* removeMin(Node* node);
    Node* remove(Node* node, T value, bool& deleted);
    void preOrder(Node* node, DynamicArray<T>* items);
    void preOrder(Node* node, AVLTree<T>* tree);
    void inOrder(Node* node, DynamicArray<T>* items);
    void postOrder(Node* node, DynamicArray<T>* items);
    void KPL(Node* node, DynamicArray<T>* items);
    void PLK(Node* node, DynamicArray<T>* items);
    void PKL(Node* node, DynamicArray<T>* items);
    void printHelper(Node* node, const std::string& pref, bool flag) const;
    void mapPreOrder(Node* node, std::function<T(const T&)> f, AVLTree<T>* new_tree);
    void wherePreOrder(Node* node, std::function<bool(const T&)> f, AVLTree<T>* new_tree);
    void reducePreOrder(Node* node, std::function<T(const T&, const T&)> f, T& result);
    bool searchNode(Node* node, const T& value) const;
    void clear(Node* node);

public:
    AVLTree();
    AVLTree(T* data, size_t size);
    AVLTree(AVLTree<T>* other);
    size_t length();
    size_t height();
    void erase(const T& value);
    void append(const T& value);
    bool search(const T& value) const;
    void concat(AVLTree<T>& other);
    DynamicArray<T>* preOderTraversal();
    DynamicArray<T>* inOderTraversal();
    DynamicArray<T>* postOderTraversal();
    DynamicArray<T>* traversal(std::string flag);
    AVLTree<T>* map(std::function<T(const T&)> f);
    AVLTree<T>* where(std::function<bool(const T&)> f);
    T reduce(std::function<T(const T&, const T&)> f, T initial);
    void printTree() const;
    ~AVLTree();
};

template <typename T>
AVLTree<T>::AVLTree(): root(nullptr), count(0) {}

template <typename T>
AVLTree<T>::AVLTree(T* data, size_t size): root(nullptr), count(0) {
    for (size_t i = 0; i < size; ++i) {
        append(data[i]);
    }
}

template <typename T>
AVLTree<T>::AVLTree(AVLTree<T>* other): root(nullptr), count(0) {
    preOrder(other->root, this);
}

template <typename T>
int AVLTree<T>::getHeight(Node* node) {
    return node == nullptr ? -1 : node->height;
}

template <typename T>
void AVLTree<T>::updateHeight(Node* node) {
    node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
}

template <typename T>
int AVLTree<T>::getBalance(Node* node) {
    return node == nullptr ? 0 : getHeight(node->right) - getHeight(node->left);
}

template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::rightRotate(Node* node) {
    Node* tmp = node->left;
    node->left = tmp->right;
    tmp->right = node;
    updateHeight(node);
    updateHeight(tmp);
    return tmp;
}

template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::leftRotate(Node* node) {
    Node* tmp = node->right;
    node->right = tmp->left;
    tmp->left = node;
    updateHeight(node);
    updateHeight(tmp);
    return tmp;
}

template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::balance(Node* node) {
    int balance = getBalance(node);
    if (balance == -2) {
        if (getBalance(node->left) > 0) {
            node->left = leftRotate(node->left);
        }
        return rightRotate(node);
    } else if (balance == 2) {
        if (getBalance(node->right) < 0) {
            node->right = rightRotate(node->right);
        }
        return leftRotate(node);
    }
    return node;
}

template <typename T>
size_t AVLTree<T>::length() {
    return this->count;
}

template <typename T>
size_t AVLTree<T>::height() {
    return this->height;
}

template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::insert(Node* node, const T& value) {
    if (node == nullptr) {
        count++;
        return new Node(value);
    }

    if (value < node->value)
        node->left = insert(node->left, value);
    else
        node->right = insert(node->right, value);

    updateHeight(node);
    return balance(node);
}

template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::findMin(Node* node) {
    return node->left ? findMin(node->left) : node;
}

template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::removeMin(Node* node) {
    if (node->left == nullptr) return node->right;
    node->left = removeMin(node->left);
    updateHeight(node);
    return balance(node);
}

template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::remove(Node* node, T value, bool& deleted) {
    if (node == nullptr) return nullptr;
    if (node->value < value) node->right = remove(node->right, value, deleted);
    else if (node->value > value) node->left = remove(node->left, value, deleted);
    else {
        if (node->right == nullptr) {
            count--;
            Node* tmp = node->left;
            delete node;
            deleted = true;
            return tmp;
        }
        Node* min = findMin(node->right);
        node->value = min->value;
        node->right = removeMin(node->right);
        delete min;
        deleted = true;
        count--;
        return balance(node);
    }
    updateHeight(node);
    return balance(node);
}

template <typename T>
void AVLTree<T>::erase(const T& value) {
    bool deleted = false;
    if (root == nullptr) throw EmptyContainer;
    this->root = remove(root, value, deleted);
    if (!deleted){
        throw IncorrectValue;
    }
}

template <typename T>
void AVLTree<T>::append(const T& value) {
    root = insert(root, value);
}

template <typename T>
bool AVLTree<T>::searchNode(Node* node, const T& value) const {
    if (node == nullptr) {
        return false;
    }
    if (node->value == value) {
        return true;
    }
    if (value < node->value) {
        return searchNode(node->left, value);
    }
    return searchNode(node->right, value);
}

template <typename T>
bool AVLTree<T>::search(const T& value) const {
    return searchNode(root, value);
}

template <typename T>
void AVLTree<T>::concat(AVLTree<T>& other) {
    DynamicArray<T>* data = other.preOderTraversal();
    for (int i = 0; i < data->GetSize(); ++i) {
        this->append(data->Get(i));
    }
}

template <typename T>
void AVLTree<T>::preOrder(Node* node, DynamicArray<T>* items) {
    if (node) {
        items->Append(node->value);
        preOrder(node->left, items);
        preOrder(node->right, items);
    }
}

template <typename T>
void AVLTree<T>::preOrder(Node* node, AVLTree<T>* tree) {
    if (node) {
        tree->append(node->value);
        preOrder(node->left, tree);
        preOrder(node->right, tree);
    }
}

template <typename T>
void AVLTree<T>::inOrder(Node* node, DynamicArray<T>* items) {
    if (node) {
        inOrder(node->left, items);
        items->Append(node->value);
        inOrder(node->right, items);
    }
}

template <typename T>
void AVLTree<T>::postOrder(Node* node, DynamicArray<T>* items) {
    if (node) {
        postOrder(node->left, items);
        postOrder(node->right, items);
        items->Append(node->value);
    }
}

template <typename T>
void AVLTree<T>::KPL(Node* node, DynamicArray<T>* items) {
    if (node) {
        items->Append(node->value);
        KPL(node->right, items);
        KPL(node->left, items);
    }
}

template <typename T>
void AVLTree<T>::PLK(Node* node, DynamicArray<T>* items) {
    if (node) {
        PLK(node->right, items);
        PLK(node->left, items);
        items->Append(node->value);
    }
}

template <typename T>
void AVLTree<T>::PKL(Node* node, DynamicArray<T>* items) {
    if (node) {
        PKL(node->right, items);
        items->Append(node->value);
        PKL(node->left, items);
    }
}

template <typename T>
DynamicArray<T>* AVLTree<T>::preOderTraversal() {
    DynamicArray<T>* items = new DynamicArray<T>(0);
    preOrder(root, items);
    return items;
}

template <typename T>
DynamicArray<T>* AVLTree<T>::inOderTraversal() {
    DynamicArray<T>* items = new DynamicArray<T>(0);
    inOrder(root, items);
    return items;
}

template <typename T>
DynamicArray<T>* AVLTree<T>::postOderTraversal() {
    DynamicArray<T>* items = new DynamicArray<T>(0);
    postOrder(root, items);
    return items;
}

template <typename T>
DynamicArray<T>* AVLTree<T>::traversal(std::string flag) {
    DynamicArray<T>* items = new DynamicArray<T>(0);
    if (flag == "KLP") preOrder(root, items);
    else if (flag == "LKP") inOrder(root, items);
    else if (flag == "LPK") postOrder(root, items);
    else if (flag == "KPL") KPL(root, items);
    else if (flag == "PLK") PLK(root, items);
    else if (flag == "PKL") PKL(root, items);
    return items;
}

template <typename T>
AVLTree<T>* AVLTree<T>::map(std::function<T(const T&)> f) {
    AVLTree<T>* new_tree = new AVLTree();
    mapPreOrder(this->root, f, new_tree);
    return new_tree;
}

template <typename T>
void AVLTree<T>::mapPreOrder(Node* node, std::function<T(const T&)> f, AVLTree<T>* new_tree) {
    if (node) {
        new_tree->append(f(node->value));
        mapPreOrder(node->left, f, new_tree);
        mapPreOrder(node->right, f, new_tree);
    }
}

template <typename T>
AVLTree<T>* AVLTree<T>::where(std::function<bool(const T&)> f) {
    AVLTree<T>* new_tree = new AVLTree();
    wherePreOrder(this->root, f, new_tree);
    return new_tree;
}

template <typename T>
void AVLTree<T>::wherePreOrder(Node* node, std::function<bool(const T&)> f, AVLTree<T>* new_tree) {
    if (node) {
        if (f(node->value)) new_tree->append(node->value);
        wherePreOrder(node->left, f, new_tree);
        wherePreOrder(node->right, f, new_tree);
    }
}

template <typename T>
T AVLTree<T>::reduce(std::function<T(const T&, const T&)> f, T initial) {
    T result = initial;
    reducePreOrder(root, f, result);
    return result;
}

template <typename T>
void AVLTree<T>::reducePreOrder(Node* node, std::function<T(const T&, const T&)> f, T& result) {
    if (node) {
        result = f(result, node->value);
        reducePreOrder(node->left, f, result);
        reducePreOrder(node->right, f, result);
    }
}

template <typename T>
void AVLTree<T>::printHelper(Node* node, const std::string& pref, bool flag) const {
    if (node == nullptr) {
        return;
    }

    std::cout << pref << std::endl << pref;
    std::cout << (flag ? "+---- " : "+---- ");
    std::cout << node->value << " (h:" << node->height << ")" << std::endl;

    std::string new_pref = pref + "      ";

    printHelper(node->left, new_pref, true);
    printHelper(node->right, new_pref, false);
}

template <typename T>
void AVLTree<T>::printTree() const {
    if (root == nullptr) {
        std::cout << "Tree is empty." << std::endl;
        return;
    }
    printHelper(root, "", true);
    std::cout << std::endl;
}

template <typename T>
void AVLTree<T>::clear(Node* node) {
    if (node == nullptr) return;
    clear(node->left);
    clear(node->right);
    delete node;
}

template <typename T>
AVLTree<T>::~AVLTree() {
    clear(root);
}

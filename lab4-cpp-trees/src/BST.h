#pragma once

#include <iostream>
#include <string.h>
#include <cmath>
#include <functional>
#include "Sequence\DynamicArray.h"
#include "Sequence\CustomErrors.h"

template <typename T>
class BST {
private:
    struct Node {
        Node* left;
        Node* right;
        T value;

        Node(const T& value) : value(value), left(nullptr), right(nullptr) {}
    };

    Node* root;
    size_t count;

    Node* rightRotate(Node* node);
    Node* leftRotate(Node* grandParent, int count);
    void makeGrapevine();

    bool searchNode(Node* node, const T& value) const;
    Node* findNode(Node* node, const T& value) const;

    Node* insert(Node* node, const T& value);
    Node* remove(Node* node, const T& value, bool& deleted);
    Node* findMin(Node* node);
    Node* removeMin(Node* node);
    void preOrder(Node* node, DynamicArray<T>* items);
    void preOrder(Node* node, BST<T>* tree);
    void inOrder(Node* node, DynamicArray<T>* items);
    void postOrder(Node* node, DynamicArray<T>* items);
    void KPL(Node* node, DynamicArray<T>* items);
    void PLK(Node* node, DynamicArray<T>* items);
    void PKL(Node* node, DynamicArray<T>* items);
    void printHelper(Node* node, const std::string& pref, bool flag) const;
    void mapPreOrder(Node* node, std::function<T(const T&)> f, BST<T>* new_tree);
    void wherePreOrder(Node* node, std::function<bool(const T&)> f, BST<T>* new_tree);
    void reducePreOrder(Node* node, std::function<T(const T&, const T&)> f, T& result);
    void clear(Node* node);

public:
    BST();
    BST(T* data, size_t size);
    BST(BST<T>* other);
    ~BST();

    void balance();
    void erase(const T& value);
    void append(const T& value);
    BST<T>* getSubtree(const T& value) const;
    bool search(const T& value) const;
    void concat(BST<T>& other);
    size_t length();

    DynamicArray<T>* preOderTraversal();
    DynamicArray<T>* inOderTraversal();
    DynamicArray<T>* postOderTraversal();
    DynamicArray<T>* traversal(std::string flag);

    BST<T>* map(std::function<T(const T&)> f);
    BST<T>* where(std::function<bool(const T&)> f);
    T reduce(std::function<T(const T&, const T&)> f, T initial);
    void printTree() const;
};

template <typename T>
BST<T>::BST() : root(nullptr), count(0) {}

template <typename T>
BST<T>::BST(T* data, size_t size) : root(nullptr), count(0) {
    for (size_t i = 0; i < size; ++i) {
        append(data[i]);
    }
}

template <typename T>
BST<T>::BST(BST<T>* other) : root(nullptr), count(0) {
    preOrder(other->root, this);
}

template <typename T>
BST<T>::~BST() {
    clear(root);
}

template <typename T>
typename BST<T>::Node* BST<T>::insert(Node* node, const T& value) {
    if (node == nullptr) {
        count++;
        return new Node(value);
    }
    if (value < node->value)
        node->left = insert(node->left, value);
    else
        node->right = insert(node->right, value);

    return node;
}

template <typename T>
typename BST<T>::Node* BST<T>::remove(Node* node, const T& value, bool& deleted) {
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
        return node;
    }
    return node;
}

template <typename T>
bool BST<T>::search(const T& value) const {
    return searchNode(root, value);
}

template <typename T>
size_t BST<T>::length() {
    return this->count;
}

template <typename T>
typename BST<T>::Node* BST<T>::findMin(Node* node) {
    return node->left ? findMin(node->left) : node;
}

template <typename T>
typename BST<T>::Node* BST<T>::removeMin(Node* node) {
    if (node->left == nullptr) return node->right;
    node->left = removeMin(node->left);
    return node;
}

template <typename T>
void BST<T>::erase(const T& value) {
    bool deleted = false;
    if (root == nullptr) throw EmptyContainer;
    this->root = remove(root, value, deleted);
    if (!deleted){
        throw IncorrectValue;
    }
}

template <typename T>
void BST<T>::append(const T& value) {
    // root = insert(root, value);
    Node* newNode = new Node(value);
    count++;
    if (root == nullptr) {
        root = newNode;
        return;
    }
    Node* current = root;
    Node* parent = nullptr;
    while (current != nullptr) {
        parent = current;
        if (value < current->value) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    if (value < parent->value) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }
}

template <typename T>
BST<T>* BST<T>::getSubtree(const T& value) const {
    Node* node = findNode(root, value);
    if (node == nullptr) throw IncorrectValue;
    BST<T>* subtree = new BST<T>();
    subtree->preOrder(node, subtree);
    return subtree;
}

template <typename T>
void BST<T>::concat(BST<T>& other) {
    DynamicArray<T>* data = other.preOderTraversal();
    for (int i = 0; i < data->GetSize(); ++i) {
        this->append(data->Get(i));
    }
}

template <typename T>
DynamicArray<T>* BST<T>::preOderTraversal() {
    DynamicArray<T>* items = new DynamicArray<T>(0);
    preOrder(root, items);
    return items;
}

template <typename T>
DynamicArray<T>* BST<T>::inOderTraversal() {
    DynamicArray<T>* items = new DynamicArray<T>(0);
    inOrder(root, items);
    return items;
}

template <typename T>
DynamicArray<T>* BST<T>::postOderTraversal() {
    DynamicArray<T>* items = new DynamicArray<T>(0);
    postOrder(root, items);
    return items;
}

template <typename T>
DynamicArray<T>* BST<T>::traversal(std::string flag) {
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
BST<T>* BST<T>::map(std::function<T(const T&)> f) {
    BST<T>* new_tree = new BST();
    mapPreOrder(this->root, f, new_tree);
    return new_tree;
}

template <typename T>
void BST<T>::mapPreOrder(Node* node, std::function<T(const T&)> f, BST<T>* new_tree) {
    if (node) {
        new_tree->append(f(node->value));
        mapPreOrder(node->left, f, new_tree);
        mapPreOrder(node->right, f, new_tree);
    }
}

template <typename T>
BST<T>* BST<T>::where(std::function<bool(const T&)> f) {
    BST<T>* new_tree = new BST();
    wherePreOrder(this->root, f, new_tree);
    return new_tree;
}

template <typename T>
void BST<T>::wherePreOrder(Node* node, std::function<bool(const T&)> f, BST<T>* new_tree) {
    if (node) {
        if (f(node->value)) new_tree->append(node->value);
        wherePreOrder(node->left, f, new_tree);
        wherePreOrder(node->right, f, new_tree);
    }
}

template <typename T>
T BST<T>::reduce(std::function<T(const T&, const T&)> f, T initial) {
    T result = initial;
    reducePreOrder(root, f, result);
    return result;
}

template <typename T>
void BST<T>::reducePreOrder(Node* node, std::function<T(const T&, const T&)> f, T& result) {
    if (node) {
        result = f(result, node->value);
        reducePreOrder(node->left, f, result);
        reducePreOrder(node->right, f, result);
    }
}

template <typename T>
void BST<T>::printTree() const {
    if (root == nullptr) {
        std::cout << "Tree is empty." << std::endl;
        return;
    }
    printHelper(root, "", true);
    std::cout << std::endl;
}

template <typename T>
bool BST<T>::searchNode(Node* node, const T& value) const {
    if (node == nullptr) return false;
    if (value == node->value) return true;
    if (value < node->value)
        return searchNode(node->left, value);
    else
        return searchNode(node->right, value);
}

template <typename T>
typename BST<T>::Node* BST<T>::findNode(Node* node, const T& value) const {
    if (node == nullptr) return nullptr;
    if (value == node->value) return node;
    if (value < node->value)
        return findNode(node->left, value);
    else
        return findNode(node->right, value);
}

template <typename T>
void BST<T>::preOrder(Node* node, DynamicArray<T>* items) {
    if (node) {
        items->Append(node->value);
        preOrder(node->left, items);
        preOrder(node->right, items);
    }
}

template <typename T>
void BST<T>::preOrder(Node* node, BST<T>* tree) {
    if (node) {
        tree->append(node->value);
        preOrder(node->left, tree);
        preOrder(node->right, tree);
    }
}

template <typename T>
void BST<T>::inOrder(Node* node, DynamicArray<T>* items) {
    if (node) {
        inOrder(node->left, items);
        items->Append(node->value);
        inOrder(node->right, items);
    }
}

template <typename T>
void BST<T>::postOrder(Node* node, DynamicArray<T>* items) {
    if (node) {
        postOrder(node->left, items);
        postOrder(node->right, items);
        items->Append(node->value);
    }
}

template <typename T>
void BST<T>::KPL(Node* node, DynamicArray<T>* items) {
    if (node) {
        items->Append(node->value);
        KPL(node->right, items);
        KPL(node->left, items);
    }
}

template <typename T>
void BST<T>::PLK(Node* node, DynamicArray<T>* items) {
    if (node) {
        PLK(node->right, items);
        PLK(node->left, items);
        items->Append(node->value);
    }
}

template <typename T>
void BST<T>::PKL(Node* node, DynamicArray<T>* items) {
    if (node) {
        PKL(node->right, items);
        items->Append(node->value);
        PKL(node->left, items);
    }
}

template <typename T>
void BST<T>::printHelper(Node* node, const std::string& pref, bool flag) const {
    if (node == nullptr) {
        return;
    }

    std::cout << pref << std::endl << pref;
    std::cout << (flag ? "+---- " : "+---- ");
    std::cout << node->value << std::endl;

    std::string new_pref = pref + "      ";

    printHelper(node->left, new_pref, true);
    printHelper(node->right, new_pref, false);
}

template <typename T>
void BST<T>::clear(Node* node) {
    if (node == nullptr) return;
    clear(node->left);
    clear(node->right);
    delete node;
}

template <typename T>
typename BST<T>::Node* BST<T>::rightRotate(Node* node) {
    Node* tmp = node->left;
    node->left = tmp->right;
    tmp->right = node;
    return tmp;
}

template <typename T>
typename BST<T>::Node* BST<T>::leftRotate(Node* grandParent, int count) {
    Node* parent = grandParent->right;
    Node* child;

    for (int i = 0; i < count; ++i) {
        if (!parent || !parent->right) break;

        child = parent->right;
        parent->right = child->left;
        child->left = parent;
        grandParent->right = child;

        grandParent = child;
        parent = grandParent->right;
    }
    return nullptr;
}

template <typename T>
void BST<T>::makeGrapevine() {
    Node fake(0);
    fake.right = root;
    Node* parent = &fake;
    Node* node = root;
    while (node != nullptr) {
        while (node->left != nullptr) {
            node = rightRotate(node);
            parent->right = node;
        }
        parent = node;
        node = node->right;
    }
    root = fake.right;
}

template <typename T>
void BST<T>::balance() {
    makeGrapevine();

    int n = count;
    int m = pow(2, floor(log2(n + 1))) - 1;

    Node dummy(0);
    dummy.right = root;

    leftRotate(&dummy, n - m);

    while (m > 1) {
        m /= 2;
        leftRotate(&dummy, m);
    }

    root = dummy.right;

}
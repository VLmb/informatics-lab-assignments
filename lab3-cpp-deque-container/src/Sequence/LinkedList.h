#ifndef LINKED_LIST_ARRAY_H
#define LINKED_LIST_ARRAY_H

#include <stdexcept>
#include "CustomErrors.h"
#include "Iterator.h"


template<typename T>
class LinkedList{
protected:
    struct Node{
        T data;
        Node* next;
        Node* previous;
        Node(const T& data) : data(data), next(nullptr), previous(nullptr) {}
        Node(const T& data, Node* next, Node* previous) : data(data), next(next), previous(previous) {}
    };
    Node* head;
    Node* tail;
    size_t size;
public:
    LinkedList();
    LinkedList(T* items, int count);
    LinkedList(const LinkedList<T>* list);
    ~LinkedList();

    T GetFirst() const;
    T GetLast() const;
    T& Get(int index) const;
    LinkedList<T>* GetSubList(int startIndex, int endIndex) const;
    int GetLength() const;

    void Append(T item);
    void Prepend(T item);
    void InsertAt(T item, int index);
    void Delete(int index);
    LinkedList<T>* Concat(LinkedList<T>* list);

    Node* merge(Node* a, Node* b);
    void sort();

    class LinkedListIterator: public Iterator<T> {
    private:
        Node* current;
        LinkedList<T>* owner;
        friend class LinkedList;

    public:
        LinkedListIterator() : current(nullptr), owner(nullptr) {}
        LinkedListIterator(Node* node, LinkedList<T>* list) : current(node), owner(list) {}

        T& operator*() override {
        if (!current) throw NotValidArgument;
        return current->data;
        }

        Iterator<T>& operator++() override {
            if (!current) throw IndexOutOfRange;
            current = current->next;
            return *this;
        }

        Iterator<T>& operator--() override {
            if (current == owner->head) throw IndexOutOfRange; 
            if (!current) {
                current = owner->tail;
            } else {
                current = current->previous;
            }
            return *this;
        }

    bool operator!=(const LinkedListIterator& other) const { return current != other.current; }
    bool operator==(const LinkedListIterator& other) const { return current == other.current; }
    };

    LinkedListIterator erase(LinkedListIterator it);

    LinkedListIterator begin() { return LinkedListIterator(head, this); }
    LinkedListIterator end() { return LinkedListIterator(tail->next, this); }

};

template<typename T>
LinkedList<T>::LinkedList() : head(nullptr), tail(nullptr), size(0) {}


template<typename T>
LinkedList<T>::LinkedList(T* items, int count) : head(nullptr), tail(nullptr), size(0) {
    if (items == nullptr && count > 0){
        throw NullPointerPassedAsArgument;
    }
    if (count < 0){
        throw NotValidArgument;
    }
    for (int i = 0; i < count; ++i){
        Append(items[i]);
    }
}

template<typename T>
LinkedList<T>::LinkedList(const LinkedList<T>* list) : head(nullptr), tail(nullptr), size(0) {
    if (!list){
        throw NullPointerPassedAsArgument;
    }
    Node* cur = list->head;
    while (cur){
        Append(cur->data);
        cur = cur->next;
    }
}

template<typename T>
T LinkedList<T>::GetFirst() const {
    if (!head){
        throw EmptyContainer;
    }
    return head->data;
}

template<typename T>
T LinkedList<T>::GetLast() const {
    if (!tail){
        throw EmptyContainer;
    }
    return tail->data;
}

template<typename T>
T& LinkedList<T>::Get(int index) const {
    if (index < 0 || index >= size){
        throw IndexOutOfRange;
    }
    Node* cur = head;
    for (int i = 0; i < index; ++i){
        cur = cur->next;
    }
    return cur->data;
}

template<typename T>
LinkedList<T>* LinkedList<T>::GetSubList(int startIndex, int endIndex) const {
    if (startIndex < 0 || endIndex >= size || startIndex > endIndex) {
        throw IndexOutOfRange;
    }
    LinkedList<T>* sublist = new LinkedList<T>();
    Node* cur = head;
    for (int i = 0; i < startIndex; ++i) {
        cur = cur->next;
    }
    for (int i = startIndex; i <= endIndex; ++i) {
        sublist->Append(cur->data);
        cur = cur->next;
    }
    return sublist;
}

template<typename T>
int LinkedList<T>::GetLength() const {
    return size;
}

template<typename T>
void LinkedList<T>::Append(T item) {
    Node* newNode = new Node{item};
    if (!head){
        this->head = this->tail = newNode;
    }
    else{
        this->tail->next = newNode;
        newNode->previous = this->tail;
        this->tail = newNode;
    }
    ++size;
}

template<typename T>
void LinkedList<T>::Delete(int index) {
    if (index < 0 || index >= size) {
        throw IndexOutOfRange;
    }

    Node* node_to_delete;

    if (index == 0) {
        node_to_delete = head;
        head = head->next;
        if (head) {
            head->previous = nullptr;
        } else { 
            tail = nullptr;
        }
    } else if (index == size - 1) {
        node_to_delete = tail;
        tail = tail->previous;
        if (tail) {
            tail->next = nullptr;
        } else { 
            head = nullptr;
        }
    } else { 
        if (index < size / 2) {
            node_to_delete = head;
            for (int i = 0; i < index; ++i) {
                node_to_delete = node_to_delete->next;
            }
        } else {
            node_to_delete = tail;
            for (int i = 0; i < (size - 1 - index); ++i) {
                node_to_delete = node_to_delete->previous;
            }
        }
        node_to_delete->previous->next = node_to_delete->next;
        node_to_delete->next->previous = node_to_delete->previous;
    }

    delete node_to_delete;
    --size;

}

template<typename T>
typename LinkedList<T>::LinkedListIterator LinkedList<T>::erase(LinkedListIterator it){
    if (it == end()){
        throw NotValidArgument;
    }
    Node* current = it.current;
    LinkedListIterator new_it;

    if (!current){
        throw IndexOutOfRange;
    }
    if (current == head){
        head = current->next;
        if (head){
            head->previous = nullptr;
        }
        else{
            tail = nullptr;
         }
        new_it = LinkedListIterator(head, this);
    }
    else if (current == tail){
        tail = current->previous;
        if (tail){
            tail->next = nullptr;
        }
        else{
            head = nullptr;
        }
        new_it = LinkedListIterator(tail, this);
    }
    else {
        current->previous->next = current->next;
        current->next->previous = current->previous;
        new_it = LinkedListIterator(current->next, this);
    }
    size--;
    delete current;
    return new_it;
}


template<typename T>
void LinkedList<T>::Prepend(T item){
    Node* newNode = new Node{item, head, nullptr};
    if (head){
        head->previous = newNode;
    }
    head = newNode;
    if (!tail){
        tail = newNode;
    }
    ++size;
}

template<typename T>
void LinkedList<T>::InsertAt(T item, int index){
    if (index < 0 || index > size){
        throw IndexOutOfRange;
    }
    if (index == 0){
        Prepend(item);
        return;
    }
    if (index == size){
        Append(item);
        return;
    }
    Node* cur = head;
    for (int i = 0; i < index - 1; ++i){
        cur = cur->next;
    }
    cur->next = new Node(item, cur->next, cur);
    ++size;
}

template<typename T>
LinkedList<T>* LinkedList<T>::Concat(LinkedList<T>* list) {
    if (!list || list->size == 0){ return this; }
    if (this->size == 0){
        *this = *list;
        return this;
    }
    this->tail->next = list->head;
    this->tail = list->tail;
    this->size += list->size;
    list->head = nullptr;
    list->tail = nullptr;
    list->size = 0;
    return this;
}

template<typename T>
LinkedList<T>::~LinkedList() {
    Node* cur = head;
    while (cur) {
        Node* tmp = cur;
        cur = cur->next;
        delete tmp;
    }
    head = nullptr;
    tail = nullptr;
    size = 0;
}

template<typename T>
typename LinkedList<T>::Node* LinkedList<T>::merge(Node* a, Node* b){
    Node* dummy = new Node(T());
    Node* tail = dummy;
    while (a && b) {
        if (a->data <= b->data) {
            tail->next = a;
            a->previous = tail;
            a = a->next;
        } else {
            tail->next = b;
            b->previous = tail;
            b = b->next;
        }
        tail = tail->next;
    }
    if (a) {
        tail->next = a;
        a->previous = tail;
    } else if (b) {
        tail->next = b;
        b->previous = tail;
    }

    Node* result = dummy->next;
    if (result) result->previous = nullptr;
    delete dummy;                                                                                            
    return result;
}

template<typename T>
void LinkedList<T>::sort(){
    if (!head || !head->next) return;

    const int MAX_BITS = 32;
    Node* hooks[MAX_BITS] = {};
    Node* curr = head;

    while (curr) {
        Node* carry = curr;
        curr = curr->next;
        carry->next = nullptr;
        carry->previous = nullptr;

        int i = 0;
        while (i < MAX_BITS && hooks[i]) {
            carry = merge(hooks[i], carry);
            hooks[i] = nullptr;
            i++;
        }
        if (i < MAX_BITS) {
            hooks[i] = carry;
        }
    }

    Node* result = nullptr;
    for (int i = 0; i < MAX_BITS; ++i) {
        if (hooks[i]) {
            result = merge(hooks[i], result);
        }
    }

    head = result;
    tail = head;
    while (tail && tail->next) {
        tail->next->previous = tail;
        tail = tail->next;
    }
}    

#endif 
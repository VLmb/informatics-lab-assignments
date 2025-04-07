#include <stdexcept>


template<typename T>
class LinkedList{
private:
    struct Node{
        T data;
        Node* next;
        Node(const T& data) : data(data), next(nullptr) {}
        Node(const T& data, Node* next) : data(data), next(next) {}
    };
    Node* head;
    Node* tail;
    size_t size;
public:
    LinkedList();
    LinkedList(T* items, int count);
    LinkedList(const LinkedList<T>& list);
    ~LinkedList();

    T GetFirst() const;
    T GetLast() const;
    T Get(int index) const;
    LinkedList<T>* GetSubList(int startIndex, int endIndex) const;
    int GetLength() const;
    bool Equal(LinkedList<T>& otherList) const;

    void Append(T item);
    void Prepend(T item);
    void InsertAt(T item, int index);
    LinkedList<T>* Concat(LinkedList<T>* list);
};

template<typename T>
LinkedList<T>::LinkedList() : head(nullptr), tail(nullptr), size(0) {}


template<typename T>
LinkedList<T>::LinkedList(T* items, int count) : head(nullptr), tail(nullptr), size(0) {
    if (items == nullptr && count > 0){
        throw std::invalid_argument("Null pointer with positive count");
    }
    if (count < 0){
        throw std::invalid_argument("IndexOutOfRange");
    }
    for (int i = 0; i < count; ++i){
        Append(items[i]);
    }
}


template<typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& list) : head(nullptr), tail(nullptr), size(0) {
    Node* cur = list.head;
    while (cur){
        Append(cur->data);
        cur = cur->next;
    }
}

template<typename T>
T LinkedList<T>::GetFirst() const {
    if (!head){
        throw std::range_error("The list is empty");
    }
    return head->data;
}

template<typename T>
T LinkedList<T>::GetLast() const {
    if (!tail){
        throw std::range_error("The list is empty");
    }
    return tail->data;
}

template<typename T>
T LinkedList<T>::Get(int index) const {
    if (index < 0 || index >= size){
        throw std::invalid_argument("IndexOutOfRange");
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
        throw std::invalid_argument("Invalid indices for sublist");
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
        head = tail = newNode;
    }
    else{
        tail->next = newNode;
        tail = newNode;
    }
    ++size;
}

template<typename T>
void LinkedList<T>::Prepend(T item){
    Node* newNode = new Node{item, head};
    head = newNode;
    if (!tail){
        tail = newNode;
    }
    ++size;
}

template<typename T>
void LinkedList<T>::InsertAt(T item, int index){
    if (index < 0 || index > size){
        throw std::invalid_argument("IndexOutOfRange");
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
    cur->next = new Node{item, cur->next};
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
bool LinkedList<T>::Equal(LinkedList<T>& otherList) const {
    if (this->size != otherList->size){
        return false;
    }
    Node* thisEl = this->head;
    Node* otherEl = otherList->head;
    while (thisEl != nullptr){
        if (thisEl->data != otherEl->data){
            return false;
        }
        thisEl = thisEl->next;
        otherEl = otherEl->next;
    }
    return true;
}

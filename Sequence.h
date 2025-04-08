#include <iostream>

template<typename T>
class Sequence{
public:
    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual T Get(int index) const = 0;
    virtual Sequence<T>* GetSubsequence(int startIndex, int endIndex) const = 0;
    virtual int GetLength() const = 0;

    virtual Sequence<T>* Append(const T& item) = 0;
    virtual Sequence<T>* Prepend(const T& item) = 0;
    virtual Sequence<T>* InsertAt(const T& item, int index) = 0;
    virtual Sequence<T>* Concat(Sequence<T>* list) = 0;

    virtual const T& operator[](int index) const = 0;
    virtual T& operator[](int index) = 0;
    virtual bool operator==(const Sequence<T>& otherList) = 0;
    virtual bool operator!=(const Sequence<T>& otherList) = 0;

    virtual Sequence<T>* Map(std::function<T(const T&)> f) const = 0;
    virtual T Reduce(std::function<T(const T&, const T&)>, T initial) const = 0;
    virtual Sequence<T>* Find(std::function<bool(const T&)> f) const = 0;
    template<typename K>
    virtual Sequence<std::tuple<T, K>>* Zip(ArraySequence<K>& otherList) const = 0;
    template<typename K, typename P>
    virtual std::tuple<Sequence<K>*, Sequence<P>*> Unzip() const = 0;
};

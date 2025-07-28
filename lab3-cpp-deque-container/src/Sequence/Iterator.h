#pragma once

template <typename T>
class Iterator {
public:
    virtual T& operator*() = 0;
    virtual Iterator<T>& operator++() = 0;
    virtual Iterator<T>& operator--() = 0;
};

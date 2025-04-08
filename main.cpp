#include <iostream>
#include <functional>

int f(int x) {
    return x * x;
}

class Filter {
    int operator()(int x) {
        return x - 3;
    }
};

template <typename T>
class Container {
    void Map(std::function<T(T)> f) {
        
    }
};

int main(){
    std::cout<<"Привет";
}
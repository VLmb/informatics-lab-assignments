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
    std::string* data = new std::string[100]{};

    for (int i = 0; i < 100; ++i){
        std::cout << "dfd" << " ";
        std::cout << data[i] << " ";
    }

    int (*ptr)(int) = f;
    std::function<int(int)> func = f;
}
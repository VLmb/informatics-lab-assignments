#include <iostream>

int main(){
    std::string* data = new std::string[100]{};

    for (int i = 0; i < 100; ++i){
        std::cout << "dfd" << " ";
        std::cout << data[i] << " ";
    }
}
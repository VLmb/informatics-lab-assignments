#include "AVLTree.h"
#include "BST.h"
#include "Compare.h"
#include <random>
#include "Test.h"
#include "UI.h"


int main(){
    runCompareTests();

    // runUI();

    return 0;
}
//сделать 2-ую версию двоичного дерева: без балансировки, но у которого есть метод Балансировка (делает из несбалансированного сбалансированное)
//сравнить по времени эти две реализации
//баг с высотой при удалении при удалении корней
#include "AVLTree.h" 
#include "Test.h" 

void assert_custom(bool condition, const std::string& testName) {
    if (condition) {
        std::cout << testName << " - [PASSED]\n";
    } else {
        std::cout << testName << " - [FAILED]\n";
    }
}

template<typename T>
bool checkTreePreOrder(AVLTree<T>& tree, T* expData, size_t expSize) {
    DynamicArray<T>* treeData = tree.preOderTraversal();
    if (treeData->GetSize() != expSize) return false;
    
    for (size_t i = 0; i < expSize; ++i) {
        if (treeData->Get(i) != expData[i]) return false;
    }
    return true;
}


void testAVLTree() {
    std::cout << "\n==== Testing AVLTree ====\n";

    // Test 1
    std::cout << "\n### Default constructor ###\n";
    AVLTree<int> tree_def;
    assert_custom(tree_def.traversal("KLP")->GetSize() == 0, "Default constructor: KLP is null");

    // Test 2
    std::cout << "\n### Append - No rotation ###\n";
    int data[3] = {10, 20, 5};
    AVLTree<int> tree(data, 3);
    int exp[] = {10, 5, 20};
    assert_custom(checkTreePreOrder(tree, exp, 3), "Append no rotation: Pre-order check");

    // Test 3
    std::cout << "\n### Append - Right Rotation ###\n";
    int dataRR[4] = {30, 20, 10, 25};
    AVLTree<int> treeRR(dataRR, 4);
    int expRR[] = {20, 10, 30, 25};
    assert_custom(checkTreePreOrder(treeRR, expRR, 4), "Right Rotation: Pre-order check");
    // treeRR.printTree();

    // Test 4
    std::cout << "\n### Append - Left Rotation ###\n";
    int dataLR[4] = {10, 20, 30, 15};
    AVLTree<int> treeLR(dataLR, 4);
    int expLR[] = {20, 10, 15, 30};
    assert_custom(checkTreePreOrder(treeLR, expLR, 4), "Left Rotation: Pre-order check");
    // treeLR.printTree();

    // Test 5
    std::cout << "\n### Append - Left-Right Rotation ###\n";
    int dataLRR[6] = {30, 40, 15, 10, 20, 25};
    AVLTree<int> treeLRR(dataLRR, 6);
    int expLRR[] = {20, 15, 10, 30, 25, 40};
    assert_custom(checkTreePreOrder(treeLRR, expLRR, 6), "LR Rotation: Pre-order check");
    // treeLRR.printTree();

    // Test 6
    std::cout << "\n### Append - Right-Left Rotation ###\n";
    int dataRLR[6] = {15, 10, 30, 40, 20, 25};
    AVLTree<int> treeRLR(dataRLR, 6);
    int expRLR[] = {20, 15, 10, 30, 25, 40};
    assert_custom(checkTreePreOrder(treeRLR, expRLR, 6), "RL Rotation: Pre-order check");
    // treeRLR.printTree();

    // Test 7
    std::cout << "\n### Erase - Leaf node ###\n";
    int dataErLeaf[4] = {20, 10, 30, 5};
    AVLTree<int> treeErLeaf(dataErLeaf, 4);
    treeErLeaf.erase(5);
    int expErLeaf[] = {20, 10, 30};
    assert_custom(checkTreePreOrder(treeErLeaf, expErLeaf, 3), "Erase leaf: Pre-order check");
    // treeErLeaf.printTree();

    // Test 8
    std::cout << "\n### Erase - Node with one child ###\n";
    int dataErOne[4] = {20, 10, 30, 5};
    AVLTree<int> treeErOne(dataErOne, 4);
    treeErOne.erase(10);
    int expErOne[] = {20,5,30};
    assert_custom(checkTreePreOrder(treeErOne, expErOne, 3), "Erase node with one child: Pre-order check");
    // treeErOne.printTree();

    // Test 9
    std::cout << "\n### Erase - Node with two children ###\n";
    int dataErTwo[7] = {4,2,1,3,6,5,7};
    AVLTree<int> treeErTwo(dataErTwo, 7);
    treeErTwo.erase(4);
    int expErTwo[] = {5,2,1,3,6,7};
    assert_custom(checkTreePreOrder(treeErTwo, expErTwo, 6), "Erase node with two children (root): Pre-order check");
    // treeErTwo.printTree();

    // Test 10
    std::cout << "\n### Erase - Causing rotation ###\n";
    int dataErR[8] = {10, 5, 20, 15, 25, 30, 40, 50};
    AVLTree<int> treeErR(dataErR, 8);
    treeErR.erase(10);
    treeErR.erase(5);
    int expErR[] = {30, 20, 15, 25, 40, 50};
    assert_custom(checkTreePreOrder(treeErR, expErR, 6), "Erase causing rotation: Pre-order check");
    // treeErR.printTree();


    // Test 11
    std::cout << "\n### Erase - Non-existent element ###\n";
    int dataErNon[2] = {10, 5};
    AVLTree<int> treeErNon(dataErNon, 2);
    Errors error;
    try {
        treeErNon.erase(100);
    } catch (Errors e) {
        error = e;
        checkErrors(e);
    }
    assert_custom(error == IncorrectValue, "Erase non-existent element throws IncorrectValue");
    int expErNon[] = {10,5};
    assert_custom(checkTreePreOrder(treeErNon, expErNon, 2), "Erase non-existent: Tree unchanged");

    // Test 12: Erase from empty tree
    std::cout << "\n### Erase - From empty tree ###\n";
    AVLTree<int> treeErEmp;
    try {
        treeErEmp.erase(10);
    } catch (Errors e) {
        error = e;
        checkErrors(e);
    }
    assert_custom(error == EmptyContainer, "Erase from empty tree throws EmptyContainer");


    // Test 13
    std::cout << "\n### Traversal functions ###\n";
    int dataTraver[] = {50, 30, 70, 20, 40, 60, 80}; 
    AVLTree<int> treeTraver(dataTraver, 7);

    DynamicArray<int>* preOrderRes = treeTraver.traversal("KLP");
    int expPre[] = {50, 30, 20, 40, 70, 60, 80};
    bool preOrderMatch = true;
    for (int i = 0; i < 7; ++i) {
        if (preOrderRes->Get(i) != expPre[i]) preOrderMatch = false;
    }
    assert_custom(preOrderMatch, "Traversal KLP (Pre-order)");

    DynamicArray<int>* inOrderRes = treeTraver.traversal("LKP");
    int expIn[] = {20, 30, 40, 50, 60, 70, 80};
    bool inOrderMatch = true;
    for (int i = 0; i < 7; ++i) {
        if (inOrderRes->Get(i) != expIn[i]) inOrderMatch = false;
    }
    assert_custom(inOrderMatch, "Traversal LKP (In-order)");

    DynamicArray<int>* postOrderRes = treeTraver.traversal("LPK");
    int expPost[] = {20, 40, 30, 60, 80, 70, 50};
    bool postOrderMatch = true;
    for (int i = 0; i < 7; ++i) {
        if (postOrderRes->Get(i) != expPost[i]) postOrderMatch = false;
    }
    assert_custom(postOrderMatch, "Traversal LPK (Post-order)");
    delete preOrderRes;
    delete inOrderRes;
    delete postOrderRes;

    // Test 14
    std::cout << "\n### Map function ###\n";
    int dataMap[7] = {5, 3, 7, 2, 4, 6, 8};
    AVLTree<int> treeMap(dataMap, 7);
    AVLTree<int>* mappedTree = treeMap.map([](const int& x){ return x * 2; });
    int expMap[] = {10, 6, 4, 8, 14, 12, 16};
    assert_custom(checkTreePreOrder(*mappedTree, expMap, 7), "Map function: Pre-order check");
    delete mappedTree;

    // std::cout << "\n### Map function ###\n";
    // int dataMap[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    // AVLTree<int> treeMap(dataMap, 7);
    // AVLTree<int>* mappedTree = treeMap.map([](const int& x){ return x * 2; });
    // mappedTree->printTree();
    // int expMap[] = {10, 6, 4, 8, 14, 12, 16};
    // assert_custom(checkTreePreOrder(*mappedTree, expMap, 7), "Map function: Pre-order check");
    // delete mappedTree;
    
    AVLTree<int>* mappedRotTree = treeMap.map([](const int& x){ return x % 5; });
    int expRotMap[] = {2, 0, 1, 3, 2, 4, 3};
    assert_custom(checkTreePreOrder(*mappedRotTree, expRotMap, 7), "Map function with rotation: Pre-order check");
    delete mappedRotTree;

    // Test 15
    std::cout << "\n### Where function ###\n";
    int dataWhere[] = {1,2,3,4,5,6,7}; // Root 4. KLP: 4,2,1,3,6,5,7
    AVLTree<int> treeWhere(dataWhere, 7);
    AVLTree<int>* whereTree = treeWhere.where([](const int& x){ return x % 2 == 0; }); // Even numbers: 2,4,6
    int expWhere[] = {4,2,6};
    assert_custom(checkTreePreOrder(*whereTree, expWhere, 3), "Where function (even numbers): Pre-order check");
    // whereTree->printTree();
    delete whereTree;

    // Test 16
    std::cout << "\n### Reduce function ###\n";
    int dataReduce[5] = {5, 4, 8, 9, 8};
    AVLTree<int> treeReduce(dataReduce, 5);
    int sum = treeReduce.reduce([](const int& a, const int& b){ return a + b; }, 0);
    assert_custom(sum == 34, "Reduce function (sum)");

}

void runTests(){
    testAVLTree();
}
#include "Interface.h"
#include "Tests.h"
#include "CustomErrors.h"
#include <sstream>

void clearInput() {
    std::cin.ignore(10000, '\n');
}

template<typename T>
T getTypedInput() {
    std::string input;
    std::getline(std::cin, input);
    T value;
    std::stringstream ss(input);
    if (!(ss >> value)) {
        throw NotValidArgument;
    }
    return value;
}

int getIntInput() {
    std::string input;
    std::getline(std::cin, input);
    int value;
    std::stringstream ss(input);
    if (!(ss >> value)) {
        throw NotValidArgument;
    }
    return value;
}

std::string getStringInput() {
    std::string input;
    std::getline(std::cin, input);
    return input.empty() ? "Unnamed" : input;
}

template<typename T>
MutableArraySequence<T> getArrayInput(std::string type) {
    std::cout << "Enter space-separated values (type: " << type << "): ";
    std::string input;
    std::getline(std::cin, input);
    std::stringstream ss(input);
    MutableArraySequence<T> values;
    T value;
    while (ss >> value) {
        values.Append(value);
    }
    return values;
}

void ShowMenu() {
    std::cout << "\nChoose an action:\n";
    std::cout << "1 - Append\n";
    std::cout << "2 - Prepend\n";
    std::cout << "3 - InsertAt\n";
    std::cout << "4 - Pick sequence\n";
    std::cout << "5 - GetSub\n";
    std::cout << "6 - Map (sum) \n";
    std::cout << "7 - Reduce\n";
    std::cout << "8 - Print\n";
    std::cout << "9 - Create new sequence\n";
    std::cout << "10 - Concat (only one type)\n";
    std::cout << "11 - Delete element by index\n";
    std::cout << "0 - Exit\n";
}

InterfaceWrapper* ChooseSequence(std::string& name) {
    bool correct = false;
    int choice;
    while (correct != true){
        std::cout << "Sequence type:\n";
        std::cout << "1 - MutableArraySequence\n";
        std::cout << "2 - ImmutableArraySequence\n";
        std::cout << "3 - MutableListSequence\n";
        std::cout << "4 - ImmutableListSequence\n";
        std::cout << "Enter choice (1-4): ";
        choice = getIntInput();
        if (choice < 1 || choice > 4){
            std::cout << "Bad input, try again" << std::endl;
        }
        else{
            correct = true;
        }
    }

    std::cout << "Data type:\n";
    std::cout << "1 - int\n";
    std::cout << "2 - float\n";
    std::cout << "3 - string\n";
    std::cout << "Enter choice (1-3): ";
    int typeChoice;
    try {
        typeChoice = getIntInput();
    }
    catch (Errors e) {
        checkErrors(e);
    }

    std::string type;
    if (typeChoice == 1) type = "int";
    else if (typeChoice == 2) type = "float";
    else type = "string";

    std::string structure;
    bool immutable;
    if (choice == 1) {
        structure = "array";
        immutable = false;
        name = "MutableArraySequence<" + type + ">";
    } else if (choice == 2) {
        structure = "array";
        immutable = true;
        name = "ImmutableArraySequence<" + type + ">";
    } else if (choice == 3) {
        structure = "list";
        immutable = false;
        name = "MutableListSequence<" + type + ">";
    } else if (choice == 4) {
        structure = "list";
        immutable = true;
        name = "ImmutableListSequence<" + type + ">";
    } else {
        structure = "array";
        immutable = false;
        type = "int";
        name = "MutableArraySequence<int>";
    }

    if (type == "int") {
        MutableArraySequence<int> values = getArrayInput<int>(type);
        return new Wrapper<int>(structure, type, immutable, values.GetData(), values.GetLength());
    } else if (type == "float") {
        MutableArraySequence<float> values = getArrayInput<float>(type);
        return new Wrapper<float>(structure, type, immutable, values.GetData(), values.GetLength());
    } else {
        MutableArraySequence<std::string> values = getArrayInput<std::string>(type);
        return new Wrapper<std::string>(structure, type, immutable, values.GetData(), values.GetLength());
    }
}

void CreateSequence(MutableArraySequence<InterfaceWrapper*>& seqs) {
    std::string name;
    InterfaceWrapper* newSeq = ChooseSequence(name);
    seqs.Append(newSeq);
    std::cout << "Created new seq, total: " << seqs.GetLength() << "\n";
}

int PickSequence(MutableArraySequence<InterfaceWrapper*>& seqs) {
    if (seqs.GetLength() == 0) {
        std::cout << "No sequences, creating one\n";
        return -1;
    }
    std::cout << "Sequences (" << seqs.GetLength() << "):\n";
    for (int i = 0; i < seqs.GetLength(); i++) {
        InterfaceWrapper* seq = seqs.Get(i);
        std::cout << i << " - " << seq->TypeKey() << ", Structure: " << seq->Structure()
                  << ", Length: " << seq->GetLength() << "\n";
    }
    std::cout << "Enter sequence index (0 to " << seqs.GetLength() - 1 << "): ";
    int index = getIntInput();
    if (index < 0 || index >= seqs.GetLength()) {
        throw IndexOutOfRange;
    }
    return index;
}

void runUI() {
    std::cout << "\n===WELCOME TO THE PROGRAMM===\n";
    bool correct = false;
    int initialChoice;
    while (!correct){
        std::cout << "1 - Sequence Constructor\n";
        std::cout << "2 - Run Tests\n";
        std::cout << "0 - Exit\n";
        std::cout << "Enter choice (0-2): ";
        try {
            initialChoice = getIntInput();
        }
        catch (Errors e) {
            checkErrors(e);
            correct = false;
        }
        if (initialChoice < 0 || initialChoice > 2){
            std::cout << "Bad input, try again" << std::endl;
        }
        else{
            correct = true;
        }
    }

    if (initialChoice == 0) {
        std::cout << "Bye!)\n";
        return;
    } else if (initialChoice == 2) {
        std::cout << "Running tests\n";
        try {
            runTests();
        } catch (Errors error) {
            checkErrors(error);
        }
        std::cout << "Tests done\n";
        return;
    }

    MutableArraySequence<InterfaceWrapper*> sequences;
    int currentSeqIndex = -1;

    while (true) {
        if (sequences.GetLength() == 0) {
            std::cout << "\nNo sequences, creating one\n";
            CreateSequence(sequences);
            currentSeqIndex = 0;
        }
        InterfaceWrapper* currentSeq = sequences.Get(currentSeqIndex);
        // std::cout << "\nCurrent seq: type - " << currentSeq->TypeKey() << "; structure - " << currentSeq->Structure()
        //           << "; " << currentSeq->GetMutable() <<"; index " << currentSeqIndex << "\n";
        ShowMenu();
        std::cout << "Enter choice (0-11): ";
        int choice;
        try {
            choice = getIntInput();
        }
        catch (Errors e) {
            checkErrors(e);
        }

        if (choice == 0) {
            std::cout << "Bye!)\n";
            for (int i = 0; i < sequences.GetLength(); i++) {
                delete sequences.Get(i);
            }
            break;
        }

        try {
            if (choice == 1) currentSeq->doAppend(sequences);
            else if (choice == 2) currentSeq->doPrepend(sequences);
            else if (choice == 3) currentSeq->doInsertAt(sequences);
            else if (choice == 4) {
                currentSeqIndex = PickSequence(sequences);
                if (currentSeqIndex == -1) {
                    CreateSequence(sequences);
                    currentSeqIndex = 0;
                }
                std::cout << "Switched to seq " << currentSeqIndex << "\n";
            } else if (choice == 5) currentSeq->doGetSub();
            else if (choice == 6) currentSeq->doMap(sequences);
            else if (choice == 7) currentSeq->doReduce();
            else if (choice == 8) currentSeq->doPrint();
            else if (choice == 9) CreateSequence(sequences);
            else if (choice == 10) currentSeq->doConcat(sequences);
            else if (choice == 11) currentSeq->doDelete(sequences);
            else std::cout << "Wrong choice\n";
        } catch (Errors error) {
            checkErrors(error);
        }
    }
}
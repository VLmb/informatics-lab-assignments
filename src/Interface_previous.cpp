#include "Interface.h"
#include "Tests.h"
#include "CustomErrors.h"
#include <sstream>
#include <memory>
#include <limits>

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

template<typename T>
T getTypedInput(const std::string& prompt) {
    if (!prompt.empty()){ std::cout << prompt; }
    std::string input;
    std::getline(std::cin, input);
    std::stringstream ss(input);
    T value;
    if (!(ss >> value)) {
        throw NotValidArgument;
    }
    return value;
}

int getIntInput(const std::string& prompt) {
    if (!prompt.empty()){ std::cout << prompt; }
    std::string input;
    std::getline(std::cin, input);  
    std::stringstream ss(input);
    int value;
    if (!(ss >> value)) {
        throw NotValidArgument;
    }
    return value;
}

std::string getStringInput(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input.empty() ? "Unnamed" : input;
}

template<typename T>
MutableArraySequence<T> getArrayInput(const std::string& prompt) {
    std::string input;
    if (!prompt.empty()){ std::cout << prompt; }
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
    std::cout << "\n===== Menu =====\n";
    std::cout << "1 - Append (add to end)\n";
    std::cout << "2 - Prepend (add to start)\n";
    std::cout << "3 - InsertAt (insert at index)\n";
    std::cout << "4 - Pick sequence\n";
    std::cout << "5 - GetSub (get a chunk)\n";
    std::cout << "6 - Map (sum everything)\n";
    std::cout << "7 - Reduce (sum it up)\n";
    std::cout << "8 - Print (show sequence)\n";
    std::cout << "9 - Create new sequence\n";
    std::cout << "0 - Exit (quit)\n";
}

std::shared_ptr<InterfaceWrapper> ChooseSequence(std::string& name) {
    std::cout << "Pick sequence type:\n";
    std::cout << "1 - MutableArraySequence\n";
    std::cout << "2 - ImmutableArraySequence\n";
    std::cout << "3 - MutableListSequence\n";
    std::cout << "4 - ImmutableListSequence\n";
    int choice = getIntInput("Your choice: ");

    std::cout << "Pick data type:\n";
    std::cout << "1 - int\n";
    std::cout << "2 - float\n";
    std::cout << "3 - string\n";
    int typeChoice = getIntInput("Your choice: ");

    std::string type;
    switch (typeChoice) {
        case 1: type = "int"; break;
        case 2: type = "float"; break;
        case 3: type = "string"; break;
        default: type = "int"; break;
    }

    std::string structure;
    bool immutable;
    switch (choice) {
        case 1: structure = "array"; immutable = false; name = "MutableArraySequence<" + type + ">"; break;
        case 2: structure = "array"; immutable = true; name = "ImmutableArraySequence<" + type + ">"; break;
        case 3: structure = "list"; immutable = false; name = "MutableListSequence<" + type + ">"; break;
        case 4: structure = "list"; immutable = true; name = "ImmutableListSequence<" + type + ">"; break;
        default:
            structure = "array"; immutable = false; type = "int"; name = "MutableArraySequence<int>";
    }

    if (type == "int") {
        MutableArraySequence<int> values = getArrayInput<int>("Enter initial elements (space-separated, or press Enter for empty): ");
        return std::make_shared<Wrapper<int>>(structure, type, immutable, values.GetData(), values.GetLength());
    } else if (type == "float") {
        MutableArraySequence<float> values = getArrayInput<float>("Enter initial elements (space-separated, or press Enter for empty): ");
        return std::make_shared<Wrapper<float>>(structure, type, immutable, values.GetData(), values.GetLength());
    } else if (type == "string") {
        MutableArraySequence<std::string> values = getArrayInput<std::string>("Enter initial elements (space-separated, or press Enter for empty): ");
        return std::make_shared<Wrapper<std::string>>(structure, type, immutable, values.GetData(), values.GetLength());
    }
    return std::make_shared<Wrapper<int>>("array", "int", false);
}

void CreateSequence(MutableArraySequence<std::shared_ptr<InterfaceWrapper>>& seqs) {
    std::string name;
    auto newSeq = ChooseSequence(name);
    // std::string customName = getStringInput("Enter name for sequence (or press Enter for default): ");
    // if (!customName.empty() && customName != "Unnamed") name = customName;
    seqs.Append(newSeq);
    std::cout << "Created new seq, total: " << seqs.GetLength() << "\n";
}

int PickSequence(const MutableArraySequence<std::shared_ptr<InterfaceWrapper>>& seqs) {
    if (seqs.GetLength() == 0) {
        std::cout << "No sequences yet, creating one\n";
        return -1;
    }
    std::cout << "Available sequences (" << seqs.GetLength() << "):\n";
    for (int i = 0; i < seqs.GetLength(); i++) {
        auto seq = seqs.Get(i);
        std::cout << i << " - " << seq->TypeKey() << ", Structure: " << seq->Structure()
                  << ", Length: " << seq->GetLength() << "\n";
    }
    int index = getIntInput("Pick seq index: ");
    if (index < 0 || index >= seqs.GetLength()) {
        std::cout << "Out of range, picking first one\n";
        return 0;
    }
    return index;
}

void runUI() {
    std::cout << "\n===== Welcome to Sequence Program =====\n";
    std::cout << "1 - Sequence Constructor (create and work with sequences)\n";
    std::cout << "2 - Run Test System\n";
    std::cout << "0 - Exit\n";
    int initialChoice = getIntInput("Your choice: ");

    if (initialChoice == 0) {
        std::cout << "Bye!\n";
        return;
    } else if (initialChoice == 2) {
        std::cout << "\nRunning tests...\n";
        try {
            runTests();
        } catch (Errors error) {
            checkErrors(error);
        }
        std::cout << "\nTests completed. Exiting.\n";
        return;
    } else if (initialChoice != 1) {
        std::cout << "Invalid choice, starting Sequence Constructor.\n";
    }

    MutableArraySequence<std::shared_ptr<InterfaceWrapper>> sequences;
    int currentSeqIndex = -1;

    while (true) {
        if (sequences.GetLength() == 0) {
            std::cout << "No sequences, let's create one\n";
            CreateSequence(sequences);
            currentSeqIndex = 0;
        }
        auto currentSeq = sequences.Get(currentSeqIndex);
        std::cout << "Current seq: " << currentSeq->TypeKey() << " (" << currentSeq->Structure()
                  << ", index " << currentSeqIndex << ")\n";
        ShowMenu();
        int choice = getIntInput("Your choice: ");

        if (choice == 0) {
            std::cout << "Bye!\n";
            break;
        }

        try {
            switch (choice) {
                case 1: currentSeq->doAppend(sequences); break;
                case 2: currentSeq->doPrepend(sequences); break;
                case 3: currentSeq->doInsertAt(sequences); break;
                case 4:  {
                currentSeqIndex = PickSequence(sequences);
                if (currentSeqIndex == -1) {
                    CreateSequence(sequences);
                    currentSeqIndex = 0;
                }
                std::cout << "Switched to seq " << currentSeqIndex << "\n";
                break;
            }
            default: std::cout << "Wrong choice, try again\n"; continue;
                case 5: currentSeq->doGetSub(); break;
                case 6: currentSeq->doMap(sequences, "Sum" + currentSeq->TypeKey()); break;
                case 7: currentSeq->doReduce(); break;
                case 8: currentSeq->doPrint(); break;
                case 9: CreateSequence(sequences); break;
            }
        } catch (Errors error) {
            checkErrors(error);
    }
}}
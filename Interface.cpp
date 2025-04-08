#include "Interface.h"

int getIntInput(const std::string& prompt) {
    int value;
    std::cout << prompt;
    while (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Bad input, try again: ";
    }
    return value;
}

std::string getStringInput(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::cin.ignore(10000, '\n'); 
    std::getline(std::cin, input);
    return input.empty() ? "Unnamed" : input; 
}


void showMenu() {
    std::cout << "\n=== Menu ===\n";
    std::cout << "1 - Append (add to end)\n";
    std::cout << "2 - Prepend (add to start)\n";
    std::cout << "3 - InsertAt (insert at index)\n";
    std::cout << "4 - Concat (merge with another)\n";
    std::cout << "5 - GetSub (get a chunk)\n";
    std::cout << "6 - Map (square everything)\n";
    std::cout << "7 - Reduce (sum it up)\n";
    std::cout << "8 - Print (show sequence)\n";
    std::cout << "9 - Create new sequence\n";
    std::cout << "10 - Switch sequence\n";
    std::cout << "11 - Set value (mutable only)\n";
    std::cout << "0 - Exit (quit)\n";
    std::cout << "Pick one: ";
}


Sequence<int>* chooseSequence(std::string& name) {
    std::cout << "Pick sequence type:\n";
    std::cout << "1 - MutableArraySeq\n";
    std::cout << "2 - ImmutableArraySeq\n";
    std::cout << "3 - MutableListSeq\n";
    std::cout << "4 - ImmutableListSeq\n";
    int choice = getIntInput("Your choice: ");
    
    switch (choice) {
        case 1: name = "MutableArraySeq"; return new MutableArraySequence<int>();
        case 2: name = "ImmutableArraySeq"; return new ImmutableArraySequence<int>();
        case 3: name = "MutableListSeq"; return new MutableListSequence<int>();
        case 4: name = "ImmutableListSeq"; return new ImmutableListSequence<int>();
        default:
            std::cout << "Wrong pick, taking MutableArraySeq\n";
            name = "MutableArraySeq"; return new MutableArraySequence<int>();
    }
}

void createSequence(MutableArraySequence<std::tuple<Sequence<int>*, std::string>>& seqs) {
    std::string name;
    Sequence<int>* newSeq = chooseSequence(name);
    std::string customName = getStringInput("Enter name for sequence (or press Enter for default): ");
    if (!customName.empty() && customName != "Unnamed") name = customName;
    seqs.Append(std::make_tuple(newSeq, name));
    std::cout << "Created new seq, total: " << seqs.GetLength() << "\n";
}


int pickSequence(const MutableArraySequence<std::tuple<Sequence<int>*, std::string>>& seqs) {
    if (seqs.GetLength() == 0) {
        std::cout << "No sequences yet, creating one\n";
        return -1; 
    }
    std::cout << "Available sequences (" << seqs.GetLength() << "):\n";
    for (int i = 0; i < seqs.GetLength(); i++) {
        auto [seq, name] = seqs.Get(i);
        std::cout << i << " - " << name << ", Length: " << seq->GetLength() << "\n";
    }
    int index = getIntInput("Pick seq index: ");
    if (index < 0 || index >= seqs.GetLength()) {
        std::cout << "Outta range, picking first one\n";
        return 0;
    }
    return index;
}


int doAppend(Sequence<int>*& seq) {
    int value = getIntInput("Enter value: ");
    Sequence<int>* newSeq = seq->Append(value);
    if (seq != newSeq) { 
        delete seq;
        seq = newSeq;
    }
    return OK; 
}


int doPrepend(Sequence<int>*& seq) {
    int value = getIntInput("Enter value: ");
    Sequence<int>* newSeq = seq->Prepend(value);
    if (seq != newSeq) {
        delete seq;
        seq = newSeq;
    }
    return OK; 
}


int doInsert(Sequence<int>* seq) {
    int value = getIntInput("Enter value: ");
    int index = getIntInput("Enter index: ");
    seq->InsertAt(value, index); 
    return OK; 
}


int doConcat(Sequence<int>*& seq) {
    std::string name;
    Sequence<int>* seq2 = chooseSequence(name);
    std::cout << "Enter some numbers for second seq (0 to stop):\n";
    while (true) {
        int val = getIntInput("Value: ");
        if (val == 0) break;
        seq2->Append(val);
    }
    Sequence<int>* newSeq = seq->Concat(seq2);
    if (seq != newSeq) {
        delete seq;
        seq = newSeq;
    }
    delete seq2;
    return OK; 
}


int doGetSub(Sequence<int>* seq) {
    int start = getIntInput("Enter start index: ");
    int end = getIntInput("Enter end index: ");
    Sequence<int>* subSeq = seq->GetSubsequence(start, end);
    if (!subSeq) return NullPointerPassedAsArgument; 
    std::cout << "Chunk: ";
    for (int i = 0; i < subSeq->GetLength(); i++) {
        std::cout << subSeq->Get(i) << " ";
    }
    std::cout << "\n";
    delete subSeq;
    return OK; 
}


int doMap(Sequence<int>*& seq) {
    auto square = [](const int& x) { return x * x; };
    Sequence<int>* newSeq = seq->Map(square);
    if (seq != newSeq) {
        delete seq;
        seq = newSeq;
    }
    return OK; 
}


int doReduce(Sequence<int>* seq) {
    auto sum = [](int a, int b) { return a + b; };
    int result = seq->Reduce(sum, 0);
    std::cout << "Sum: " << result << "\n";
    return OK; 
}


void doPrint(Sequence<int>* seq) {
    std::cout << "Sequence: ";
    for (int i = 0; i < seq->GetLength(); i++) {
        std::cout << seq->Get(i) << " ";
    }
    std::cout << "\n";
}


int doSet(Sequence<int>* seq) {
    if (dynamic_cast<MutableArraySequence<int>*>(seq)) {
        MutableArraySequence<int>* mutableSeq = static_cast<MutableArraySequence<int>*>(seq);
        int pos = getIntInput("Enter index: ");
        int value = getIntInput("Enter value: ");
        mutableSeq->InsertAt(value, pos); 
        return OK;
    } else if (dynamic_cast<MutableListSequence<int>*>(seq)) {
        MutableListSequence<int>* mutableSeq = static_cast<MutableListSequence<int>*>(seq);
        int pos = getIntInput("Enter index: ");
        int value = getIntInput("Enter value: ");
        mutableSeq->InsertAt(value, pos); 
        return OK;
    } else {
        return NotValidArgument; 
    }
}


void runUI() {
    MutableArraySequence<std::tuple<Sequence<int>*, std::string>> sequences;
    int currentSeqIndex = -1; 

    while (true) {
        if (sequences.GetLength() == 0) {
            std::cout << "No sequences, let's create one\n";
            createSequence(sequences);
            currentSeqIndex = 0;
        }
        std::cout << "Current seq: " << std::get<1>(sequences.Get(currentSeqIndex)) 
                  << " (index " << currentSeqIndex << ")\n";
        showMenu();
        int choice = getIntInput("Your choice: ");

        if (choice == 0) {
            std::cout << "Bye!\n";
            break;
        }

        Sequence<int>* currentSeq = std::get<0>(sequences.Get(currentSeqIndex));
        int error = OK;
        switch (choice) {
            case 1: error = doAppend(currentSeq); break;
            case 2: error = doPrepend(currentSeq); break;
            case 3: error = doInsert(currentSeq); break;
            case 4: error = doConcat(currentSeq); break;
            case 5: error = doGetSub(currentSeq); break;
            case 6: error = doMap(currentSeq); break;
            case 7: error = doReduce(currentSeq); break;
            case 8: doPrint(currentSeq); break;
            case 9: createSequence(sequences); break;
            case 10:
                currentSeqIndex = pickSequence(sequences);
                if (currentSeqIndex == -1) {
                    createSequence(sequences);
                    currentSeqIndex = 0;
                }
                std::cout << "Switched to seq " << currentSeqIndex << "\n";
                break;
            case 11: error = doSet(currentSeq); break;
            default: std::cout << "Wrong choice, try again\n"; continue;
        }
        checkErrors(error);
    }

    
    for (int i = 0; i < sequences.GetLength(); i++) {
        delete std::get<0>(sequences.Get(i));
    }
}
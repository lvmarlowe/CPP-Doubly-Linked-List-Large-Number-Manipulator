/*  LV Marlowe
    SDEV-345: Data Structures & Algorithms
    Week 7: Assignment
    13 Oct 2024
    This file implements the LargeNumber class methods
    defined in LargeNumber.h. It includes the implementation
    of arithmetic operations, constructors, and helper functions
    for handling large numbers using a doubly linked list structure.
    It requires LargeNumber.h, <algorithm>, <iomanip>, and <stdexcept> to run.
-------------------------------------------------- */

#include "LargeNumber.h"
#include <algorithm> // For element ranges (all_of())
#include <iomanip> // For formatting (setw() and setfill())
#include <stdexcept> // For exceptions (out_of_range)

using namespace std;

// Default constructor
LargeNumber::LargeNumber() : head(nullptr), tail(nullptr), negative(false), size(0) {}

// Constructor from string
LargeNumber::LargeNumber(const string& number) : head(nullptr), tail(nullptr), negative(false), size(0) {
    string reversedNumber = number;
    reverse(reversedNumber.begin(), reversedNumber.end());

    // Add leading zeros to make the length a multiple of 3
    while (reversedNumber.length() % 3 != 0) {
        reversedNumber += '0';  
    }

    for (size_t i = 0; i < reversedNumber.length(); i += 3) {
        string chunk = reversedNumber.substr(i, 3);
        reverse(chunk.begin(), chunk.end());
        int value = stoi(chunk);

        // Only add non-zero nodes
        if (value != 0 || size > 0) {
            addNodeFront(value);
        }
    }

    // Add node to front if input was zero
    if (size == 0) {
        addNodeFront(0);
    }
}

// Virtual destructor
LargeNumber::~LargeNumber() {
    clear();
}

// Copy constructor
LargeNumber::LargeNumber(const LargeNumber& other) : head(nullptr), tail(nullptr), negative(other.negative), size(0) {
    copyFrom(other);
}

// Overloaded assignment (=) operator
LargeNumber& LargeNumber::operator=(const LargeNumber& other) {
    if (this != &other) {
        clear();
        negative = other.negative;
        size = 0;
        copyFrom(other);
    }
    return *this;
}

// Overloaded addition (+) operator
LargeNumber LargeNumber::operator+(const LargeNumber& other) const {
    LargeNumber result;
    const Node* n1 = this->tail;
    const Node* n2 = other.tail;
    int carry = 0;

    while (n1 || n2 || carry) {
        int sum = carry;
        if (n1) {
            sum += n1->value;
            n1 = n1->prev;
        }
        if (n2) {
            sum += n2->value;
            n2 = n2->prev;
        }
        result.addNodeFront(sum % 1000);
        carry = sum / 1000;
    }

    return result;
}

// Overloaded subtraction (-) operator
LargeNumber LargeNumber::operator-(const LargeNumber& other) const {
    LargeNumber result;
    const Node* n1 = this->tail;
    const Node* n2 = other.tail;
    int borrow = 0;
    bool isNegative = false;

    if (compare(other) < 0) {
        isNegative = true;
        swap(n1, n2);
    }

    while (n1 || n2) {
        int diff = -borrow;
        if (n1) {
            diff += n1->value;
            n1 = n1->prev;
        }
        if (n2) {
            diff -= n2->value;
            n2 = n2->prev;
        }

        if (diff < 0) {
            diff += 1000;
            borrow = 1;
        }
        else {
            borrow = 0;
        }

        result.addNodeFront(diff);
    }

    while (result.head && result.head != result.tail && result.head->value == 0) {
        Node* temp = result.head;
        result.head = result.head->next;
        result.head->prev = nullptr;
        delete temp;
        result.size--;
    }

    result.negative = isNegative;
    return result;
}

// Insert a node at a specific position
void LargeNumber::insertNode(int position, int value) {
    if (position < 0 || position > size) {
        throw out_of_range("Invalid position for insertion");
    }

    Node* newNode = new Node(value);

    if (position == 0) {
        newNode->next = head;
        if (head) head->prev = newNode;
        head = newNode;
        if (!tail) tail = newNode;
    }
    else if (position == size) {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    else {
        Node* current = getNodeAtPosition(position);
        newNode->prev = current->prev;
        newNode->next = current;
        current->prev->next = newNode;
        current->prev = newNode;
    }

    size++;
}

// Delete a node at a specific position
void LargeNumber::deleteNode(int position) {
    if (position < 0 || position >= size) {
        throw out_of_range("Invalid position for deletion");
    }

    Node* nodeToDelete;

    if (position == 0) {
        nodeToDelete = head;
        head = head->next;
        if (head) head->prev = nullptr;
        if (size == 1) tail = nullptr;
    }
    else if (position == size - 1) {
        nodeToDelete = tail;
        tail = tail->prev;
        tail->next = nullptr;
    }
    else {
        nodeToDelete = getNodeAtPosition(position);
        nodeToDelete->prev->next = nodeToDelete->next;
        nodeToDelete->next->prev = nodeToDelete->prev;
    }

    delete nodeToDelete;
    size--;
}

// Modify the value of a node at a specific position
void LargeNumber::modifyNode(int position, int newValue) {
    if (position < 0 || position >= size) {
        throw out_of_range("Invalid position for modification");
    }

    Node* node = getNodeAtPosition(position);
    node->value = newValue;
}

// Overloaded output stream (<<) operator for improved printing
ostream& operator<<(ostream& os, const LargeNumber& num) {
    if (num.negative) {
        os << "-";
    }
    Node* current = num.head;
    bool first = true;
    while (current) {
        if (!first) {
            os << ",";
        }

        // Print the first node's value without leading zeros
        if (first) {
            os << current->value;  
        }

        // Print subsequent nodes with leading zeros
        else {
            os << setfill('0') << setw(3) << current->value;  
        }
        first = false;
        current = current->next;
    }

    // Print ) if the number is zero
    if (!num.head) {  
        os << "0";
    }
    return os;
}

// Add a new node to end of list
void LargeNumber::addNode(int value) {
    Node* newNode = new Node(value);
    if (!head) {
        head = tail = newNode;
    }
    else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    size++;
}

// Add a new node to front of list
void LargeNumber::addNodeFront(int value) {
    Node* newNode = new Node(value);
    if (!head) {
        head = tail = newNode;
    }
    else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
    size++;
}

// Compare two LargeNumbers
int LargeNumber::compare(const LargeNumber& other) const {
    if (size != other.size) {
        return size - other.size;
    }

    Node* n1 = head;
    Node* n2 = other.head;
    while (n1 && n2) {
        if (n1->value != n2->value) {
            return n1->value - n2->value;
        }
        n1 = n1->next;
        n2 = n2->next;
    }

    return 0;
}

// Clear linked list
void LargeNumber::clear() {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    tail = nullptr;
    size = 0;
}

// Copy nodes from another LargeNumber instance
void LargeNumber::copyFrom(const LargeNumber& other) {
    Node* current = other.head;
    while (current) {
        addNode(current->value);
        current = current->next;
    }
}

// Get node at a specific position
Node* LargeNumber::getNodeAtPosition(int position) const {
    if (position < 0 || position >= size) {
        throw out_of_range("Invalid position");
    }

    Node* current;
    if (position < size / 2) {
        current = head;
        for (int i = 0; i < position; i++) {
            current = current->next;
        }
    }
    else {
        current = tail;
        for (int i = size - 1; i > position; i--) {
            current = current->prev;
        }
    }
    return current;
}
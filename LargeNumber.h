/*  LV Marlowe
    SDEV-345: Data Structures & Algorithms
    Week 7: Assignment
    13 Oct 2024
    This header file defines the LargeNumber class,
    which implements arbitrary-precision arithmetic
    using a doubly linked list structure. It includes methods
    for addition, subtraction, comparison of large numbers,
    and operations for inserting, deleting, and modifying nodes.
    This file requires the standard libraries <string> and
    <iostream>.
-------------------------------------------------- */

#ifndef LARGE_NUMBER_H
#define LARGE_NUMBER_H
#include <string>
#include <iostream>

// Node structure for doubly linked list
struct Node {
    int value;
    Node* next;
    Node* prev;
    Node(int val) : value(val), next(nullptr), prev(nullptr) {}
};

class LargeNumber {
public:

    // Constructors and destructor
    LargeNumber();
    explicit LargeNumber(const std::string& number);
    ~LargeNumber();
    LargeNumber(const LargeNumber& other);
    LargeNumber& operator=(const LargeNumber& other);

    // Arithmetic operations
    LargeNumber operator+(const LargeNumber& other) const;
    LargeNumber operator-(const LargeNumber& other) const;

    // Node manipulation method
    void insertNode(int position, int value);
    void deleteNode(int position);
    void modifyNode(int position, int newValue);

    // Overloaded output stream operator for improved printing
    friend std::ostream& operator<<(std::ostream& os, const LargeNumber& num);

    // Accessor methods
    bool isNegative() const { return negative; }
    Node* getHead() const { return head; }
    int getSize() const { return size; }

private:
    Node* head;
    Node* tail;
    bool negative;
    int size; // Track number of nodes

    // Helper methods for internal operations
    void addNode(int value);
    void addNodeFront(int value);
    int compare(const LargeNumber& other) const;
    void clear();
    void copyFrom(const LargeNumber& other);
    Node* getNodeAtPosition(int position) const;
};

#endif // LARGE_NUMBER_H
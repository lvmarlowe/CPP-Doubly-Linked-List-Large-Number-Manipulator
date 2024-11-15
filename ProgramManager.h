/*  LV Marlowe
    SDEV-345: Data Structures & Algorithms
    Week 7: Assignment
    13 Oct 2024
    This header file defines the ProgramManager class,
    which handles the main program flow, user interaction,
    and operations for the large number arithmetic calculator.
    It includes method declarations for addition, subtraction,
    node manipulation, and input validation.
    This file requires LargeNumber.h and <string>.
-------------------------------------------------- */

#ifndef PROGRAM_MANAGER_H
#define PROGRAM_MANAGER_H

#include "LargeNumber.h"
#include <string>

class ProgramManager {
public:

    // Default constructor
    ProgramManager() = default;

    // Main program loop
    void run();

private:

    // Helper methods
    LargeNumber performAddition(const LargeNumber& number);
    LargeNumber performSubtraction(const LargeNumber& number);
    void printResult(const LargeNumber& result);
    void printNodes(const LargeNumber& number);
    LargeNumber getInputNumber();
    std::string getValidInput(const std::string& prompt, bool (*validator)(const std::string&));
    int getValidIntegerInput(const std::string& prompt, int min, int max);

    // Node manipulation method
    void manipulateNode(LargeNumber& number, const std::string& operation);

    // Input validation methods
    static bool isValidNumber(const std::string& input);
    static bool isValidInteger(const std::string& input);
};

#endif // PROGRAM_MANAGER_H
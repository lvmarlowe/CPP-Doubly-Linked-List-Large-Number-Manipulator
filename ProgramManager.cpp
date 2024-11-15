/*  LV Marlowe
    SDEV-345: Data Structures & Algorithms
    Week 7: Assignment
    13 Oct 2024
    This file implements the ProgramManager class methods
    defined in ProgramManager.h. It handles user input,
    program flow, and output formatting for the large number
    arithmetic calculator. It manages the interaction between
    the user and the LargeNumber operations.
    It requires ProgramManager.h, <iostream>, <iomanip>,
    <limits>, <algorithm>, and <stdexcept> to run.
-------------------------------------------------- */

#include "ProgramManager.h"
#include <iostream>
#include <iomanip> // For formatting (setw() and setfill())
#include <limits> // For numeric limits
#include <algorithm> // For element ranges (all_of())
#include <stdexcept> // For exceptions (out_of_range)

using namespace std;

// Run main program loop
void ProgramManager::run() {
    cout << "\n+---------------------------------------------+" << endl;
    cout << "|    Welcome to Large Number Manipulator       |" << endl;
    cout << "+---------------------------------------------+" << endl;

    LargeNumber number = getInputNumber();
    cout << "| Initial number entered: " << number << endl;

    // Display main menu until user exits program
    while (true) {
        cout << "+--- Menu ------------------------------------+" << endl;
        cout << "| 1. Add to Number                            |" << endl;
        cout << "| 2. Subtract from Number                     |" << endl;
        cout << "| 3. Insert a Node                            |" << endl;
        cout << "| 4. Delete a Node                            |" << endl;
        cout << "| 5. Modify a Node                            |" << endl;
        cout << "| 6. Exit                                     |" << endl;
        cout << "+---------------------------------------------+" << endl;

        int choice = getValidIntegerInput("| Enter your choice: ", 1, 6);

        switch (choice) {
        case 1:
            number = performAddition(number);
            break;
        case 2:
            number = performSubtraction(number);
            break;
        case 3:
            manipulateNode(number, "Insert");
            break;
        case 4:
            manipulateNode(number, "Delete");
            break;
        case 5:
            manipulateNode(number, "Modify");
            break;
        case 6:
            cout << "+--- Thank You -------------------------------+" << endl;
            cout << "| Thank you for using the program.            |" << endl;
            cout << "| Goodbye!                                    |" << endl;
            cout << "+---------------------------------------------+" << endl;
            return;
        }
    }
}

// Perform addition
LargeNumber ProgramManager::performAddition(const LargeNumber& number) {
    cout << "+--- Addition ---------------------------------+" << endl;
    LargeNumber secondNumber = getInputNumber();
    LargeNumber result = number + secondNumber;
    printResult(result);
    return result;
}

// Perform subtraction
LargeNumber ProgramManager::performSubtraction(const LargeNumber& number) {
    cout << "+--- Subtraction ------------------------------+" << endl;
    LargeNumber secondNumber = getInputNumber();
    LargeNumber result = number - secondNumber;
    printResult(result);
    return result;
}

// Display result
void ProgramManager::printResult(const LargeNumber& result) {
    cout << "| The answer is                               |" << endl;
    printNodes(result);
    cout << "| Giving: " << result << endl;
    cout << "+---------------------------------------------+" << endl;
}

// Print nodes
void ProgramManager::printNodes(const LargeNumber& number) {
    Node* current = number.getHead();
    int nodeCount = 1;
    while (current) {
        cout << "| Node " << setw(1) << nodeCount << " = " << setfill('0') << setw(3) << current->value << setfill(' ') << endl;
        current = current->next;
        nodeCount++;
    }
}

// Obtain valid number input from user
LargeNumber ProgramManager::getInputNumber() {
    cout << "+--- Input -----------------------------------+" << endl;
    string input = getValidInput("| Enter a number: ", isValidNumber);
    cout << "+---------------------------------------------+" << endl;
    return LargeNumber(input);
}

// Validate input
string ProgramManager::getValidInput(const string& prompt, bool (*validator)(const string&)) {
    string input;

    // Remove blank space from input
    do {
        cout << prompt;
        getline(cin, input);
        input.erase(0, input.find_first_not_of(" \t\n\r\f\v"));
        input.erase(input.find_last_not_of(" \t\n\r\f\v") + 1);
        if (!validator(input)) {
            cout << "| Invalid input. Please try again.           |" << endl;
        }
    } while (!validator(input));
    return input;
}

// Get valid integer input of 0 through 999 for 3-digit chunk
int ProgramManager::getValidIntegerInput(const string& prompt, int min, int max) {
    while (true) {
        string input = getValidInput(prompt, isValidInteger);
        try {
            int value = stoi(input);
            if (value >= min && value <= max) {
                return value;
            }
            cout << "| Input out of range. Please enter a number  |" << endl;
            cout << "| between " << min << " and " << max << ".                        |" << endl;
        }
        catch (const std::invalid_argument&) {
            cout << "| Invalid input. Please enter a number.      |" << endl;
        }
        catch (const std::out_of_range&) {
            cout << "| Number too large. Please try again.        |" << endl;
        }
    }
}

// Manipulate node
void ProgramManager::manipulateNode(LargeNumber& number, const string& operation) {
    cout << "+--- " << operation << " Node ------------------------------+" << endl;

    // Display current number and node positions
    cout << "| Current number: " << number << endl;
    printNodes(number);
    cout << "| Node positions: ";

    int maxPosition = number.getSize();
    if (operation == "Insert") {
        cout << "0 (before Node 1), ";
        for (int i = 1; i <= maxPosition; i++) {
            cout << i;
            if (i < maxPosition) cout << ", ";
        }
        cout << " (after Node " << maxPosition << ")";
    }
    else {
        for (int i = 1; i <= maxPosition; i++) {
            cout << i;
            if (i < maxPosition) cout << ", ";
        }
    }
    cout << endl;

    int positionChoice;

    // Get position choice based on operation
    if (operation == "Insert") {
        positionChoice = getValidIntegerInput("| Enter position to insert (0 to " + to_string(maxPosition) + "): ", 0, maxPosition);
    }
    else if (operation == "Delete") {
        positionChoice = getValidIntegerInput("| Enter position to delete (1 to " + to_string(maxPosition) + "): ", 1, maxPosition);
    }
    else {
        positionChoice = getValidIntegerInput("| Enter position to modify (1 to " + to_string(maxPosition) + "): ", 1, maxPosition);
    }

    // Get value for insert or modify operations
    int value = (operation != "Delete") ? getValidIntegerInput("| Enter value (0-999): ", 0, 999) : 0;

    try {
        // Perform the requested operation
        if (operation == "Insert") {
            number.insertNode(positionChoice, value);
        }
        else if (operation == "Delete") {
            number.deleteNode(positionChoice - 1);
        }
        else if (operation == "Modify") {
            number.modifyNode(positionChoice - 1, value);
        }

        // Display success message based on operation
        if (operation == "Insert") {
            cout << "| Node inserted successfully.                  |" << endl;
        }
        else if (operation == "Delete") {
            cout << "| Node deleted successfully.                   |" << endl;
        }
        else if (operation == "Modify") {
            cout << "| Node modified successfully.                  |" << endl;
        }

        cout << "| Updated number: " << number << endl;
        printNodes(number);
    }
    catch (const out_of_range& e) {
        cout << "| Error: " << e.what() << endl;
    }
    cout << "+---------------------------------------------+" << endl;
}

// Check if string represents a valid positive integer
bool ProgramManager::isValidNumber(const string& input) {
    return !input.empty() && all_of(input.begin(), input.end(), ::isdigit);
}

// Check if string represents a valid integer
bool ProgramManager::isValidInteger(const string& input) {
    return !input.empty() && (input[0] == '-' || isdigit(input[0])) &&
        all_of(input.begin() + 1, input.end(), ::isdigit);
}
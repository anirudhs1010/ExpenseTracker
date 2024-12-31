#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <stdexcept>

using namespace std;

// Structure to hold an expense
struct Expense {
    int id;
    string description;
    double amount;
};

// Global variables for expense tracking
vector<Expense> expenses;
int currentID = 1;

// Function to add an expense
void addExpense(const string& description, double amount) {
    Expense e = {currentID, description, amount};
    expenses.push_back(e);
    cout << "# Expense added successfully (ID: " << currentID << ")\n";
    currentID++;
}

// Function to parse arguments
map<string, string> parseArguments(int argc, char* argv[]) {
    map<string, string> options;
    for (int i = 2; i < argc; i++) {
        string arg = argv[i];
        if (arg[0] == '-' && arg[1] == '-') {
            if (i + 1 < argc) {
                options[arg] = argv[i + 1];
                i++;
            } else {
                throw invalid_argument("Missing value for option: " + arg);
            }
        }
    }
    return options;
}

// Main function
int main(int argc, char* argv[]) {
    try {
        if (argc < 2) {
            cerr << "Error: No command provided.\n";
            return 1;
        }

        string command = argv[1];
        if (command == "add") {
            map<string, string> options = parseArguments(argc, argv);
            if (options.count("--description") && options.count("--amount")) {
                string description = options["--description"];
                double amount = stod(options["--amount"]);
                addExpense(description, amount);
            } else {
                cerr << "Error: Missing required options --description or --amount.\n";
                return 1;
            }
            cout << "ID     Description    Amount" << endl;
            for (int i= 0; i < expenses.size(); i++) {
                cout << expenses[i].id << " " << expenses[i].description << " " << expenses[i].amount<< endl;
            }
        }
        else if (command == "list") {
            
        } else {
            cerr << "Error: Unknown command \"" << command << "\".\n";
            return 1;
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

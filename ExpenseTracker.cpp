#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <ctime>
#include <iomanip> //using get_time
using namespace std;

// Structure to hold an expense
struct Expense {
    int id;
    string description;
    double amount;
    string date;
};

// Global variables for expense tracking
vector<Expense> expenses;
int currentID = 1;
const string expenseFile = "expenses.csv";

// Function to get the current date as a string
string getCurrentDate() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    stringstream ss;
    ss << 1900 + ltm->tm_year << "-" << 1 + ltm->tm_mon << "-" << ltm->tm_mday;
    return ss.str();
}

// Function to load expenses from a file
void loadExpenses() {
    ifstream file(expenseFile);
    if (!file.is_open()) return;
    
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Expense e;
        char delimiter;
        ss >> e.id >> delimiter;
        getline(ss, e.description, ',');
        ss >> e.amount >> delimiter;
        getline(ss, e.date, ',');
        expenses.push_back(e);
        if (e.id >= currentID) currentID = e.id + 1;
    }
    file.close();
}

// Function to save expenses to a file
void saveExpenses() {
    ofstream file(expenseFile);
    for (int i = 0; i < expenses.size(); i++) {
        file << expenses[i].id << "," << expenses[i].description << "," << expenses[i].amount << "," << expenses[i].date << "\n";
    }
    file.close();
}

// Function to add an expense
void addExpense(const string& description, double amount) {
    Expense e = {currentID, description, amount, getCurrentDate()};
    expenses.push_back(e);
    cout << "# Expense added successfully (ID: " << currentID << ")\n";
    currentID++;
    saveExpenses();
}

// Function to update an expense
void updateExpense(int id, const string& description, double amount) {
    for (int i = 0; i < expenses.size(); i++) {
        if (expenses[i].id == id) {
            expenses[i].description = description;
            expenses[i].amount = amount;
            cout << "# Expense updated successfully (ID: " << id << ")\n";
            saveExpenses();
            return;
        }
    }
    cout << "# Error: Expense ID not found.\n";
}

// Function to delete an expense
void deleteExpense(int id) {
    for (vector<Expense>::iterator it = expenses.begin(); it != expenses.end(); ++it) {
        if (it->id == id) {
            expenses.erase(it);
            cout << "# Expense deleted successfully\n";
            saveExpenses();
            return;
        }
    }
    cout << "# Error: Expense ID not found.\n";
}

// Function to list all expenses
void listExpenses() {
    cout << "ID  Date       Description  Amount\n";
    for (int i = 0; i < expenses.size(); i++) {
        cout << expenses[i].id << "  " << expenses[i].date << "  " << expenses[i].description << "  $" << expenses[i].amount << "\n";
    }
}

// Function to show a summary of all expenses
void showSummary() {
    double total = 0;
    for (int i = 0; i < expenses.size(); i++) {
        total += expenses[i].amount;
    }
    cout << "# Total expenses: $" << total << "\n";
}

// Function to show a summary of expenses for a specific month
void showMonthlySummary(int month) {
    double total = 0;
    for (int i = 0; i < expenses.size(); i++) {
        tm t = {};
        stringstream ss(expenses[i].date);
        ss >> get_time(&t, "%Y-%m-%d");
        if (t.tm_mon + 1 == month) {
            total += expenses[i].amount;
        }
    }
    cout << "# Total expenses for month " << month << ": $" << total << "\n";
}

// Function to export expenses to a CSV file
void exportToCSV(const string& filename) {
    ofstream file(filename);
    file << "ID,Description,Amount,Date\n";
    for (int i = 0; i < expenses.size(); i++) {
        file << expenses[i].id << "," << expenses[i].description << "," << expenses[i].amount << "," << expenses[i].date << "\n";
    }
    file.close();
    cout << "# Expenses exported to " << filename << " successfully.\n";
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
    loadExpenses();
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
        }
        else if (command == "update") {
            map<string, string> options = parseArguments(argc, argv);
            if (options.count("--id") && options.count("--description") && options.count("--amount")) {
                int id = stoi(options["--id"]);
                string description = options["--description"];
                double amount = stod(options["--amount"]);
                updateExpense(id, description, amount);
            } else {
                cerr << "Error: Missing required options --id, --description or --amount.\n";
                return 1;
            }
        }
        else if (command == "delete") {
            map<string, string> options = parseArguments(argc, argv);
            if (options.count("--id")) {
                int id = stoi(options["--id"]);
                deleteExpense(id);
            } else {
                cerr << "Error: Missing required option --id.\n";
                return 1;
            }
        }
        else if (command == "list") {
            listExpenses();
        }
        else if (command == "summary") {
            if (argc == 2) {
                showSummary();
            } else {
                map<string, string> options = parseArguments(argc, argv);
                if (options.count("--month")) {
                    int month = stoi(options["--month"]);
                    showMonthlySummary(month);
                } else {
                    cerr << "Error: Missing required option --month.\n";
                    return 1;
                }
            }
        }
        else if (command == "export") {
            map<string, string> options = parseArguments(argc, argv);
            if (options.count("--filename")) {
                string filename = options["--filename"];
                exportToCSV(filename);
            } else {
                cerr << "Error: Missing required option --filename.\n";
                return 1;
            }
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

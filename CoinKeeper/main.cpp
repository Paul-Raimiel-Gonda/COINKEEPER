#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

//enum ay parang ano like category?? idk
enum BudgetType { DAILY, WEEKLY, MONTHLY };

class Budget {
public:
    Budget(double amount, BudgetType type)
        : amount(amount), type(type) {}

    double getAmount() const { return amount; }
    BudgetType getType() const { return type; }

    void setAmount(double newAmount) { amount = newAmount; }

private:
    double amount;
    BudgetType type;
};

class User {
public:
    User(const string& name)
        : name(name) {}

    void addBudget(double amount, BudgetType type) {
        budgets[type].emplace_back(amount, type);
    }

    void modifyBudget(double amount, BudgetType type) {
        if (budgets.find(type) != budgets.end() && !budgets[type].empty()) {
            budgets[type].back().setAmount(budgets[type].back().getAmount() + amount);
        }
        else {
            cout << "No budget found for the given type to modify." << endl;
        }
    }

    void showBudgets() const {
        for (const auto& entry : budgets) {
            cout << "Budgets for ";
            switch (entry.first) {
            case DAILY: cout << "Daily:" << endl; break;
            case WEEKLY: cout << "Weekly:" << endl; break;
            case MONTHLY: cout << "Monthly:" << endl; break;
            }
            for (const auto& budget : entry.second) {
                cout << "  - Amount: $" << budget.getAmount() << endl;
            }
        }
    }

private:
    string name;
    map<BudgetType, vector<Budget>> budgets;
};

// convert lang 
BudgetType stringToBudgetType(const string& type) {
    if (type == "daily") return DAILY;
    if (type == "weekly") return WEEKLY;
    if (type == "monthly") return MONTHLY;
    throw invalid_argument("Invalid budget type");
}

void userAccountMenu(User& user) {
    while (true) {
        cout << "\nUser Account Menu:" << endl;
        cout << "1. Add Budget" << endl;
        cout << "2. Modify Budget" << endl;
        cout << "3. Display All Budgets" << endl;
        cout << "4. Back to Main Menu" << endl;
        cout << "Choose an option: ";
        int choice;
        cin >> choice;

        if (choice == 4) break;

        switch (choice) {
        case 1: {
            cout << "Enter budget type (daily, weekly, monthly): ";
            string type;
            cin >> type;
            try {
                BudgetType budgetType = stringToBudgetType(type);
                cout << "Enter amount: ";
                double amount;
                cin >> amount;
                user.addBudget(amount, budgetType);
            }
            catch (const invalid_argument& e) {
                cout << e.what() << endl;
            }
            break;
        }
        case 2: {
            cout << "Enter budget type (daily, weekly, monthly): ";
            string type;
            cin >> type;
            try {
                BudgetType budgetType = stringToBudgetType(type);
                cout << "Enter amount to add or deduct: ";
                double amount;
                cin >> amount;
                user.modifyBudget(amount, budgetType);
            }
            catch (const invalid_argument& e) {
                cout << e.what() << endl;
            }
            break;
        }
        case 3: {
            user.showBudgets();
            break;
        }
        default: {
            cout << "Invalid option. Please try again." << endl;
        }
        }
    }
}

int main() {
    string userName;
    cout << "Enter user name: ";
    getline(cin, userName);

    User user(userName);

    while (true) {
        cout << "\nCoreKeeper Main Menu:" << endl;
        cout << "1. User Account" << endl;
        cout << "2. Budget Allocation" << endl;
        cout << "3. Financial Advisor" << endl;
        cout << "4. Exit Program" << endl;
        cout << "Choose an option: ";
        int choice;
        cin >> choice;

        if (choice == 4) break;

        switch (choice) {
        case 1:
            userAccountMenu(user);
            break;
        case 2:
            cout << "Budget Allocation option selected. (wala pa)" << endl;
            break;
        case 3:
            cout << "Financial Advisor option selected. (di koalam wala pa)" << endl;
            break;
        default:
            cout << "Invalid option. Please try again." << endl;
        }
    }

    return 0;
}

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set> 

using namespace std;

enum BudgetType { DAILY, WEEKLY, MONTHLY };

class Budget {
public:
    Budget() : amount(0.0), type(DAILY) {} 
    Budget(double amount, BudgetType type)
        : amount(amount), type(type) {}

    double getAmount() const { return amount; }
    BudgetType getType() const { return type; }

    void setAmount(double newAmount) { amount = newAmount; }

private:
    double amount;
    BudgetType type;
};

class Expense {
public:
    Expense(const string& category, double amount, BudgetType type)
        : category(category), amount(amount), type(type) {}

    string getCategory() const { return category; }
    double getAmount() const { return amount; }
    BudgetType getType() const { return type; }

private:
    string category;
    double amount;
    BudgetType type;
};

class User {
public:
    User(const string& name)
        : name(name) {
        initializeDefaultCategories();
    }

    void addBudget(double amount, BudgetType type) {
        if (budgets.find(type) != budgets.end()) {
            cout << "Budget for this type already exists. Please modify the existing budget." << endl;
        }
        else {
            budgets[type] = Budget(amount, type);
            cout << "Budget added successfully." << endl;
        }
    }

    void modifyBudget(double amount, BudgetType type) {
        if (budgets.find(type) != budgets.end()) {
            budgets[type].setAmount(amount);
            cout << "Budget modified successfully." << endl;
        }
        else {
            cout << "No budget found for the given type to modify. Please add a budget first." << endl;
        }
    }

    void deleteBudget(BudgetType type) {
        if (budgets.find(type) != budgets.end()) {
            budgets.erase(type);
            cout << "Budget deleted successfully." << endl;
        }
        else {
            cout << "No budget found for the given type to delete." << endl;
        }
    }

    void showBudgets() const {
        if (budgets.empty()) {
            cout << "No budgets available." << endl;
            return;
        }

        for (const auto& entry : budgets) {
            cout << "Budgets for ";
            switch (entry.first) {
            case DAILY: cout << "Daily:" << endl; break;
            case WEEKLY: cout << "Weekly:" << endl; break;
            case MONTHLY: cout << "Monthly:" << endl; break;
            }
            cout << "  - Amount: $" << entry.second.getAmount() << endl;
        }
    }

    void displayAvailableBudgets() const {
        if (budgets.empty()) {
            cout << "No budgets available to modify or delete." << endl;
            return;
        }

        cout << "Available budgets:" << endl;
        for (const auto& entry : budgets) {
            switch (entry.first) {
            case DAILY: cout << "  - Daily" << endl; break;
            case WEEKLY: cout << "  - Weekly" << endl; break;
            case MONTHLY: cout << "  - Monthly" << endl; break;
            }
        }
    }

    void addCategory(const string& category) {
        categories.insert(category);
        cout << "Category added successfully." << endl;
    }

    void addExpense(const string& category, double amount, BudgetType type) {
        if (categories.find(category) == categories.end()) {
            cout << "Category does not exist. Please add the category first." << endl;
            return;
        }
        expenses.emplace_back(category, amount, type);
        cout << "Expense added successfully." << endl;
    }

    void showExpenses() const {
        if (expenses.empty()) {
            cout << "No expenses recorded." << endl;
            return;
        }

        for (const auto& expense : expenses) {
            cout << "Expense for ";
            switch (expense.getType()) {
            case DAILY: cout << "Daily"; break;
            case WEEKLY: cout << "Weekly"; break;
            case MONTHLY: cout << "Monthly"; break;
            }
            cout << " - Category: " << expense.getCategory() << ", Amount: $" << expense.getAmount() << endl;
        }
    }

private:
    //YUNG MGA INITIALIZED NA CATEGORIES PWEDE PA DAGDAGAN
    void initializeDefaultCategories() {
        categories = { "Food", "Transportation", "Electricity Bills", "Rent", "Entertainment" };
    }

    string name;
    map<BudgetType, Budget> budgets;
    set<string> categories;
    vector<Expense> expenses;
};

BudgetType stringToBudgetType(const string& type) {
    if (type == "daily") return DAILY;
    if (type == "weekly") return WEEKLY;
    if (type == "monthly") return MONTHLY;
    throw invalid_argument("Invalid budget type");
}

void budgetMenu(User& user) {
    while (true) {
        cout << "\nBudget Menu:" << endl;
        cout << "1. Add Budget" << endl;
        cout << "2. Modify Budget" << endl;
        cout << "3. Delete Budget" << endl;
        cout << "4. Display All Budgets" << endl;
        cout << "5. Back to User Account Menu" << endl;
        cout << "Choose an option: ";
        int choice;
        cin >> choice;

        if (choice == 5) break;

        switch (choice) {
        case 1: {
            cout << "\nADD BUDGET\n";
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
            cout << "\nMODIFY BUDGET\n";
            user.displayAvailableBudgets();
            cout << "Enter budget type to modify (daily, weekly, monthly): ";
            string type;
            cin >> type;
            try {
                BudgetType budgetType = stringToBudgetType(type);
                cout << "Enter new amount: ";
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
            cout << "\nDELETE BUDGET\n";
            user.displayAvailableBudgets();
            cout << "Enter budget type to delete (daily, weekly, monthly): ";
            string type;
            cin >> type;
            try {
                BudgetType budgetType = stringToBudgetType(type);
                user.deleteBudget(budgetType);
            }
            catch (const invalid_argument& e) {
                cout << e.what() << endl;
            }
            break;
        }
        case 4: {
            user.showBudgets();
            break;
        }
        default: {
            cout << "Invalid option. Please try again." << endl;
        }
        }
    }
}

void expenseMenu(User& user) {
    while (true) {
        cout << "\nExpense Menu:" << endl;
        cout << "1. Add Expense Category" << endl;
        cout << "2. Add Expense" << endl;
        cout << "3. Display All Expenses" << endl;
        cout << "4. Back to User Account Menu" << endl;
        cout << "Choose an option: ";
        int choice;
        cin >> choice;

        if (choice == 4) break;

        switch (choice) {
        case 1: {
            cout << "\nADD EXPENSE CATEGORY\n";
            cout << "Enter new category: ";
            string category;
            cin.ignore();
            getline(cin, category);
            user.addCategory(category);
            break;
        }
        case 2: {
            cout << "\nADD EXPENSE\n";
            cout << "Enter budget type (daily, weekly, monthly): ";
            string type;
            cin >> type;
            try {
                BudgetType budgetType = stringToBudgetType(type);
                cout << "Enter expense category: ";
                string category;
                cin.ignore();
                getline(cin, category);
                cout << "Enter amount: ";
                double amount;
                cin >> amount;
                user.addExpense(category, amount, budgetType);
            }
            catch (const invalid_argument& e) {
                cout << e.what() << endl;
            }
            break;
        }
        case 3: {
            user.showExpenses();
            break;
        }
        default: {
            cout << "Invalid option. Please try again." << endl;
        }
        }
    }
}

void userAccountMenu(User& user) {
    while (true) {
        cout << "\nUser Account Menu:" << endl;
        cout << "1. Budget (yung pag didislplay pwede pa pagandahin)" << endl;
        cout << "2. Expense (medyo magulo pa pre tsaka yung display pwede pa mapaganda)" << endl;
        cout << "3. List Totals (wala pa)" << endl;
        cout << "4. Back to Main Menu" << endl;
        cout << "Choose an option: ";
        int choice;
        cin >> choice;

        if (choice == 4) break;

        switch (choice) {
        case 1:
            budgetMenu(user);
            break;
        case 2:
            expenseMenu(user);
            break;
        case 3:
            cout << "List Totals option selected. (ala pa)" << endl;
            break;
        default:
            cout << "Invalid option. Please try again." << endl;
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
            cout << "Budget Allocation option selected. (di pa gawa)" << endl;
            break;
        case 3:
            cout << "Financial Advisor option selected. (ala pa pre)" << endl;
            break;
        default:
            cout << "Invalid option. Please try again." << endl;
        }
    }

    return 0;
}

#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <limits>

using namespace std;

void loadCategories(map<string, double>& dailyAllocations, map<string, double>& weeklyAllocations, map<string, double>& monthlyAllocations) {
    ifstream infile("expenseCategory.txt");
    if (!infile) {
        cerr << "Could not open the file!" << endl;
        return;
    }

    string type, category;
    double amount;
    while (infile >> type >> category) {
        if (type == "daily") {
            dailyAllocations[category] = 0.0;
        }
        else if (type == "weekly") {
            weeklyAllocations[category] = 0.0;
        }
        else if (type == "monthly") {
            monthlyAllocations[category] = 0.0;
        }
    }
    infile.close();
}

void saveCategories(const map<string, double>& dailyAllocations, const map<string, double>& weeklyAllocations, const map<string, double>& monthlyAllocations) {
    ofstream outfile("expenseCategory.txt");
    if (!outfile) {
        cerr << "Could not open the file for writing!" << endl;
        return;
    }

    for (const auto& pair : dailyAllocations) {
        outfile << "daily " << pair.first << endl;
    }
    for (const auto& pair : weeklyAllocations) {
        outfile << "weekly " << pair.first << endl;
    }
    for (const auto& pair : monthlyAllocations) {
        outfile << "monthly " << pair.first << endl;
    }

    outfile.close();
}

void displayStatus(const string& username, double monthlyBudget, double remainingMonthlyBudget, double remainingWeeklyBudget, double remainingDailyBudget, double savings) {
    cout << "\n                              ____      _       _  __                         " << endl;
    cout << "                             / ___|___ (_)_ __ | |/ /___  ___ _ __   ___ _ __ " << endl;
    cout << "                             | |  / _ \\| | '_ \\| ' // _ \\/ _ \\ '_ \\ / _ \\ '__| " << endl;
    cout << "                             | |__|(_) | | | | | . \\  __ / __/ |_) |  __/  | " << endl;
    cout << "                             \\____\\___/|_|_| |_|_|\\_\\___|\\___| ___/ \\___ |_| " << endl;
    cout << "                                                             |_| " << endl;
    cout << "\nUser: " << username << endl;
    cout << "--------------------------------------------" << endl;
    cout << "| Total Monthly Budget:         " << setw(10) << right << fixed << setprecision(2) << monthlyBudget << " |" << endl;
    cout << "| Remaining Monthly Budget:     " << setw(10) << right << fixed << setprecision(2) << remainingMonthlyBudget << " |" << endl;
    cout << "| Remaining Weekly Budget:      " << setw(10) << right << fixed << setprecision(2) << remainingWeeklyBudget << " |" << endl;
    cout << "| Remaining Daily Budget:       " << setw(10) << right << fixed << setprecision(2) << remainingDailyBudget << " |" << endl;
    cout << "| Savings:                      " << setw(10) << right << fixed << setprecision(2) << savings << " |" << endl;
    cout << "--------------------------------------------" << endl;
    cout << endl;
}

void displayMenu() {
    cout << "\n\n-------------------------------------- M  A  I  N    M  E  N  U ---------------------------------------" << endl << endl;
    cout << "\n        S i m u l a t e           F i n a n c e  M a n a g e r        F i n a n c e  A d v i s o r" << endl;
    cout << "        1. End/Start Day          4. Budget Management                7. Financial Advice" << endl;
    cout << "        2. End/Start Week         5. Expenses/Allocations             8. Modify Savings" << endl;
    cout << "        3. End/Start Month        6. Add/Remove Expense Category" << endl;
    cout << "\n                                               9. Exit" << endl;
    cout << "\n                                          Enter your choice: ";
}

void displayTable(const string& title, const map<string, double>& dailyAllocations, double remainingDailyBudget,
    const map<string, double>& weeklyAllocations, double remainingWeeklyBudget,
    const map<string, double>& monthlyAllocations, double remainingMonthlyBudget) {
    cout << title << endl;

    cout << "Daily Budget Allocations           Weekly Budget Allocations          Monthly Budget Allocations" << endl;
    cout << "|-------------------------------|  |-------------------------------|  |-------------------------------|" << endl;
    cout << "|     Category     |  Amount    |  |     Category     |  Amount    |  |     Category     |  Amount    |" << endl;
    cout << "|-------------------------------|  |-------------------------------|  |-------------------------------|" << endl;

    auto dailyIt = dailyAllocations.begin();
    auto weeklyIt = weeklyAllocations.begin();
    auto monthlyIt = monthlyAllocations.begin();

    while (dailyIt != dailyAllocations.end() || weeklyIt != weeklyAllocations.end() || monthlyIt != monthlyAllocations.end()) {
        // Daily Allocation Row
        if (dailyIt != dailyAllocations.end()) {
            cout << "| " << setw(16) << left << dailyIt->first << " | " << setw(10) << right << fixed << setprecision(2) << dailyIt->second << " |";
            dailyIt++;
        }
        else {
            cout << "|                             |            |";
        }

        // Weekly Allocation Row
        if (weeklyIt != weeklyAllocations.end()) {
            cout << "  | " << setw(16) << left << weeklyIt->first << " | " << setw(10) << right << fixed << setprecision(2) << weeklyIt->second << " |";
            weeklyIt++;
        }
        else {
            cout << "  |                             |            |";
        }

        // Monthly Allocation Row
        if (monthlyIt != monthlyAllocations.end()) {
            cout << "  | " << setw(16) << left << monthlyIt->first << " | " << setw(10) << right << fixed << setprecision(2) << monthlyIt->second << " |" << endl;
            monthlyIt++;
        }
        else {
            cout << "  |                             |            |" << endl;
        }
    }

    cout << "|-------------------------------|  |-------------------------------|  |-------------------------------|" << endl;

    double totalDailyExpenses = 0.0;
    for (const auto& alloc : dailyAllocations) {
        totalDailyExpenses += alloc.second;
    }

    double totalWeeklyExpenses = 0.0;
    for (const auto& alloc : weeklyAllocations) {
        totalWeeklyExpenses += alloc.second;
    }

    double totalMonthlyExpenses = 0.0;
    for (const auto& alloc : monthlyAllocations) {
        totalMonthlyExpenses += alloc.second;
    }

    cout << "| Total Expenses   | " << setw(10) << right << fixed << setprecision(2) << totalDailyExpenses << " |";
    cout << "  | Total Expenses   | " << setw(10) << right << fixed << setprecision(2) << totalWeeklyExpenses << " |";
    cout << "  | Total Expenses   | " << setw(10) << right << fixed << setprecision(2) << totalMonthlyExpenses << " |" << endl;

    cout << "| Remaining Budget | " << setw(10) << right << fixed << setprecision(2) << remainingDailyBudget << " |";
    cout << "  | Remaining Budget | " << setw(10) << right << fixed << setprecision(2) << remainingWeeklyBudget << " |";
    cout << "  | Remaining Budget | " << setw(10) << right << fixed << setprecision(2) << remainingMonthlyBudget << " |" << endl;

    cout << "|-------------------------------|  |-------------------------------|  |-------------------------------|" << endl;
}

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void startEndDay(double& remainingDailyBudget, double& remainingWeeklyBudget, double& remainingMonthlyBudget, double& savings, map<string, double>& dailyAllocations) {
    double totalAllocations = 0.0;

    for (const auto& alloc : dailyAllocations) {
        totalAllocations += alloc.second;
    }

    if (remainingDailyBudget > 0) {
        char choice;
        cout << "Day ended. Remaining daily budget: " << remainingDailyBudget << endl;
        do {
            cout << "Do you want to add the remaining daily budget to savings or return it to the weekly budget? (s/r): ";
            cin >> choice;
            if (choice != 's' && choice != 'S' && choice != 'r' && choice != 'R') {
                cout << "Invalid input. Please enter 's' or 'r'." << endl;
                clearInput();
            }
        } while (choice != 's' && choice != 'S' && choice != 'r' && choice != 'R');

        if (choice == 's' || choice == 'S') {
            savings += remainingDailyBudget;
            cout << "Added to savings." << endl;
        }
        else {
            remainingWeeklyBudget += remainingDailyBudget;
            cout << "Returned to weekly budget." << endl;
        }
    }

    remainingDailyBudget = 0.0;
    for (auto& alloc : dailyAllocations) {
        alloc.second = 0.0;
    }

    cout << "Enter today's budget: ";
    double newDailyBudget;
    while (!(cin >> newDailyBudget) || newDailyBudget < 0 || newDailyBudget > remainingWeeklyBudget) {
        cout << "Daily Budget must be positive and within the Weekly Budget: ";
        clearInput();
    }
    remainingWeeklyBudget += remainingDailyBudget; // Restore previous daily budget to weekly budget
    remainingDailyBudget = newDailyBudget;
    remainingWeeklyBudget -= newDailyBudget; // Deduct new daily budget from weekly budget
    cout << "New day started with budget: " << remainingDailyBudget << endl;
    cout << "\nPress Enter to return to the main menu...\n";
    cin.ignore();
    cin.get();
}

void startEndWeek(double& remainingDailyBudget, double& remainingWeeklyBudget, double& remainingMonthlyBudget, double& savings, map<string, double>& dailyAllocations, map<string, double>& weeklyAllocations) {
    double totalAllocations = 0.0;

    for (const auto& alloc : weeklyAllocations) {
        totalAllocations += alloc.second;
    }

    if (remainingDailyBudget > 0) {
        cout << "Remaining Daily Budget: " << remainingDailyBudget << endl;
        cout << "Remaining Daily Budget added to Remaining Weekly Budget." << endl;
        remainingWeeklyBudget += remainingDailyBudget;
        remainingDailyBudget = 0.0;
    }
    if (remainingWeeklyBudget > 0) {
        char choice;
        cout << "Week ended. Remaining weekly budget: " << remainingWeeklyBudget << endl;
        do {
            cout << "Do you want to add the remaining weekly budget to savings or return it to the monthly budget? (s/r): ";
            cin >> choice;
            if (choice != 's' && choice != 'S' && choice != 'r' && choice != 'R') {
                cout << "Invalid input. Please enter 's' or 'r'." << endl;
                clearInput();
            }
        } while (choice != 's' && choice != 'S' && choice != 'r' && choice != 'R');

        if (choice == 's' || choice == 'S') {
            savings += remainingWeeklyBudget;
            cout << "Added to savings." << endl;
        }
        else {
            remainingMonthlyBudget += remainingWeeklyBudget;
            cout << "Returned to monthly budget." << endl;
        }
    }

    remainingWeeklyBudget = 0.0;
    for (auto& alloc : dailyAllocations) {
        alloc.second = 0.0;
    }

    for (auto& alloc : weeklyAllocations) {
        alloc.second = 0.0;
    }

    cout << "Enter new Weekly budget: ";
    double newWeeklyBudget;
    while (!(cin >> newWeeklyBudget) || newWeeklyBudget < 0 || newWeeklyBudget > remainingMonthlyBudget) {
        cout << "Weekly Budget must be positive and within the Monthly Budget: ";
        clearInput();
    }
    remainingMonthlyBudget += remainingWeeklyBudget; // Restore previous weekly budget to monthly budget
    remainingWeeklyBudget = newWeeklyBudget;
    remainingMonthlyBudget -= newWeeklyBudget; // Deduct new weekly budget from monthly budget

    cout << "Enter new Daily Budget: ";
    double newDailyBudget;
    while (!(cin >> newDailyBudget) || newDailyBudget < 0 || newDailyBudget > remainingWeeklyBudget) {
        cout << "Daily Budget must be positive and within the Weekly Budget: ";
        clearInput();
    }
    remainingWeeklyBudget += remainingDailyBudget; // Restore previous daily budget to weekly budget
    remainingDailyBudget = newDailyBudget;
    remainingWeeklyBudget -= newDailyBudget; // Deduct new daily budget from weekly budget
    cout << "New week started with New Weekly Budget: " << remainingWeeklyBudget << endl;
    cout << "New Daily Budget: " << remainingDailyBudget << endl;
    cout << "\nPress Enter to return to the main menu...";
    cin.ignore();
    cin.get();
}

void startEndMonth(double& monthlyBudget, double& remainingMonthlyBudget, double& remainingWeeklyBudget, double& remainingDailyBudget, double& savings, map<string, double>& dailyAllocations, map<string, double>& weeklyAllocations, map<string, double>& monthlyAllocations) {
    double totalAllocations = 0.0;

    for (const auto& alloc : monthlyAllocations) {
        totalAllocations += alloc.second;
    }

    if (remainingDailyBudget > 0) {
        cout << "Remaining Daily Budget: " << remainingDailyBudget << endl;
        cout << "Remaining Daily Budget added to Remaining Monthly Budget." << endl;
        remainingMonthlyBudget += remainingDailyBudget;
        remainingDailyBudget = 0.0;
    }
    if (remainingWeeklyBudget > 0) {
        cout << "Remaining Weekly Budget: " << remainingWeeklyBudget << endl;
        cout << "Remaining Weekly Budget added to Remaining Monthly Budget." << endl;
        remainingMonthlyBudget += remainingWeeklyBudget;
        remainingWeeklyBudget = 0.0;
    }
    if (remainingMonthlyBudget > 0) {
        char choice;
        cout << "Month ended. Remaining monthly budget: " << remainingMonthlyBudget << endl;
        do {
            cout << "Do you want to add the remaining monthly budget to savings? (y/n): ";
            cin >> choice;
            if (choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N') {
                cout << "Invalid input. Please enter 'y' or 'n'." << endl;
                clearInput();
            }
        } while (choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N');

        if (choice == 'y' || choice == 'Y') {
            savings += remainingMonthlyBudget;
            cout << "Added to savings." << endl;
        }
    }

    double newMonthlyBudget = 0.0;

    cout << "Choose an option for budget allocation:\n";
    cout << "1. Automate Budget Allocation\n";
    cout << "2. Set Budget Manually\n";
    cout << "Enter your choice (1/2): ";
    int allocationChoice;
    while (!(cin >> allocationChoice) || (allocationChoice != 1 && allocationChoice != 2)) {
        cout << "Invalid choice. Please enter 1 or 2: ";
        clearInput();
    }

    if (allocationChoice == 1) {
        cout << "Enter your total Monthly Budget: ";
        while (!(cin >> newMonthlyBudget) || newMonthlyBudget < 0) {
            cout << "Invalid input. Please enter a positive number for the budget: ";
            clearInput();
        }
        char addSavingsToBudget;
        do {
            cout << "Do you want to add your savings to the new monthly budget? (y/n): ";
            cin >> addSavingsToBudget;
            if (addSavingsToBudget != 'y' && addSavingsToBudget != 'Y' && addSavingsToBudget != 'n' && addSavingsToBudget != 'N') {
                cout << "Invalid input. Please enter 'y' or 'n'." << endl;
                clearInput();
            }
        } while (addSavingsToBudget != 'y' && addSavingsToBudget != 'Y' && addSavingsToBudget != 'n' && addSavingsToBudget != 'N');

        if (addSavingsToBudget == 'y' || addSavingsToBudget == 'Y') {
            cout << "Adding savings to the new monthly budget." << endl;
            newMonthlyBudget += savings;
            savings = 0.0; // Reset savings after adding to budget
            cout << "New Monthly Budget: " << newMonthlyBudget << endl;
        }
        else {
            cout << "Savings will not be added to the new monthly budget." << endl;
        }
        remainingMonthlyBudget = newMonthlyBudget;
        remainingWeeklyBudget = newMonthlyBudget / 4;
        remainingMonthlyBudget -= remainingWeeklyBudget;
        remainingDailyBudget = newMonthlyBudget / 30;
        remainingWeeklyBudget -= remainingDailyBudget;
    }
    else if (allocationChoice == 2) {
        cout << "Enter your total monthly budget: ";
        while (!(cin >> newMonthlyBudget) || newMonthlyBudget < 0) {
            cout << "Please enter a positive number for the monthly budget: ";
            clearInput();
        }
        char addSavingsToBudget;
        do {
            cout << "Do you want to add your savings to the new monthly budget? (y/n): ";
            cin >> addSavingsToBudget;
            if (addSavingsToBudget != 'y' && addSavingsToBudget != 'Y' && addSavingsToBudget != 'n' && addSavingsToBudget != 'N') {
                cout << "Invalid input. Please enter 'y' or 'n'." << endl;
                clearInput();
            }
        } while (addSavingsToBudget != 'y' && addSavingsToBudget != 'Y' && addSavingsToBudget != 'n' && addSavingsToBudget != 'N');

        if (addSavingsToBudget == 'y' || addSavingsToBudget == 'Y') {
            cout << "Adding savings to the new monthly budget." << endl;
            newMonthlyBudget += savings;
            savings = 0.0; // Reset savings after adding to budget
            cout << "New Monthly Budget: " << newMonthlyBudget << endl;
        }
        else {
            cout << "Savings will not be added to the new monthly budget." << endl;
        }
        cout << "Enter your weekly budget: ";
        while (!(cin >> remainingWeeklyBudget) || remainingWeeklyBudget < 0 || remainingWeeklyBudget > newMonthlyBudget) {
            cout << "Weekly Budget must be positive and within the Monthly Budget: ";
            clearInput();
        }

        cout << "Enter your daily budget: ";
        while (!(cin >> remainingDailyBudget) || remainingDailyBudget < 0 || remainingDailyBudget > remainingWeeklyBudget) {
            cout << "Daily Budget must be positive and within the Weekly Budget: ";
            clearInput();
        }
        remainingMonthlyBudget = newMonthlyBudget;
        remainingMonthlyBudget -= remainingWeeklyBudget;
        remainingWeeklyBudget -= remainingDailyBudget;
    }

    for (auto& alloc : dailyAllocations) {
        alloc.second = 0.0;
    }
    for (auto& alloc : weeklyAllocations) {
        alloc.second = 0.0;
    }
    for (auto& alloc : monthlyAllocations) {
        alloc.second = 0.0;
    }

    cout << "New month started with budget: " << newMonthlyBudget << endl;
    cout << "\nPress Enter to return to the main menu...";
    cin.ignore();
    cin.get();
}

void budgetManagement(double& monthlyBudget, double& remainingMonthlyBudget, double& remainingWeeklyBudget, double& remainingDailyBudget) {
    int choice;
    cout << "Budget Management:" << endl;
    cout << "1. Modify Total Monthly Budget" << endl;
    cout << "2. Modify Weekly Budget" << endl;
    cout << "3. Modify Daily Budget" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
    case 1:
        cout << "Enter new monthly budget: ";
        cin >> monthlyBudget;
        remainingMonthlyBudget = monthlyBudget;
        remainingWeeklyBudget = monthlyBudget / 4;
        remainingMonthlyBudget -= remainingWeeklyBudget;
        remainingDailyBudget = monthlyBudget / 30;
        remainingWeeklyBudget -= remainingDailyBudget;
        cout << "\nNew Monthly Budget now set as: " << monthlyBudget << endl;
        cout << "\nPress Enter to return to the main menu...";
        cin.ignore();
        cin.get();
        break;
    case 2:
        double newWeeklyBudget;
        cout << "Enter new weekly budget: ";
        cin >> newWeeklyBudget;
        remainingMonthlyBudget += remainingWeeklyBudget + remainingDailyBudget; // Restore previous weekly budget to monthly budget
        remainingWeeklyBudget = newWeeklyBudget;
        remainingMonthlyBudget -= newWeeklyBudget; // Deduct new weekly budget from monthly budget
        remainingWeeklyBudget -= remainingDailyBudget;
        if (newWeeklyBudget > remainingMonthlyBudget) {
            cout << "\nThe new Weekly Budget cannot exceed the remaining Monthly Budget!" << endl;
            cout << "\nPress Enter to return to the main menu...";
            cin.ignore();
            cin.get();
        }
        else {
            cout << "\nNew Weekly Budget now set as: " << newWeeklyBudget << endl;
            cout << "\nPress Enter to return to the main menu...";
            cin.ignore();
            cin.get();
        }
        break;
    case 3:
        double newDailyBudget;
        cout << "Enter new daily budget: ";
        cin >> newDailyBudget;
        remainingWeeklyBudget += remainingDailyBudget; // Restore previous daily budget to weekly budget
        remainingDailyBudget = newDailyBudget;
        remainingWeeklyBudget -= newDailyBudget; // Deduct new daily budget from weekly budget
        if (newDailyBudget > remainingWeeklyBudget) {
            cout << "\nThe new Daily Budget cannot exceed the remaining Weekly Budget!" << endl;
            cout << "\nPress Enter to return to the main menu...";
            cin.ignore();
            cin.get();
        }
        else {
            cout << "\nNew Daily Budget now set as: " << newDailyBudget << endl;
            cout << "\nPress Enter to return to the main menu...";
            cin.ignore();
            cin.get();
        }
        break;
    default:
        cout << "Invalid choice." << endl;
        cout << "\nPress Enter to return to the main menu...";
        cin.ignore();
        cin.get();
        break;
    }
}

void expensesAllocations(double& remainingDailyBudget, double& remainingWeeklyBudget, double& remainingMonthlyBudget,
    map<string, double>& dailyAllocations, map<string, double>& weeklyAllocations, map<string, double>& monthlyAllocations) {
    int choice;
    cout << "Expense Types:" << endl;
    cout << "1. Daily" << endl;
    cout << "2. Weekly" << endl;
    cout << "3. Monthly" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    vector<string> categories;
    string category;
    double amount;

    switch (choice) {
    case 1:
        for (const auto& alloc : dailyAllocations) {
            categories.push_back(alloc.first);
        }
        break;
    case 2:
        for (const auto& alloc : weeklyAllocations) {
            categories.push_back(alloc.first);
        }
        break;
    case 3:
        for (const auto& alloc : monthlyAllocations) {
            categories.push_back(alloc.first);
        }
        break;
    default:
        cout << "Invalid choice." << endl;
        cout << "\nPress Enter to return to the main menu...";
        cin.ignore();
        cin.get();
        return;
    }

    cout << "Select a category by number:" << endl;
    for (size_t i = 0; i < categories.size(); ++i) {
        cout << i + 1 << ". " << categories[i] << endl;
    }

    int categoryIndex;
    cout << "Enter category number: ";
    cin >> categoryIndex;

    if (categoryIndex < 1 || categoryIndex > categories.size()) {
        cout << "\nInvalid category number." << endl;
        cout << "Press Enter to return to the main menu..." << endl;
        cin.ignore();
        cin.get();
        return;
    }

    category = categories[categoryIndex - 1];
    cout << "Enter amount: ";
    cin >> amount;

    switch (choice) {
    case 1:
        if (amount > remainingDailyBudget) {
            cout << "Expense amount cannot exceed set Daily Budget.\n";
            cout << "If you wish to add to your Daily Budget, modify the Daily Budget first.\n";
        }
        else if (remainingDailyBudget == 0) {
            cout << "Warning: No Daily Budget Remaining!" << endl;
        }
        else {
            dailyAllocations[category] += amount;
            remainingDailyBudget -= amount;
        }
        break;
    case 2:
        if (amount > remainingWeeklyBudget) {
            cout << "Expense amount cannot exceed set Weekly Budget.\n";
            cout << "If you wish to add to your Weekly Budget, modify the Weekly Budget first.\n";
        }
        else if (remainingWeeklyBudget == 0) {
            cout << "Warning: No Weekly Budget Remaining!" << endl;
        }
        else {
            weeklyAllocations[category] += amount;
            remainingWeeklyBudget -= amount;
        }
        break;
    case 3:
        if (amount > remainingMonthlyBudget) {
            cout << "Expense amount cannot exceed set Monthly Budget.\n";
            cout << "If you wish to add to your Monthly Budget, modify the Monthly Budget first.\n";
        }
        else if (remainingMonthlyBudget == 0) {
            cout << "Warning: No Monthly Budget Remaining!" << endl;
        }
        else {
            monthlyAllocations[category] += amount;
            remainingMonthlyBudget -= amount;
        }
        break;
    default:
        cout << "Invalid choice." << endl;
        break;
    }

    cout << "\nPress Enter to return to the main menu...";
    cin.ignore();
    cin.get();
}

void addExpenseCategory(map<string, double>& dailyAllocations, map<string, double>& weeklyAllocations, map<string, double>& monthlyAllocations) {
    cout << "Expense Types: " << endl;
    cout << "1. Daily" << endl;
    cout << "2. Weekly" << endl;
    cout << "3. Monthly" << endl;

    int type;
    cout << "Enter expense type: ";
    cin >> type;

    string category;
    cout << "Enter new category name: ";
    cin >> category;

    switch (type) {
    case 1:
        if (dailyAllocations.find(category) == dailyAllocations.end()) {
            dailyAllocations[category] = 0.0;
            cout << "Category added: " << category << endl;
            cout << "\nPress Enter to return to the main menu...";
            cin.ignore();
            cin.get();
        }
        else {
            cout << "Category already exists." << endl;
            cout << "\nPress Enter to return to the main menu...";
            cin.ignore();
            cin.get();
        }
        break;
    case 2:
        if (weeklyAllocations.find(category) == weeklyAllocations.end()) {
            weeklyAllocations[category] = 0.0;
            cout << "Category added: " << category << endl;
            cout << "\nPress Enter to return to the main menu...";
            cin.ignore();
            cin.get();
        }
        else {
            cout << "Category already exists." << endl;
            cout << "\nPress Enter to return to the main menu...";
            cin.ignore();
            cin.get();
        }
        break;
    case 3:
        if (monthlyAllocations.find(category) == monthlyAllocations.end()) {
            monthlyAllocations[category] = 0.0;
            cout << "Category added: " << category << endl;
            cout << "\nPress Enter to return to the main menu...";
            cin.ignore();
            cin.get();
        }
        else {
            cout << "Category already exists." << endl;
            cout << "\nPress Enter to return to the main menu...";
            cin.ignore();
            cin.get();
        }
        break;
    default:
        cout << "Invalid expense type." << endl;
        cout << "\nPress Enter to return to the main menu...";
        cin.ignore();
        cin.get();
        return;
    }

    saveCategories(dailyAllocations, weeklyAllocations, monthlyAllocations);
}

void removeExpenseCategory(map<string, double>& dailyAllocations, map<string, double>& weeklyAllocations, map<string, double>& monthlyAllocations) {
    cout << "Expense Types: " << endl;
    cout << "1. Daily" << endl;
    cout << "2. Weekly" << endl;
    cout << "3. Monthly" << endl;

    int type;
    cout << "Enter expense type: ";
    cin >> type;

    string category;
    cout << "Enter category name to remove: ";
    cin >> category;

    bool categoryExists = false;
    switch (type) {
    case 1:
        if (dailyAllocations.erase(category)) {
            categoryExists = true;
        }
        break;
    case 2:
        if (weeklyAllocations.erase(category)) {
            categoryExists = true;
        }
        break;
    case 3:
        if (monthlyAllocations.erase(category)) {
            categoryExists = true;
        }
        break;
    default:
        cout << "Invalid expense type." << endl;
        cout << "\nPress Enter to return to the main menu...";
        cin.ignore();
        cin.get();
        return;
    }

    if (categoryExists) {
        cout << "Category removed: " << category << endl;
        cout << "\nPress Enter to return to the main menu...";
        cin.ignore();
        cin.get();
    }
    else {
        cout << "Category does not exist." << endl;
        cout << "\nPress Enter to return to the main menu...";
        cin.ignore();
        cin.get();
    }

    saveCategories(dailyAllocations, weeklyAllocations, monthlyAllocations);
}

void detailedAdvice() {
    cout << "\n\n               50 - 20 - 30 RULE FOR SAVING MONEY\n"
        << "                 by  PHILIPPINE NATIONAL BANK\n"
        << "--------------------------------------------------------------------\n\n"
        << "50% of your money should be allocated for needs. This is the most \n"
        << "important expense vertical that needs to be met. A 50% allocation \n"
        << "is given to this because these are items/products/payables that \n"
        << "enable us to go on with ourdaily lives.\n" << endl;

    cout << "While 20% of your money is portioned for savings. It is never too \n"
        << "early to start building up your savings funds. You may be hearing \n"
        << "a lot about emergency funds as this is something that you must also \n"
        << "prioritize to help you secure your future and keep you out of debt. \n"
        << "Savings can also mean it is for money management such as savings \n"
        << "for debit payments. This is a good tip that can help you avoid \n"
        << "drowning in debt as well. Another way of saving could also be\n"
        << "allocated for milestones such as retirement. But overall 20% of \n"
        << "all your money shouldgo to these savings items/goals.\n" << endl;

    cout << "Then 30% of your money goes to our favorite segment, our wants.\n"
        << "These include the thingswe desire and not necessarily what we\n"
        << "need. These include your favorite food from fancyrestaurants,\n"
        << "high-end gadgets, and alike. Though its okay to spend a few\n"
        << "luxuries every once in a while, your priority should still be the \n"
        << "portion for needs and savings. To better manage your income, one \n"
        << "should know the delineation of needs and wants.Then, you can keep\n"
        << "track of your spending and more importantly where your money goes.\n" << endl;

    cout << "If you want to be able to enjoy the most of your hard-earned money, \n"
        << "this is the way to go.The big picture here is that this way of \n"
        << "saving helps you to live within your means also prevents you from \n"
        << "falling into a trap of overspending.\n" << endl;

    cout << "\nPress Enter to return to the main menu...";
    cin.ignore();
    cin.get();
}

void financialAdvice(double monthlyBudget, double remainingMonthlyBudget, double remainingWeeklyBudget, double remainingDailyBudget, double savings) {
    double savingsPercentage;
    char periodChoice;

    cout << "\nEnter your desired savings percentage (e.g., 20 for 20%): ";
    while (!(cin >> savingsPercentage) || savingsPercentage < 0 || savingsPercentage > 100) {
        cout << "Invalid input. Please enter a number between 0 and 100 for the savings percentage: ";
        clearInput();
    }

    cout << "\nChoose the period for this savings percentage:" << endl;
    cout << "1. Daily" << endl;
    cout << "2. Weekly" << endl;
    cout << "3. Monthly" << endl;
    cout << "Enter your choice (1/2/3): ";
    while (!(cin >> periodChoice) || (periodChoice != '1' && periodChoice != '2' && periodChoice != '3')) {
        cout << "Invalid choice. Please enter 1, 2, or 3: ";
        clearInput();
    }

    double savingsGoal;

    switch (periodChoice) {
    case '1':
        savingsGoal = (monthlyBudget / 30) * (savingsPercentage / 100.0);
        break;
    case '2':
        savingsGoal = (monthlyBudget / 4) * (savingsPercentage / 100.0);
        break;
    case '3':
        savingsGoal = monthlyBudget * (savingsPercentage / 100.0);
        break;
    default:
        savingsGoal = 0;
        break;
    }

    cout << "\nFinancial Status:" << endl;


    switch (periodChoice) {
    case '1':
        cout << "\nDaily Budget:" << endl;
        if (remainingMonthlyBudget < 0) {
            cout << "- You have exceeded your daily budget. Aim to reduce spending tomorrow." << endl;
            cout << "- Your Remaining Daily Budget is: " << remainingMonthlyBudget << "\n";
        }
        else if (remainingMonthlyBudget < savingsGoal) {
            cout << "- Your savings are below the recommended " << savingsPercentage << "% of your daily budget. Try to save more." << endl;
            cout << "- Your Remaining Daily Budget is: " << remainingMonthlyBudget << "\n";
        }
        else {
            cout << "- You are managing your daily budget well and have met your savings goal. Continue maintaining control over your expenses." << endl;
            cout << "- Your Remaining Daily Budget is: " << remainingMonthlyBudget << "\n";
        }
        break;
    case '2':
        cout << "\nWeekly Budget:" << endl;
        if (remainingDailyBudget < 0) {
            cout << "- You have exceeded your weekly budget. Try to limit spending for the rest of the week." << endl;
            cout << "- Your Remaining Weekly Budget is: " << remainingDailyBudget << "\n";
        }
        else if (remainingDailyBudget < savingsGoal) {
            cout << "- Your savings are below the recommended " << savingsPercentage << "% of your weekly budget. Aim to allocate more towards savings." << endl;
            cout << "- Your Remaining Weekly Budget is: " << remainingDailyBudget << "\n";
        }
        else {
            cout << "- Your weekly budget is on track and you have met your savings goal. Keep it up!" << endl;
            cout << "- Your Remaining Weekly Budget is: " << remainingDailyBudget << "\n";
        }
        break;
    case '3':
        cout << "\nMonthly Budget:" << endl;
        if (remainingWeeklyBudget < 0) {
            cout << "- You have exceeded your monthly budget. Consider reducing expenses or finding ways to increase your income." << endl;
            cout << "- Your Remaining Monthly Budget is: " << remainingWeeklyBudget << "\n";
        }
        else if (remainingWeeklyBudget < savingsGoal) {
            cout << "- Your savings are below the recommended " << savingsPercentage << "% of your monthly budget. Try to allocate more towards savings." << endl;
            cout << "- Your Remaining Monthly Budget is: " << remainingWeeklyBudget << "\n";
        }
        else {
            cout << "- You are managing your monthly budget well and have met your savings goal. Continue monitoring your expenses and saving where possible." << endl;
            cout << "- Your Remaining Monthly Budget is: " << remainingWeeklyBudget << "\n";
        }
        break;
    }
    cout << "\nSavings: " << fixed << setprecision(2) << savings << endl;
    if (savings >= savingsGoal) {
        cout << "- Great job on saving! You've met or exceeded your savings goal. Consider investing your savings or keeping them for future unexpected expenses." << endl;
    }
    else {
        cout << "- Try to save at least " << savingsPercentage << "% of your chosen budget period to build a financial cushion." << endl;
    }

    cout << "\nWould you like more details on your financial advice? (y/n): ";
    char choice;
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        detailedAdvice();
    }
    else if (choice == 'n' || choice == 'N') {
        cout << "\nReturning to the main menu..." << endl;
    }
    else {
        cout << "\nInvalid choice." << endl;
        cout << "\nPress Enter to return to the main menu...";
        cin.ignore();
        cin.get();
    }
}


void setBudgetManually(double& monthlyBudget, double& remainingMonthlyBudget, double& remainingWeeklyBudget, double& remainingDailyBudget) {
    cout << "Enter your total monthly budget: ";
    while (!(cin >> monthlyBudget) || monthlyBudget < 0) {
        cout << "Please enter a positive number for the monthly budget: ";
        clearInput();
    }

    cout << "Enter your weekly budget: ";
    while (!(cin >> remainingWeeklyBudget) || remainingWeeklyBudget < 0 || remainingWeeklyBudget > monthlyBudget) {
        cout << "Weekly Budget must be positive and within the Monthly Budget: ";
        clearInput();
    }

    cout << "Enter your daily budget: ";
    while (!(cin >> remainingDailyBudget) || remainingDailyBudget < 0 || remainingDailyBudget > remainingWeeklyBudget) {
        cout << "Daily Budget must be positive and within the Weekly Budget: ";
        clearInput();
    }

    remainingMonthlyBudget = monthlyBudget;
    remainingMonthlyBudget -= remainingWeeklyBudget;
    remainingWeeklyBudget -= remainingDailyBudget;
}

void modifySavings(double& remainingDailyBudget, double& remainingWeeklyBudget, double& remainingMonthlyBudget, double& savings) {
    int choice;
    cout << "Modify Savings:" << endl;
    cout << "1. Add to Savings" << endl;
    cout << "2. Withdraw from Savings" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1) {
        cout << "Add to Savings from:" << endl;
        cout << "1. Daily Budget" << endl;
        cout << "2. Weekly Budget" << endl;
        cout << "3. Monthly Budget" << endl;
        cout << "Enter your choice: ";
        int fromChoice;
        cin >> fromChoice;

        double amount;
        cout << "Enter amount to add to savings: ";
        cin >> amount;

        switch (fromChoice) {
        case 1:
            if (amount <= remainingDailyBudget) {
                remainingDailyBudget -= amount;
                savings += amount;
                cout << "Added " << amount << " to savings from daily budget." << endl;
            }
            else {
                cout << "Insufficient funds in daily budget." << endl;
                cout << "\nPress Enter to return to the main menu...";
                cin.ignore();
                cin.get();
            }
            break;
        case 2:
            if (amount <= remainingWeeklyBudget) {
                remainingWeeklyBudget -= amount;
                savings += amount;
                cout << "Added " << amount << " to savings from weekly budget." << endl;
            }
            else {
                cout << "Insufficient funds in weekly budget." << endl;
                cout << "\nPress Enter to return to the main menu...";
                cin.ignore();
                cin.get();
            }
            break;
        case 3:
            if (amount <= remainingMonthlyBudget) {
                remainingMonthlyBudget -= amount;
                savings += amount;
                cout << "Added " << amount << " to savings from monthly budget." << endl;
            }
            else {
                cout << "Insufficient funds in monthly budget." << endl;
                cout << "\nPress Enter to return to the main menu...";
                cin.ignore();
                cin.get();
            }
            break;
        default:
            cout << "Invalid choice." << endl;
            cout << "\nPress Enter to return to the main menu...";
            cin.ignore();
            cin.get();
        }
    }
    else if (choice == 2) {
        cout << "Withdraw from Savings to:" << endl;
        cout << "1. Daily Budget" << endl;
        cout << "2. Weekly Budget" << endl;
        cout << "3. Monthly Budget" << endl;
        cout << "Enter your choice: ";
        int toChoice;
        cin >> toChoice;

        double amount;
        cout << "Enter amount to withdraw from savings: ";
        cin >> amount;

        if (amount <= savings) {
            switch (toChoice) {
            case 1:
                remainingDailyBudget += amount;
                savings -= amount;
                cout << "Withdrew " << amount << " from savings to daily budget." << endl;
                break;
            case 2:
                remainingWeeklyBudget += amount;
                savings -= amount;
                cout << "Withdrew " << amount << " from savings to weekly budget." << endl;
                break;
            case 3:
                remainingMonthlyBudget += amount;
                savings -= amount;
                cout << "Withdrew " << amount << " from savings to monthly budget." << endl;
                break;
            default:
                cout << "Invalid choice." << endl;
            }
        }
        else {
            cout << "Insufficient funds in savings." << endl;
            cout << "\nPress Enter to return to the main menu...";
            cin.ignore();
            cin.get();
        }
    }
    else {
        cout << "Invalid choice." << endl;
        cout << "\nPress Enter to return to the main menu...";
        cin.ignore();
        cin.get();
    }
}

int main() {
    string username;
    double monthlyBudget = 0.0;
    double remainingMonthlyBudget = 0.0;
    double remainingWeeklyBudget = 0.0;
    double remainingDailyBudget = 0.0;
    double savings = 0.0;
    map<string, double> dailyAllocations;
    map<string, double> weeklyAllocations;
    map<string, double> monthlyAllocations;

    // Load categories from file
    loadCategories(dailyAllocations, weeklyAllocations, monthlyAllocations);

    cout << "Enter your username: ";
    getline(cin, username);

    cout << "Choose an option for budget allocation:\n";
    cout << "1. Automate Budget Allocation\n";
    cout << "2. Set Budget Manually\n";
    cout << "Enter your choice (1/2): ";
    int allocationChoice;
    while (!(cin >> allocationChoice) || (allocationChoice != 1 && allocationChoice != 2)) {
        cout << "Invalid choice. Please enter 1 or 2: ";
        clearInput();
    }

    if (allocationChoice == 1) {
        cout << "Enter your total Monthly Budget: ";
        while (!(cin >> monthlyBudget) || monthlyBudget < 0) {
            cout << "Invalid input. Please enter a positive number for the budget: ";
            clearInput();
        }
        remainingMonthlyBudget = monthlyBudget;
        remainingWeeklyBudget = monthlyBudget / 4;
        remainingMonthlyBudget -= remainingWeeklyBudget;
        remainingDailyBudget = monthlyBudget / 30;
        remainingWeeklyBudget -= remainingDailyBudget;
    }
    else if (allocationChoice == 2) {
        setBudgetManually(monthlyBudget, remainingMonthlyBudget, remainingWeeklyBudget, remainingDailyBudget);
    }

    int choice = 0;
    while (choice != 9) {
        displayStatus(username, monthlyBudget, remainingMonthlyBudget, remainingWeeklyBudget, remainingDailyBudget, savings);

        // Update the table display calls
        displayTable("Budget Allocations:", dailyAllocations, remainingDailyBudget, weeklyAllocations, remainingWeeklyBudget, monthlyAllocations, remainingMonthlyBudget);

        displayMenu();
        while (!(cin >> choice) || choice < 1 || choice > 9) {
            cout << "Invalid choice. Please enter a number between 1 and 9: ";
            clearInput();
        }
        switch (choice) {
        case 1:
            startEndDay(remainingDailyBudget, remainingWeeklyBudget, remainingMonthlyBudget, savings, dailyAllocations);
            break;
        case 2:
            startEndWeek(remainingDailyBudget, remainingWeeklyBudget, remainingMonthlyBudget, savings, dailyAllocations, weeklyAllocations);
            break;
        case 3:
            startEndMonth(monthlyBudget, remainingMonthlyBudget, remainingWeeklyBudget, remainingDailyBudget, savings, dailyAllocations, weeklyAllocations, monthlyAllocations);
            break;
        case 4:
            budgetManagement(monthlyBudget, remainingMonthlyBudget, remainingWeeklyBudget, remainingDailyBudget);
            break;
        case 5:
            expensesAllocations(remainingDailyBudget, remainingWeeklyBudget, remainingMonthlyBudget, dailyAllocations, weeklyAllocations, monthlyAllocations);
            break;
        case 6:
            int subChoice;
            cout << "1. Add Category\n2. Remove Category\nEnter your choice: ";
            cin >> subChoice;
            if (subChoice == 1) {
                addExpenseCategory(dailyAllocations, weeklyAllocations, monthlyAllocations);
            }
            else if (subChoice == 2) {
                removeExpenseCategory(dailyAllocations, weeklyAllocations, monthlyAllocations);
            }
            else {
                cout << "Invalid choice.\n";
                cout << "\nPress Enter to return to the main menu...";
                cin.ignore();
                cin.get();
            }
            break;
        case 7:
            financialAdvice(monthlyBudget, remainingDailyBudget, remainingMonthlyBudget, remainingWeeklyBudget, savings);
            break;
        case 8:
            modifySavings(remainingDailyBudget, remainingWeeklyBudget, remainingMonthlyBudget, savings);
            break;
        case 9:
            cout << "Exiting program. Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice." << endl;
            cout << "\nPress Enter to return to the main menu...";
            cin.ignore();
            cin.get();
            break;
        }
    }

    return 0;
}

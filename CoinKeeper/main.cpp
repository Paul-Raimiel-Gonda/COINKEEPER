#include <iostream>
#include <iomanip>
#include <string>
#include <map>
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

void displayStatus(const string& username, const string& date, double monthlyBudget, double remainingMonthlyBudget, double remainingWeeklyBudget, double remainingDailyBudget, double savings) {
    cout << "\nUser: " << username << endl;
    cout << "Date: " << date << endl;
    cout << "Monthly Budget: " << fixed << setprecision(2) << monthlyBudget << endl;
    cout << "Remaining Monthly Budget: " << fixed << setprecision(2) << remainingMonthlyBudget << endl;
    cout << "Remaining Budget for this Week: " << (remainingWeeklyBudget == -1 ? "None Set" : to_string(remainingWeeklyBudget)) << endl;
    cout << "Remaining Budget for Today: " << (remainingDailyBudget == -1 ? "None Set" : to_string(remainingDailyBudget)) << endl;
    cout << "Savings: " << fixed << setprecision(2) << savings << endl;
    cout << endl;
}

void displayMenu() {
    cout << "MAIN MENU:" << endl;
    cout << "1. End/Start Day" << endl;
    cout << "2. End/Start Week (DI PA MAAYOS)" << endl;
    cout << "3. End/Start Month (DI PA MAAAYOS)" << endl;
    cout << "4. Budget Management" << endl;
    cout << "5. Expenses/Allocations" << endl;
    cout << "6. Add/Remove Expense Category" << endl;
    cout << "7. Check Financial Guidance" << endl;
    cout << "8. Exit" << endl;
    cout << "Enter your choice: ";
}

void displayTable(const string& title, const map<string, double>& allocations) {
    cout << "\n" << title << endl;
    cout << "|-------------------------------|" << endl;
    cout << "|     Category     |  Amount    |" << endl;
    cout << "|-------------------------------|" << endl;
    for (const auto& alloc : allocations) {
        cout << "| " << setw(16) << left << alloc.first << " | " << setw(10) << right << fixed << setprecision(2) << alloc.second << " |" << endl;
    }
    cout << "|-------------------------------|" << endl << endl;
}

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void startEndDay(double& remainingDailyBudget, double& remainingWeeklyBudget, double& remainingMonthlyBudget, double& savings, map<string, double>& dailyAllocations) {
    double totalAllocations = 0.0;

    for (const auto& alloc : dailyAllocations) {
        totalAllocations += alloc.second;
        remainingDailyBudget -= alloc.second;
        remainingWeeklyBudget -= alloc.second;
        remainingMonthlyBudget -= alloc.second;
    }

    if (remainingDailyBudget > 0) {
        savings += remainingDailyBudget;
        cout << "Day ended. Remaining daily budget added to savings: " << remainingDailyBudget << endl;
    }
    else {
        cout << "There is no savings for today." << endl;
    }

    remainingDailyBudget = -1.0;
    for (auto& alloc : dailyAllocations) {
        alloc.second = 0.0;
    }

    cout << "Enter today's budget: ";
    while (!(cin >> remainingDailyBudget) || remainingDailyBudget < 0) {
        cout << "Invalid input. Please enter a positive number for today's budget: ";
        clearInput();
    }
    cout << "New day started with budget: " << remainingDailyBudget << endl;
}

void startEndWeek(double& remainingWeeklyBudget, double& remainingMonthlyBudget, double& savings, map<string, double>& weeklyAllocations) {
    for (const auto& alloc : weeklyAllocations) {
        remainingWeeklyBudget -= alloc.second;
        remainingMonthlyBudget -= alloc.second;
    }
    if (remainingWeeklyBudget > 0) {
        savings += remainingWeeklyBudget;
    }
    remainingWeeklyBudget = -1.0;
    for (auto& alloc : weeklyAllocations) {
        alloc.second = 0.0;
    }
    cout << "Week ended. Remaining weekly budget added to savings." << endl;

    cout << "Enter this week's budget: ";
    while (!(cin >> remainingWeeklyBudget) || remainingWeeklyBudget < 0) {
        cout << "Invalid input. Please enter a positive number for this week's budget: ";
        clearInput();
    }
    cout << "New week started with budget: " << remainingWeeklyBudget << endl;
}

void startEndMonth(double& remainingMonthlyBudget, double& savings, map<string, double>& dailyAllocations, map<string, double>& weeklyAllocations, map<string, double>& monthlyAllocations) {
    for (const auto& alloc : monthlyAllocations) {
        remainingMonthlyBudget -= alloc.second;
    }
    savings += remainingMonthlyBudget;
    remainingMonthlyBudget = 0.0;
    cout << "Month ended. Remaining monthly budget added to savings." << endl;

    char addSavingsToBudget;
    cout << "Do you want to add your savings to the new monthly budget? (y/n): ";
    cin >> addSavingsToBudget;
    if (addSavingsToBudget == 'y' || addSavingsToBudget == 'Y') {
        cout << "Adding savings to the new monthly budget." << endl;
    }
    else {
        cout << "Savings will not be added to the new monthly budget." << endl;
        savings = 0.0;
    }

    double newMonthlyBudget;
    cout << "Enter this month's budget: ";
    while (!(cin >> newMonthlyBudget) || newMonthlyBudget < 0) {
        cout << "Invalid input. Please enter a positive number for this month's budget: ";
        clearInput();
    }
    remainingMonthlyBudget = newMonthlyBudget + savings;
    savings = 0.0;
    for (auto& alloc : dailyAllocations) {
        alloc.second = 0.0;
    }
    for (auto& alloc : weeklyAllocations) {
        alloc.second = 0.0;
    }
    for (auto& alloc : monthlyAllocations) {
        alloc.second = 0.0;
    }
    cout << "New month started with budget: " << remainingMonthlyBudget << endl;
}

void budgetManagement(double& monthlyBudget, double& remainingMonthlyBudget, double& remainingWeeklyBudget, double& remainingDailyBudget) {
    int choice;
    cout << "Budget Management:" << endl;
    cout << "1. Modify Monthly Budget" << endl;
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
        remainingDailyBudget = monthlyBudget / 30;
        break;
    case 2:
        cout << "Enter new weekly budget: ";
        cin >> remainingWeeklyBudget;
        break;
    case 3:
        cout << "Enter new daily budget: ";
        cin >> remainingDailyBudget;
        break;
    default:
        cout << "Invalid choice." << endl;
        break;
    }
}

void expensesAllocations(map<string, double>& dailyAllocations, map<string, double>& weeklyAllocations, map<string, double>& monthlyAllocations) {
    cout << "Expense Types: " << endl;
    cout << "1. Daily" << endl;
    cout << "2. Weekly" << endl;
    cout << "3. Monthly" << endl;

    int type;
    cout << "Enter expense type: ";
    cin >> type;

    string category;
    double amount;
    cout << "Enter category: ";
    cin >> category;
    cout << "Enter expense amount: ";
    cin >> amount;

    bool categoryExists = false;
    switch (type) {
    case 1:
        if (dailyAllocations.find(category) != dailyAllocations.end()) {
            categoryExists = true;
        }
        if (categoryExists) {
            dailyAllocations[category] = amount;
        }
        else {
            cout << "Category does not exist." << endl;
            return;
        }
        break;
    case 2:
        if (weeklyAllocations.find(category) != weeklyAllocations.end()) {
            categoryExists = true;
        }
        if (categoryExists) {
            weeklyAllocations[category] = amount;
        }
        else {
            cout << "Category does not exist." << endl;
            return;
        }
        break;
    case 3:
        if (monthlyAllocations.find(category) != monthlyAllocations.end()) {
            categoryExists = true;
        }
        if (categoryExists) {
            monthlyAllocations[category] = amount;
        }
        else {
            cout << "Category does not exist." << endl;
            return;
        }
        break;
    default:
        cout << "Invalid expense type." << endl;
        return;
    }

    cout << "Expense added to category: " << category << endl;
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
        }
        else {
            cout << "Category already exists." << endl;
        }
        break;
    case 2:
        if (weeklyAllocations.find(category) == weeklyAllocations.end()) {
            weeklyAllocations[category] = 0.0;
            cout << "Category added: " << category << endl;
        }
        else {
            cout << "Category already exists." << endl;
        }
        break;
    case 3:
        if (monthlyAllocations.find(category) == monthlyAllocations.end()) {
            monthlyAllocations[category] = 0.0;
            cout << "Category added: " << category << endl;
        }
        else {
            cout << "Category already exists." << endl;
        }
        break;
    default:
        cout << "Invalid expense type." << endl;
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
        return;
    }

    if (categoryExists) {
        cout << "Category removed: " << category << endl;
    }
    else {
        cout << "Category does not exist." << endl;
    }

    saveCategories(dailyAllocations, weeklyAllocations, monthlyAllocations);
}

void financialWarning(double remainingDailyBudget, double remainingWeeklyBudget, double remainingMonthlyBudget) {
    double PercentOfDaily = remainingDailyBudget * 0.2;
    double PercentOfWeekly = remainingWeeklyBudget * 0.2;
    double PercentOfMonthly = remainingMonthlyBudget * 0.2;

    if (remainingDailyBudget < PercentOfDaily) {
        cout << "Warning: Your daily budget is running low. Consider reducing expenses or reallocating funds." << endl;
    }
    if (remainingWeeklyBudget < PercentOfWeekly) {
        cout << "Warning: Your weekly budget is running low. Keep an eye on your expenses." << endl;
    }
    if (remainingMonthlyBudget < PercentOfMonthly) {
        cout << "Warning: Your monthly budget is running low. Plan your expenses carefully." << endl;
    }
}


void financialAdvice(double remainingMonthlyBudget, double remainingWeeklyBudget, double remainingDailyBudget, double savings) {
    cout << "\nFinancial Advice:" << endl;
    if (remainingMonthlyBudget < 0) {
        cout << "- You have exceeded your monthly budget. Consider reducing discretionary expenses or finding ways to increase your income." << endl;
    }
    else if (remainingMonthlyBudget < 0.25 * remainingMonthlyBudget) {
        cout << "- Your monthly budget is running low. Prioritize essential expenses and avoid unnecessary spending." << endl;
    }
    else {
        cout << "- You are managing your monthly budget well. Continue monitoring your expenses and saving where possible." << endl;
    }

    if (remainingWeeklyBudget < 0) {
        cout << "- You have exceeded your weekly budget. Try to limit spending for the rest of the week." << endl;
    }
    else if (remainingWeeklyBudget < 0.25 * remainingWeeklyBudget) {
        cout << "- Your weekly budget is running low. Be cautious with your spending for the remainder of the week." << endl;
    }
    else {
        cout << "- Your weekly budget is on track. Keep it up!" << endl;
    }

    if (remainingDailyBudget < 0) {
        cout << "- You have exceeded your daily budget. Aim to reduce spending tomorrow." << endl;
    }
    else if (remainingDailyBudget < 0.25 * remainingDailyBudget) {
        cout << "- Your daily budget is running low. Spend wisely for the rest of the day." << endl;
    }
    else {
        cout << "- You are managing your daily budget well. Keep maintaining this control over your expenses." << endl;
    }

    cout << "\nSavings: " << fixed << setprecision(2) << savings << endl;
    if (savings > 0) {
        cout << "- Great job on saving! Consider investing your savings or keeping them for future unexpected expenses." << endl;
    }
    else {
        cout << "- Try to save a portion of your income each month to build a financial cushion." << endl;
    }
}

int main() {
    string username;
    string date;
    double monthlyBudget = 0.0;
    double remainingMonthlyBudget = 0.0;
    double remainingWeeklyBudget = -1.0;
    double remainingDailyBudget = -1.0;
    double savings = 0.0;
    map<string, double> dailyAllocations;
    map<string, double> weeklyAllocations;
    map<string, double> monthlyAllocations;

    loadCategories(dailyAllocations, weeklyAllocations, monthlyAllocations);
    cout << "TRIAL FORKING";

    cout << "Enter your username: ";
    getline(cin, username);

    cout << "Enter the date (YYYY-MM-DD): ";
    getline(cin, date);

    cout << "Enter your monthly budget/income: ";
    while (!(cin >> monthlyBudget) || monthlyBudget < 0) {
        cout << "Invalid input. Please enter a positive number for the budget: ";
        clearInput();
    }

    remainingMonthlyBudget = monthlyBudget;
    remainingWeeklyBudget = monthlyBudget / 4;
    remainingDailyBudget = monthlyBudget / 30;

    int choice = 0;
    while (choice != 7) {
        displayStatus(username, date, monthlyBudget, remainingMonthlyBudget, remainingWeeklyBudget, remainingDailyBudget, savings);
        displayTable("Daily Expense Categories", dailyAllocations);
        displayTable("Weekly Expense Categories", weeklyAllocations);
        displayTable("Monthly Expense Categories", monthlyAllocations);
        displayMenu();
        while (!(cin >> choice) || choice < 1 || choice > 7) {
            cout << "Invalid choice. Please enter a number between 1 and 7: ";
            clearInput();
        }
        switch (choice) {
        case 1:
            startEndDay(remainingDailyBudget, remainingWeeklyBudget, remainingMonthlyBudget, savings, dailyAllocations);
            financialWarning(remainingDailyBudget, remainingWeeklyBudget, remainingMonthlyBudget);
            break;
        case 2:
            startEndWeek(remainingWeeklyBudget, remainingMonthlyBudget, savings, weeklyAllocations);
            financialWarning(remainingDailyBudget, remainingWeeklyBudget, remainingMonthlyBudget);
            break;
        case 3:
            startEndMonth(remainingMonthlyBudget, savings, dailyAllocations, weeklyAllocations, monthlyAllocations);
            financialWarning(remainingDailyBudget, remainingWeeklyBudget, remainingMonthlyBudget);
            break;
        case 4:
            budgetManagement(monthlyBudget, remainingMonthlyBudget, remainingWeeklyBudget, remainingDailyBudget);
            financialWarning(remainingDailyBudget, remainingWeeklyBudget, remainingMonthlyBudget);
            break;
        case 5:
            expensesAllocations(dailyAllocations, weeklyAllocations, monthlyAllocations);
            financialWarning(remainingDailyBudget, remainingWeeklyBudget, remainingMonthlyBudget);
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
            }
            break;
        case 7:
            financialAdvice(remainingMonthlyBudget, remainingWeeklyBudget, remainingDailyBudget, savings);
            break;
        case 8:
            cout << "Exiting the program. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    }

    return 0;
}

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
    cout << "Total Budget: " << fixed << setprecision(2) << monthlyBudget << endl;
    cout << "Monthly Budget: " << fixed << setprecision(2) << remainingMonthlyBudget << endl;
    cout << "Weekly Budget: " << (remainingWeeklyBudget == -1 ? "None Set" : to_string(remainingWeeklyBudget)) << endl;
    cout << "Daily Budget: " << (remainingDailyBudget == -1 ? "None Set" : to_string(remainingDailyBudget)) << endl;
    cout << "Savings: " << fixed << setprecision(2) << savings << endl;
    cout << endl;
}

void displayMenu() {
    cout << "MAIN MENU:" << endl;
    cout << "1. End/Start Day" << endl;
    cout << "2. End/Start Week" << endl;
    cout << "3. End/Start Month" << endl;
    cout << "4. Budget Management" << endl;
    cout << "5. Expenses/Allocations" << endl;
    cout << "6. Add/Remove Expense Category" << endl;
    cout << "7. Financial Advice" << endl;
    cout << "8. Exit" << endl;
    cout << "Enter your choice: ";
}

void displayTable(const string& title, const map<string, double>& allocations, double remainingBudget) {
    cout << "\n" << title << endl;
    cout << "|-------------------------------|" << endl;
    cout << "|     Category     |  Amount    |" << endl;
    cout << "|-------------------------------|" << endl;

    double totalExpenses = 0.0;

    for (const auto& alloc : allocations) {
        cout << "| " << setw(16) << left << alloc.first << " | " << setw(10) << right << fixed << setprecision(2) << alloc.second << " |" << endl;
        totalExpenses += alloc.second;
    }

    cout << "|-------------------------------|" << endl;
    cout << "| Total Expenses   | " << setw(10) << right << fixed << setprecision(2) << totalExpenses << " |" << endl;
    cout << "| Remaining Budget | " << setw(10) << right << fixed << setprecision(2) << remainingBudget << " |" << endl;
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
    }

    if (remainingDailyBudget > 0) {
        char choice;
        cout << "Day ended. Remaining daily budget: " << remainingDailyBudget << endl;
        cout << "Do you want to add the remaining daily budget to savings or return it to the weekly budget? (s/r): ";
        cin >> choice;

        if (choice == 's' || choice == 'S') {
            savings += remainingDailyBudget;
            cout << "Added to savings." << endl;
        }
        else if (choice == 'r' || choice == 'R') {
            remainingWeeklyBudget += remainingDailyBudget;
            cout << "Returned to weekly budget." << endl;
        }
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
    remainingWeeklyBudget -= remainingDailyBudget; // Deduct from weekly budget
    cout << "New day started with budget: " << remainingDailyBudget << endl;
}

void startEndWeek(double& remainingWeeklyBudget, double& remainingMonthlyBudget, double& savings, map<string, double>& weeklyAllocations) {
    double totalAllocations = 0.0;

    for (const auto& alloc : weeklyAllocations) {
        totalAllocations += alloc.second;
    }

    if (remainingWeeklyBudget > 0) {
        char choice;
        cout << "Week ended. Remaining weekly budget: " << remainingWeeklyBudget << endl;
        cout << "Do you want to add the remaining weekly budget to savings or return it to the monthly budget? (s/r): ";
        cin >> choice;

        if (choice == 's' || choice == 'S') {
            savings += remainingWeeklyBudget;
            cout << "Added to savings." << endl;
        }
        else if (choice == 'r' || choice == 'R') {
            remainingMonthlyBudget += remainingWeeklyBudget;
            cout << "Returned to monthly budget." << endl;
        }
    }

    remainingWeeklyBudget = -1.0;
    for (auto& alloc : weeklyAllocations) {
        alloc.second = 0.0;
    }

    cout << "Enter this week's budget: ";
    while (!(cin >> remainingWeeklyBudget) || remainingWeeklyBudget < 0) {
        cout << "Invalid input. Please enter a positive number for this week's budget: ";
        clearInput();
    }
    remainingMonthlyBudget -= remainingWeeklyBudget; // Deduct from monthly budget
    cout << "New week started with budget: " << remainingWeeklyBudget << endl;
}

void startEndMonth(double& monthlyBudget, double& remainingMonthlyBudget, double& remainingWeeklyBudget, double& remainingDailyBudget, double& savings, map<string, double>& dailyAllocations, map<string, double>& weeklyAllocations, map<string, double>& monthlyAllocations) {
    double totalAllocations = 0.0;

    for (const auto& alloc : monthlyAllocations) {
        totalAllocations += alloc.second;
    }

    if (remainingMonthlyBudget > 0) {
        char choice;
        cout << "Month ended. Remaining monthly budget: " << remainingMonthlyBudget << endl;
        cout << "Do you want to add the remaining monthly budget to savings? (y/n): ";
        cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            savings += remainingMonthlyBudget;
            cout << "Added to savings." << endl;
        }
    }

    char addSavingsToBudget;
    cout << "Do you want to add your savings to the new monthly budget? (y/n): ";
    cin >> addSavingsToBudget;

    double newMonthlyBudget = 0.0;
    cout << "Enter this month's budget: ";
    while (!(cin >> newMonthlyBudget) || newMonthlyBudget < 0) {
        cout << "Invalid input. Please enter a positive number for this month's budget: ";
        clearInput();
    }

    if (addSavingsToBudget == 'y' || addSavingsToBudget == 'Y') {
        cout << "Adding savings to the new monthly budget." << endl;
        newMonthlyBudget += savings;
    }
    else {
        cout << "Savings will not be added to the new monthly budget." << endl;
    }

    monthlyBudget = newMonthlyBudget;
    remainingMonthlyBudget = monthlyBudget;
    remainingWeeklyBudget = monthlyBudget / 4;
    remainingMonthlyBudget = monthlyBudget - remainingWeeklyBudget;
    remainingDailyBudget = monthlyBudget / 30;
    remainingWeeklyBudget = remainingWeeklyBudget - remainingDailyBudget;
    savings = 0.0; // Reset savings after adding to budget

    for (auto& alloc : dailyAllocations) {
        alloc.second = 0.0;
    }
    for (auto& alloc : weeklyAllocations) {
        alloc.second = 0.0;
    }
    for (auto& alloc : monthlyAllocations) {
        alloc.second = 0.0;
    }

    cout << "New month started with budget: " << monthlyBudget << endl;
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
        remainingMonthlyBudget -= remainingWeeklyBudget; // Deduct from monthly budget
        break;
    case 3:
        cout << "Enter new daily budget: ";
        cin >> remainingDailyBudget;
        remainingWeeklyBudget -= remainingDailyBudget; // Deduct from weekly budget
        break;
    default:
        cout << "Invalid choice." << endl;
        break;
    }
}

void expensesAllocations(double& remainingDailyBudget, double& remainingWeeklyBudget, double& remainingMonthlyBudget, map<string, double>& dailyAllocations, map<string, double>& weeklyAllocations, map<string, double>& monthlyAllocations) {
    int choice;
    cout << "Expense Types:" << endl;
    cout << "1. Daily" << endl;
    cout << "2. Weekly" << endl;
    cout << "3. Monthly" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    string category;
    double amount;

    switch (choice) {
    case 1:
        cout << "Enter expense category: ";
        cin >> category;
        cout << "Enter amount: ";
        cin >> amount;

        if (dailyAllocations.find(category) != dailyAllocations.end()) {
            dailyAllocations[category] += amount;
            remainingDailyBudget -= amount;
            if (remainingDailyBudget < 0) {
                cout << "Warning: Daily budget exceeded!" << endl;
            }
        }
        else {
            cout << "Category not found." << endl;
        }
        break;
    case 2:
        cout << "Enter expense category: ";
        cin >> category;
        cout << "Enter amount: ";
        cin >> amount;

        if (weeklyAllocations.find(category) != weeklyAllocations.end()) {
            weeklyAllocations[category] += amount;
            remainingWeeklyBudget -= amount;
            if (remainingWeeklyBudget < 0) {
                cout << "Warning: Weekly budget exceeded!" << endl;
            }
        }
        else {
            cout << "Category not found." << endl;
        }
        break;
    case 3:
        cout << "Enter expense category: ";
        cin >> category;
        cout << "Enter amount: ";
        cin >> amount;

        if (monthlyAllocations.find(category) != monthlyAllocations.end()) {
            monthlyAllocations[category] += amount;
            remainingMonthlyBudget -= amount;
            if (remainingMonthlyBudget < 0) {
                cout << "Warning: Monthly budget exceeded!" << endl;
            }
        }
        else {
            cout << "Category not found." << endl;
        }
        break;
    default:
        cout << "Invalid choice." << endl;
        break;
    }
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
        if (remainingDailyBudget < 0) {
            cout << "- You have exceeded your daily budget. Aim to reduce spending tomorrow." << endl;
        }
        else if (remainingDailyBudget < savingsGoal) {
            cout << "- Your savings are below the recommended " << savingsPercentage << "% of your daily budget. Try to save more." << endl;
        }
        else {
            cout << "- You are managing your daily budget well and have met your savings goal. Continue maintaining control over your expenses." << endl;
        }
        break;
    case '2':
        cout << "\nWeekly Budget:" << endl;
        if (remainingWeeklyBudget < 0) {
            cout << "- You have exceeded your weekly budget. Try to limit spending for the rest of the week." << endl;
        }
        else if (remainingWeeklyBudget < savingsGoal) {
            cout << "- Your savings are below the recommended " << savingsPercentage << "% of your weekly budget. Aim to allocate more towards savings." << endl;
        }
        else {
            cout << "- Your weekly budget is on track and you have met your savings goal. Keep it up!" << endl;
        }
        break;
    case '3':
        cout << "\nMonthly Budget:" << endl;
        if (remainingMonthlyBudget < 0) {
            cout << "- You have exceeded your monthly budget. Consider reducing expenses or finding ways to increase your income." << endl;
        }
        else if (remainingMonthlyBudget < savingsGoal) {
            cout << "- Your savings are below the recommended " << savingsPercentage << "% of your monthly budget. Try to allocate more towards savings." << endl;
        }
        else {
            cout << "- You are managing your monthly budget well and have met your savings goal. Continue monitoring your expenses and saving where possible." << endl;
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
        cout << "\nInvalid choice. Returning to the main menu..." << endl;
    }
}


void setBudgetManually(double& monthlyBudget, double& remainingMonthlyBudget, double& remainingWeeklyBudget, double& remainingDailyBudget) {
    cout << "Enter your monthly budget: ";
    while (!(cin >> monthlyBudget) || monthlyBudget < 0) {
        cout << "Invalid input. Please enter a positive number for the monthly budget: ";
        clearInput();
    }

    cout << "Enter your weekly budget: ";
    while (!(cin >> remainingWeeklyBudget) || remainingWeeklyBudget < 0) {
        cout << "Invalid input. Please enter a positive number for the weekly budget: ";
        clearInput();
    }

    cout << "Enter your daily budget: ";
    while (!(cin >> remainingDailyBudget) || remainingDailyBudget < 0) {
        cout << "Invalid input. Please enter a positive number for the daily budget: ";
        clearInput();
    }

    remainingMonthlyBudget = monthlyBudget;
    remainingWeeklyBudget = remainingWeeklyBudget;
    remainingDailyBudget = remainingDailyBudget;
    remainingMonthlyBudget -= remainingWeeklyBudget; // Deduct weekly budget from monthly
    remainingWeeklyBudget -= remainingDailyBudget; // Deduct daily budget from weekly
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

    // Load categories from file
    loadCategories(dailyAllocations, weeklyAllocations, monthlyAllocations);

    cout << "Enter your username: ";
    getline(cin, username);

    cout << "Enter the date (YYYY-MM-DD): ";
    getline(cin, date);

    cout << "Enter your total budget/income: ";
    while (!(cin >> monthlyBudget) || monthlyBudget < 0) {
        cout << "Invalid input. Please enter a positive number for the budget: ";
        clearInput();
    }

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
        remainingMonthlyBudget = monthlyBudget;
        remainingWeeklyBudget = monthlyBudget / 4;
        remainingMonthlyBudget = remainingMonthlyBudget - remainingWeeklyBudget;
        remainingDailyBudget = monthlyBudget / 30;
        remainingWeeklyBudget = remainingWeeklyBudget - remainingDailyBudget;
    }
    else if (allocationChoice == 2) {
        setBudgetManually(monthlyBudget, remainingMonthlyBudget, remainingWeeklyBudget, remainingDailyBudget);
    }

    int choice = 0;
    while (choice != 8) {
        displayStatus(username, date, monthlyBudget, remainingMonthlyBudget, remainingWeeklyBudget, remainingDailyBudget, savings);

        // Update the table display calls
        displayTable("Daily Expense Categories", dailyAllocations, remainingDailyBudget);
        displayTable("Weekly Expense Categories", weeklyAllocations, remainingWeeklyBudget);
        displayTable("Monthly Expense Categories", monthlyAllocations, remainingMonthlyBudget);

        displayMenu();
        while (!(cin >> choice) || choice < 1 || choice > 8) {
            cout << "Invalid choice. Please enter a number between 1 and 8: ";
            clearInput();
        }
        switch (choice) {
        case 1:
            startEndDay(remainingDailyBudget, remainingWeeklyBudget, remainingMonthlyBudget, savings, dailyAllocations);
            break;
        case 2:
            startEndWeek(remainingWeeklyBudget, remainingMonthlyBudget, savings, weeklyAllocations);
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
            // Load categories from file
            addExpenseCategory(dailyAllocations, weeklyAllocations, monthlyAllocations);
            break;
        case 7:
            financialAdvice(monthlyBudget, remainingDailyBudget, remainingMonthlyBudget, remainingWeeklyBudget, savings);
            break;
        case 8:
            cout << "Exiting program. Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    }

    return 0;
}

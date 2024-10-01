#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

class User {
public:
    string userId;

    User(const string& id) : userId(id) {}
};

class ExpenseManager {
private:
    vector<User> users;
    map<string, map<string, double> > balances;

public:
    void addUser(const User& user) {
        users.push_back(user);
    }

    void addExpense(const string& payerId, double amount, int numUsers, const vector<string>& userIds, const string& type, const vector<double>& shares) {
        if (type == "EQUAL") {
            double splitAmount = round((amount / numUsers) * 100.0) / 100.0; // Round to 2 decimal places
            for (const auto& id : userIds) {
                if (id != payerId) {
                    balances[id][payerId] += splitAmount;
                }
            }
        } else if (type == "EXACT") {
            double totalShare = 0;
            for (const auto& share : shares) {
                totalShare += share;
            }
            if (totalShare != amount) {
                cout << "Error: Total shares do not equal the amount." << endl;
                return;
            }
            for (size_t i = 0; i < numUsers; ++i) {
                if (userIds[i] != payerId) {
                    balances[userIds[i]][payerId] += shares[i];
                }
            }
        } else if (type == "PERCENT") {
            double totalPercent = 0;
            for (const auto& percent : shares) {
                totalPercent += percent;
            }
            if (totalPercent != 100) {
                cout << "Error: Total percentage does not equal 100." << endl;
                return;
            }
            for (size_t i = 0; i < numUsers; ++i) {
                if (userIds[i] != payerId) {
                    double amountOwed = round((shares[i] / 100.0) * amount * 100.0) / 100.0; // Round to 2 decimal places
                    balances[userIds[i]][payerId] += amountOwed;
                }
            }
        }
    }

    void showBalances(const string& userId = "") {
        if (userId.empty()) {
            for (const auto& user : users) {
                showBalances(user.userId);
            }
        } else {
            if (balances[userId].empty()) {
                cout << "No balances" << endl;
                return;
            }
            for (const auto& [owedUser, amount] : balances[userId]) {
                cout << owedUser << " owes " << userId << ": " << round(amount) << endl;
            }
        }
    }

    // string formatAmount(double amount) {
    //     // Format amount to two decimal places without using iomanip
    //     int wholePart = static_cast<int>(amount);
    //     int fractionalPart = static_cast<int>(round((amount - wholePart) * 100));

    //     // Ensure fractional part is two digits
    //     string formattedAmount = to_string(wholePart);
    //     if (fractionalPart < 10) {
    //         formattedAmount += ".0" + to_string(fractionalPart);
    //     } else {
    //         formattedAmount += "." + to_string(fractionalPart);
    //     }

    //     return formattedAmount;
    // }
};

int main() {
    ExpenseManager expenseManager;

    // Creating users
    expenseManager.addUser(User("u1"));
    expenseManager.addUser(User("u2"));
    expenseManager.addUser(User("u3"));
    expenseManager.addUser(User("u4"));

    // Sample inputs
    expenseManager.showBalances(); // SHOW
    expenseManager.showBalances("u1"); // SHOW u1
    expenseManager.addExpense("u1", 1000, 4, {"u1", "u2", "u3", "u4"}, "EQUAL", {}); // EXPENSE u1 1000 4 u1 u2 u3 u4 EQUAL
    expenseManager.showBalances("u4"); // SHOW u4
    expenseManager.showBalances("u1"); // SHOW u1
    expenseManager.addExpense("u1", 1250, 2, {"u2", "u3"}, "EXACT", {370, 880}); // EXPENSE u1 1250 2 u2 u3 EXACT 370 880
    expenseManager.showBalances(); // SHOW
    expenseManager.addExpense("u4", 1200, 4, {"u1", "u2", "u3", "u4"}, "PERCENT", {40, 20, 20, 20}); // EXPENSE u4 1200 4 u1 u2 u3 u4 PERCENT 40 20 20 20
    expenseManager.showBalances("u1"); // SHOW u1
    expenseManager.showBalances(); // SHOW

    return 0;
}

#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

enum UserType {
    CUSTOMER,
    ADMIN
};

class User {
    string id;
    UserType utype;

public:
    User(string _id, UserType _utype) {
        id = _id;
        utype = _utype;
    }

    string getID() {
        return id;
    }

    bool isAdmin() {
        return utype == ADMIN;
    }
};

class Loan {
public:
    int loan_id;
    string admin_username;
    string customer_username;
    int principal;
    float rate;
    int tenure;

    float EMI;
    float repayment_amount;
    float amount_repaid;
    float interest_amount;
    int paidEMIcount;
    vector<pair<float, int> > paidEMIs;

    void calculateEMI(int principal, float rate, int tenure) {
        interest_amount = ((float)principal * rate * (float)tenure) / (float)100.0;
        repayment_amount = interest_amount + (float)principal;
        amount_repaid = 0.0;
        EMI = repayment_amount / (12.0 * (float)tenure);
        paidEMIcount = 0;
    }

    void makePayment(int date) {
        if (amount_repaid >= repayment_amount) return;
        amount_repaid += EMI;
        paidEMIcount += 1;
        paidEMIs.push_back(make_pair(EMI, date));
    }

    Loan(User _admin, User _customer, int _principal, float _rate, int _tenure) {
        admin_username = _admin.getID();
        customer_username = _customer.getID();
        principal = _principal;
        rate = _rate;
        tenure = _tenure;
        calculateEMI(principal, rate, tenure);
        loan_id = rand() % 1000;
    }

    string getCustomerID() {
        return customer_username;
    }
};

class Bank {
    vector<User> people;
    vector<Loan> loans;

public:
    Bank(int _bank_id) {
        int bank_id = _bank_id;
    }

    void addUser(User user) {
        people.push_back(user);
    }

    void addLoan(User admin, User customer, int _principal, float _rate, int _tenure) {
        if (!admin.isAdmin() || _principal <= 0 || _rate <= 0.0 || _tenure <= 0) {
            cout << "Loan creation failed!\n";
            return;
        }
        Loan loan = Loan(admin, customer, _principal, _rate, _tenure);
        loans.push_back(loan);
        cout << "Loan created successfully for customer: " << customer.getID() << endl;
    }

    void display_loan_details(User customer) {
        int count = 0;
        for (int i = 0; i < loans.size(); i++) {
            if (loans[i].getCustomerID() != customer.getID()) continue;
            count += 1;
            cout << "LOAN " << count << " : with loan ID = " << loans[i].loan_id 
                 << ", Principal = " << loans[i].principal 
                 << ", Rate = " << loans[i].rate 
                 << ", Tenure = " << loans[i].tenure 
                 << ", EMI = " << loans[i].EMI 
                 << ", Amount Repaid = " << loans[i].amount_repaid 
                 << ", Total Repayment Amount = " << loans[i].repayment_amount << endl;
        }
        if (count == 0) {
            cout << "No loans found for customer: " << customer.getID() << endl;
        }
    }

    vector<Loan> getAllLoans(User admin) {
        if (!admin.isAdmin()) {
            cout << "ERROR: You should be admin to access this!\n";
            return vector<Loan>();
        }
        return loans;
    }
};

int main() {
    Bank bank(1);

    User admin("admin1", ADMIN);
    User customer("customer1", CUSTOMER);

    bank.addUser(admin);
    bank.addUser(customer);

    bank.addLoan(admin, customer, 100000, 10.0, 5);

    bank.display_loan_details(customer);

    vector<Loan> allLoans = bank.getAllLoans(admin);
    cout << "Total loans in the bank: " << allLoans.size() << endl;

    if (!allLoans.empty()) {
        allLoans[0].makePayment(1);
        cout << "Payment made! Total amount repaid: " << allLoans[0].amount_repaid << endl;
    }

    bank.display_loan_details(customer);

    return 0;
}

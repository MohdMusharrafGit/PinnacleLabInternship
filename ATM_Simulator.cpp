#include <iostream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

class Account {
private:
    string username;
    string password;
    double balance;
    vector<string> transactionHistory;

public:
    Account(string user, string pass, double initialBalance) {
        username = user;
        password = pass;
        balance = initialBalance;
    }

    bool authenticate(string user, string pass) {
        return (username == user && password == pass);
    }

    double getBalance() {
        return balance;
    }

    void deposit(double amount) {
        balance += amount;
        recordTransaction("Deposited: " + to_string(amount));
    }

    bool withdraw(double amount) {
        if (amount > balance) {
            cout << "Insufficient funds!" << endl;
            return false;
        } else {
            balance -= amount;
            recordTransaction("Withdrew: " + to_string(amount));
            return true;
        }
    }

    bool transfer(Account& recipient, double amount) {
        if (withdraw(amount)) {
            recipient.deposit(amount);
            recordTransaction("Transferred: " + to_string(amount) + " to " + recipient.username);
            return true;
        }
        return false;
    }

    void printTransactionHistory() {
        cout << "Transaction History: " << endl;
        for (const string& transaction : transactionHistory) {
            cout << transaction << endl;
        }
    }

private:
    void recordTransaction(string transaction) {
        time_t now = time(0);
        char* dt = ctime(&now);
        transactionHistory.push_back(dt + string(": ") + transaction);
    }
};

class ATM {
private:
    Account* currentAccount;

public:
    ATM() : currentAccount(nullptr) {}

    void login(Account& account) {
        string username, password;
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        if (account.authenticate(username, password)) {
            currentAccount = &account;
            cout << "Login successful!" << endl;
        } else {
            cout << "Invalid credentials!" << endl;
        }
    }

    void showMenu() {
        if (!currentAccount) {
            cout << "Please log in first!" << endl;
            return;
        }

        int choice;
        do {
            cout << "\nATM Menu: \n";
            cout << "1. Check Balance\n";
            cout << "2. Deposit Funds\n";
            cout << "3. Withdraw Funds\n";
            cout << "4. Transfer Funds\n";
            cout << "5. View Transaction History\n";
            cout << "6. Logout\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    cout << "Current Balance: $" << currentAccount->getBalance() << endl;
                    break;
                case 2: {
                    double amount;
                    cout << "Enter amount to deposit: $";
                    cin >> amount;
                    currentAccount->deposit(amount);
                    break;
                }
                case 3: {
                    double amount;
                    cout << "Enter amount to withdraw: $";
                    cin >> amount;
                    currentAccount->withdraw(amount);
                    break;
                }
                case 4: {
                    string recipientUsername;
                    double amount;
                    cout << "Enter recipient's username: ";
                    cin >> recipientUsername;
                    cout << "Enter amount to transfer: $";
                    cin >> amount;

                    // For simplicity, assuming a recipient account with same username
                    Account recipient(recipientUsername, "password", 500);  // Creating a dummy account for the transfer
                    if (currentAccount->transfer(recipient, amount)) {
                        cout << "Transfer successful!" << endl;
                    } else {
                        cout << "Transfer failed!" << endl;
                    }
                    break;
                }
                case 5:
                    currentAccount->printTransactionHistory();
                    break;
                case 6:
                    cout << "Logging out...\n";
                    currentAccount = nullptr;
                    break;
                default:
                    cout << "Invalid choice, please try again.\n";
            }
        } while (choice != 6);
    }
};

int main() {
    Account account("user1", "password123", 1000.00);
    ATM atm;

    cout << "Welcome to the ATM System\n";

    atm.login(account);
    atm.showMenu();

    return 0;
}

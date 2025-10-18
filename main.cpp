#include <iostream>
#include "bank_customer.h"
#include "buyer.h"
#include <vector>
#include <string>

enum PrimaryPrompt{LOGIN, REGISTER, EXIT, ADMIN_LOGIN};
enum RegisterPrompt{CREATE_BUYER, CREATE_SELLER, BACK};
using namespace std;

Buyer createBuyer(BankCustomer &account);
BankCustomer createBankCustomer(int id, string name, double balance);
bool verifyBankAccount(string name);

int main() {
    //create a loop prompt 
    PrimaryPrompt prompt = LOGIN;
    RegisterPrompt regPrompt = CREATE_BUYER;
    const string ADMIN_USERNAME = "root";
    const string ADMIN_PASSWORD = "toor";
    string username, password;
    

    while (prompt != EXIT) {
        cout << "Select an option: " << endl;
        cout << "1. Login" << endl;
        cout << "2. Register" << endl;
        cout << "3. Exit" << endl;
        cout << "4. Admin Login" << endl;
        int choice;
        cin >> choice;
        prompt = static_cast<PrimaryPrompt>(choice - 1);
        switch (prompt) {
            case LOGIN:
                cout << "Login selected." << endl;
                /* if Login is selected, based on authority then provide options:
                assume user is logged in as Buyer for now
                1. Chek Account Status (will display if user is Buyer or Seller or both and linked banking account status)
                Will display Buyer, Seller and Banking Account details
                2. Upgrade Account to Seller
                Will prompt user to enter Seller details and create a Seller account linked to Buyer account
                Will reject if a user dont have a banking account linked
                3. Create Banking Account (if not already linked), will be replaced with banking functions
                Must provides: initial deposit amount, Address, Phone number, Email
                Banking functions will provides: Balance checking, Transaction History, Deposit, Withdraw
                4. Browse Store Functionality
                Will display all stores initially
                Need to select a store to browse each store inventory
                Will display all items in the store inventory
                After selecting an item, will display item details and option to add to cart
                After adding to cart, will notify user item is added to cart
                5. Order Functionality
                Will display all items in cart
                Will provide option to remove item from cart
                Will provide option to checkout
                After checkout invoide will be generated (will go to payment functionality)
                6. Payment Functionality
                Will display all listed invoices
                Pick an invoice to pay
                Will display invoice details and total amount
                Will provide option to pay invoice
                Payment is done through confirmation dialogue
                In confirmation dialogue, will display account balance as precursor
                User will need to manually enter invoice id to pay
                After paying balance will be redacted from buyer and added to the responding seller account
                After payment, order status will be changed to paid
                7. Logout (return to main menu)
                Display confirmation dialogue
                If confirmed, return to main menu
                If not, return to Buyer menu
                8. Delete Account (remove both Buyer and Seller account and relevant banking account)
                Display confirmation dialogue
                If confirmed, delete account and return to main menu
                If not, return to Buyer menu

                assume user is logged in as Seller for now
                9. Check Inventory
                10. Add Item to Inventory
                11. Remove Item from Inventory
                12. View Orders (will display all orders placed to this seller
                Only orders with paid status will be listed
                Order details will listing items, quantity, total amount, buyer details, order status (paid, cancelled, completed)
                extra functions
                9. Exit to main Menu
                10. Exit Program
                **/
                break;
            case REGISTER:
                regPrompt = CREATE_BUYER; // reset regPrompt to CREATE_BUYER when entering register menu
                while (regPrompt != BACK){
                    cout << "Register selected. " << endl;
                    cout << "Select an option: " << endl;
                    cout << "1. Create Buyer Account" << endl;
                    cout << "2. Create Seller Account" << endl;
                    cout << "3. Back" << endl;
                    int regChoice;
                    cin >> regChoice;
                    regPrompt = static_cast<RegisterPrompt>(regChoice - 1);
                    switch (regPrompt) {

                        case CREATE_BUYER: {
                            int bankAccountID = 0;
                            cout << "Create Buyer Account selected." << endl;
                            cout << "Punya akun bank? (y/n): ";
                            char hasBankAccount;
                            cin >> hasBankAccount;
                            cin.ignore(); // to clear the newline character from the buffer

                            if (hasBankAccount == 'n' || hasBankAccount == 'N') 
                            {
                                double initialDeposit;

                                cout << "Creating Bank Account first." << endl;
                                cout << "Enter Name for Bank Account: ";
                                getline(cin, username);
                                cout << "Enter Initial Deposit Amount: ";
                                cin >> initialDeposit;
                                cin.ignore(); // to clear the newline character from the buffer
                                bankAccountID++;

                                BankCustomer newBankAccount = createBankCustomer(bankAccountID, username, initialDeposit);
                                cout << "\nBank Account created successfully.\n" << endl;
                                cout << "Creating Buyer Account now." << endl;
                                createBuyer(newBankAccount);
                                
                            }

                            else if (hasBankAccount == 'y' || hasBankAccount == 'Y')
                            {
                                cout << "Verifying Bank Account." << endl;
                                cout << "Enter Name for Bank Account: ";
                                getline(cin, username);
                                cout << verifyBankAccount(username) << endl;
                            }
                            break;
                        }

                        case CREATE_SELLER:
                            cout << "Create Seller Account selected." << endl;
                            break;
                        case BACK:
                            cout << "Back selected." << endl;
                            break;
                        default:
                            cout << "Invalid option." << endl;
                            break;
                    }
                }
                /* if register is selected then went throuhh registration process:
                1. Create a new Buyer Account
                Must provides: Name, Home Address, Phone number, Email
                2. Option to create a Seller Account (will be linked to Buyer account)
                Must Provides 1: Home Address, Phone number, Email
                Must provides 2: Store Name, Store Address, Store Phone number, Store Email
                Must provides 3: initial deposit amount
                After finished immediately logged in as Buyer/Seller
                */
                break;
            case EXIT:
                cout << "Exiting." << endl;
                break;
            case ADMIN_LOGIN:
                /* Prompt for username & password then check the entries with our hard coded features */
                cout << "Username: ";
                cin >> username;
                cout << "Password: ";
                cin >> password;
                /** After login create a sub prompt that provides the following features
                1. Account Management
                    - View All Buyers, Sellers
                    - View All details of Buyers, Sellers
                    - Seek certain buyer of seller based on Name / account Id / address / phone number
                    - Create new buyer/seller/Bank account
                    - Remove buyer/seller based on ID (all related info will be deleted)
                2. System Report
                    - Total number of Buyers, Sellers
                    - Total number of Banking Accounts
                */
                break;
            default:
                cout << "Invalid option." << endl;
                break;
        }
        cout << endl;
    }
    return 1;
}

Buyer createBuyer(BankCustomer &account)
{
    string username, password, address, phoneNumber, email;
    int buyerIdCounter = 0;

    cout << "Enter Name: ";
    getline(cin, username);
    cout << "Enter Address: ";
    getline(cin, address);
    cout << "Enter Phone Number: ";
    getline(cin, phoneNumber);
    cout << "Enter Email: ";
    getline(cin, email);
    buyerIdCounter++;
    Buyer* createBuyer = new Buyer(buyerIdCounter, username, address, phoneNumber, email, account);
    return (*createBuyer);
}

BankCustomer createBankCustomer(int id, string name, double balance)
{
    BankCustomer* newBankCustomer = new BankCustomer(id, name, balance);
    return (*newBankCustomer);
}

bool verifyBankAccount(string name)
{
    return true;
}
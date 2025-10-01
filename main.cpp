#include <iostream>
#include "bank_customer.h"
#include "buyer.h"
#include <vector>

enum PrimaryPrompt{LOGIN, REGISTER, EXIT};
enum RegisterPrompt{CREATE_BUYER, CREATE_SELLER, BACK};

using namespace std;
vector<Buyer*> buyers;

int main() {
    PrimaryPrompt prompt = LOGIN;
    static int nextBuyerId = 1;
    static int nextBankId = 1;
    static int nextSellerId = 1;
    while (prompt != EXIT) {
        cout << "Select an option: " << endl;
        cout << "1. Login" << endl;
        cout << "2. Register" << endl;
        cout << "3. Exit" << endl;
        int choice;
        cin >> choice;
        cin.ignore();
        prompt = static_cast<PrimaryPrompt>(choice - 1);
        switch (prompt) {
            case LOGIN: {
                cout << "Login selected." << endl;
                if (buyers.empty()) {
                    cout << "No buyers registered. Please register first." << endl;
                    break;
                }
                string loginName;
                cout << "Enter your name to login: ";
                getline(cin, loginName);
                bool found = false;
                Buyer* loggedInBuyer = nullptr;
                for (auto b : buyers) {
                    if (b->getName() == loginName) {
                        cout << "Login successful! Welcome, " << b->getName() << "." << endl;
                        found = true;
                        loggedInBuyer = b;
                        break;
                    }
                }
                if (!found) {
                    cout << "Login failed. Name not found." << endl;
                } else {
                    // Subprompt setelah login
                    bool backToMain = false;
                    while (!backToMain) {
                        cout << "\nBuyer Menu:" << endl;
                        cout << "1. Check Account Status" << endl;
                        cout << "2. Bank Account Functions" << endl;
                        cout << "3. Upgrade Account to Seller" << endl;
                        cout << "4. Logout" << endl;
                        int subChoice;
                        cin >> subChoice;
                        cin.ignore();
                        string storeName, storeAddress, storePhone, storeEmail;
                        switch (subChoice) {
                            case 1:
                                cout << "Account Status:" << endl;
                                cout << "Name: " << loggedInBuyer->getName() << endl;
                                cout << "Bank Balance: " << loggedInBuyer->getAccount().getBalance() << endl;
                                break;
                            case 2: {
                                bool bankBack = false;
                                while (!bankBack) {
                                    cout << "\nBank Account Menu:" << endl;
                                    cout << "1. Check Balance" << endl;
                                    cout << "2. Deposit" << endl;
                                    cout << "3. Withdraw" << endl;
                                    cout << "4. Back" << endl;
                                    int bankChoice;
                                    cin >> bankChoice;
                                    cin.ignore();
                                    switch (bankChoice) {
                                        case 1:
                                            cout << "Current Balance: " << loggedInBuyer->getAccount().getBalance() << endl;
                                            break;
                                        case 2: {
                                            double depositAmount;
                                            cout << "Enter deposit amount: ";
                                            cin >> depositAmount;
                                            cin.ignore();
                                            loggedInBuyer->getAccount().addBalance(depositAmount);
                                            cout << "Deposit successful. New balance: " << loggedInBuyer->getAccount().getBalance() << endl;
                                            break;
                                        }
                                        case 3: {
                                            double withdrawAmount;
                                            cout << "Enter withdraw amount: ";
                                            cin >> withdrawAmount;
                                            cin.ignore();
                                            if (loggedInBuyer->getAccount().withdrawBalance(withdrawAmount)) {
                                                cout << "Withdraw successful. New balance: " << loggedInBuyer->getAccount().getBalance() << endl;
                                            } else {
                                                cout << "Withdraw failed. Insufficient funds." << endl;
                                            }
                                            break;
                                        }
                                        case 4:
                                            bankBack = true;
                                            break;
                                        default:
                                            cout << "Invalid option." << endl;
                                            break;
                                    }
                                }
                                break;
                            }
                            case 3:
                                cout << "Upgrade to Seller selected." << endl;
                                if (&(loggedInBuyer->getAccount()) == nullptr) {
                                    cout << "You must have a bank account to upgrade to Seller!" << endl;
                                    break;
                                }
                                cout << "Enter store name: ";
                                getline(cin, storeName);
                                cout << "Enter store address: ";
                                getline(cin, storeAddress);
                                cout << "Enter store phone number: ";
                                getline(cin, storePhone);
                                cout << "Enter store email: ";
                                getline(cin, storeEmail);
                                cout << "Seller account created and linked to Buyer!" << endl;
                                break;
                            case 4:
                                cout << "Logging out..." << endl;
                                backToMain = true;
                                break;
                            default:
                                cout << "Invalid option." << endl;
                                break;
                        }
                    }
                }

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
            }
            case REGISTER: {

                cout << "Register selected." << endl;
                string name, address, phone, email;
                double initialDeposit;
                cout << "Enter your name: ";
                getline(cin, name);
                cout << "Enter your home address: ";
                getline(cin, address);
                cout << "Enter your phone number: ";
                getline(cin, phone);
                cout << "Enter your email: ";
                getline(cin, email);
                cout << "Enter initial deposit for bank account: ";
                cin >> initialDeposit;
                cin.ignore();
                BankCustomer* newBankAcc = new BankCustomer(nextBankId++, name, initialDeposit);
                Buyer* newBuyer = new Buyer(nextBuyerId++, name, *newBankAcc);
                buyers.push_back(newBuyer);
                cout << "Buyer account created!\n";
                cout << "Do you want to register as Seller too? (y/n): ";
                char sellerChoice;
                cin >> sellerChoice;
                cin.ignore();
                if (sellerChoice == 'y' || sellerChoice == 'Y') {
                    string storeName, storeAddress, storePhone, storeEmail;
                    cout << "Enter store name: ";
                    getline(cin, storeName);
                    cout << "Enter store address: ";
                    getline(cin, storeAddress);
                    cout << "Enter store phone number: ";
                    getline(cin, storePhone);
                    cout << "Enter store email: ";
                    getline(cin, storeEmail);
                    cout << "Seller account created and linked to Buyer!\n";
                    // Subprompt Seller menu
                    bool backToMainSeller = false;
                    while (!backToMainSeller) {
                        cout << "\nSeller Menu:" << endl;
                        cout << "1. Check Store Info" << endl;
                        cout << "2. Add Item to Inventory" << endl;
                        cout << "3. Remove Item from Inventory" << endl;
                        cout << "4. View Orders" << endl;
                        cout << "5. Logout" << endl;
                        int sellerChoiceMenu;
                        cin >> sellerChoiceMenu;
                        cin.ignore();
                        switch (sellerChoiceMenu) {
                            case 1:
                                cout << "Store Info:" << endl;
                                cout << "Store Name: " << storeName << endl;
                                cout << "Store Address: " << storeAddress << endl;
                                cout << "Store Phone: " << storePhone << endl;
                                cout << "Store Email: " << storeEmail << endl;
                                break;
                            case 2:
                                break;
                            case 3:
                                break;
                            case 4:
                                break;
                            case 5:
                                cout << "Logging out from Seller..." << endl;
                                backToMainSeller = true;
                                break;
                            default:
                                cout << "Invalid option." << endl;
                                break;
                        }
                    }
                } else {
                    cout << "Registration complete. You are now logged in as Buyer." << endl;
                }

                /* if register is selected then went throuhh registration process:
                1. Create a new Buyer Account
                Must provides: Name, Home Address, Phone number, Email
                2. Option to create a Seller Account (will be linked to Buyer account)
                Must Provides 1: Home Address, Phone number, Email
                Must provides 2: Store Name, Store Address, Store Phone number, Store Email
                Must provides 3: initial deposit amount
                After finished immediately logged in as Buyer or Seller
                */
                break;
            }
            case EXIT:
                cout << "Exiting." << std::endl;
                break;
            default:
                cout << "Invalid option." << endl;
                break;
        }
    cout << endl;
    
    }
        //BankCustomer customer1(1, "Alice", 1000.0);
        //Buyer buyer1(1, customer1.getName(), customer1);
    return 1;
}

   
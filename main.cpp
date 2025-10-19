#include <iostream>
#include "bank_customer.h"
#include "buyer.h"
#include "seller.h"
#include "buyer_menu.h"
#include "seller_menu.h"
#include <memory>
#include <vector>
#include <string>



enum PrimaryPrompt{LOGIN, REGISTER, EXIT, ADMIN_LOGIN};
enum RegisterPrompt{CREATE_BUYER, CREATE_SELLER, BACK};
using namespace std;

// Find buyer by name and email for login
Buyer* findBuyer(const string &name, const string &email, std::vector<Buyer> &buyers) {
    for (auto &buyer : buyers) {
        if (buyer.getName() == name && buyer.getEmail() == email) {
            return &buyer;
        }
    }
    return nullptr;
}

Buyer createBuyer(BankCustomer *account);
BankCustomer* createBankCustomer(int id, string name, double balance, std::vector<std::unique_ptr<BankCustomer>> &store);
BankCustomer* verifyBankAccount(const string &name, const std::vector<std::unique_ptr<BankCustomer>> &store);
Buyer* findBuyer(const string &name, const string &email, std::vector<Buyer> &buyers);

int main() {
    //create a loop prompt 
    PrimaryPrompt prompt = LOGIN;
    RegisterPrompt regPrompt = CREATE_BUYER;
    const string ADMIN_USERNAME = "root";
    const string ADMIN_PASSWORD = "toor";
    string username, password;

    // Login state
    UserRole currentRole = UserRole::NONE;
    Buyer* loggedInBuyer = nullptr;
    seller* loggedInSeller = nullptr;

    // storage to own created accounts and buyers so pointers/references remain valid
    std::vector<std::unique_ptr<BankCustomer>> bankAccounts;
    std::vector<Buyer> buyers;
    int bankAccountID = 0;
    

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
            case LOGIN: {
                string email;
                Buyer* buyer = nullptr;
                if (currentRole != UserRole::NONE) {
                    cout << "Already logged in. Please logout first." << endl;
                    break;
                }
                cout << "Login selected." << endl;
                cout << "Enter your name: ";
                cin.ignore();
                getline(cin, username);
                cout << "Enter your email: ";
                getline(cin, email);
                buyer = findBuyer(username, email, buyers);
                if (buyer) {
                    loggedInBuyer = buyer;
                    currentRole = UserRole::BUYER;
                    // Check if this buyer is also a seller
                    seller* foundSeller = nullptr;
                    for (auto &sptr : g_sellers) {
                        if (sptr && sptr->getBuyer().getEmail() == buyer->getEmail()) {
                            foundSeller = sptr.get();
                            break;
                        }
                    }
                    if (foundSeller) {
                        loggedInSeller = foundSeller;
                        currentRole = UserRole::SELLER;
                        cout << "Welcome, Seller! You have access to both seller and buyer features." << endl;
                        bool stay = true;
                        while (stay) {
                            cout << "\nChoose mode:\n1. Seller Menu\n2. Buyer Menu\n3. Logout" << endl;
                            int modeChoice; cin >> modeChoice;
                            if (modeChoice == 1) {
                                handleSellerMenu(loggedInSeller);
                            } else if (modeChoice == 2) {
                                handleBuyerMenu(loggedInBuyer, currentRole);
                            } else if (modeChoice == 3) {
                                stay = false;
                                loggedInSeller = nullptr;
                                loggedInBuyer = nullptr;
                                currentRole = UserRole::NONE;
                            } else {
                                cout << "Invalid choice." << endl;
                            }
                        }
                    } else {
                        cout << "Welcome, Buyer!" << endl;
                        handleBuyerMenu(loggedInBuyer, currentRole);
                        loggedInBuyer = nullptr;
                        currentRole = UserRole::NONE;
                    }
                } else {
                    cout << "Buyer not found. Please register first." << endl;
                }
                break;
            }
                /* if Login is selected, based on authority then provide options:
                assume user is logged in as Buyer for now


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
            case REGISTER: {
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

                                BankCustomer* newBankAccount = createBankCustomer(bankAccountID, username, initialDeposit, bankAccounts);
                                cout << "\nBank Account created successfully.\n" << endl;
                                cout << "Creating Buyer Account now." << endl;
                                Buyer newBuyer = createBuyer(newBankAccount);
                                buyers.push_back(std::move(newBuyer));
                                // Auto-login after creation
                                loggedInBuyer = &buyers.back();
                                currentRole = UserRole::BUYER;
                                cout << "Buyer account created and logged in successfully!" << endl;
                                handleBuyerMenu(loggedInBuyer, currentRole);
                                
                            }

                            else if (hasBankAccount == 'y' || hasBankAccount == 'Y')
                            {
                                cout << "Verifying Bank Account." << endl;
                                cout << "Enter Name for Bank Account: ";
                                getline(cin, username);
                                BankCustomer* existingAccount = verifyBankAccount(username, bankAccounts);
                                if (existingAccount) {
                                    cout << "Bank account found. Account details:" << endl;
                                    existingAccount->printInfo();
                                    cout << "\nCreating Buyer Account now." << endl;
                                    Buyer newBuyer = createBuyer(existingAccount);
                                    buyers.push_back(std::move(newBuyer));
                                                    cout << "Buyer account created successfully." << endl;
                                    // Auto-login after creation
                                    loggedInBuyer = &buyers.back();
                                    currentRole = UserRole::BUYER;
                                    cout << "Logged in as " << loggedInBuyer->getName() << endl;
                                    handleBuyerMenu(loggedInBuyer, currentRole);
                                } else {
                                    cout << "Bank account NOT found. Please create a new bank account first." << endl;
                                }
                            }
                            break;
                        }

                        case CREATE_SELLER: {
                            cout << "Create Seller Account selected." << endl;
                            cout << "Enter your buyer account details to continue..." << endl;
                            cout << "Enter your name: ";
                            cin.ignore();
                            getline(cin, username);
                            cout << "Enter your email: ";
                            string email;
                            getline(cin, email);

                            Buyer* existingBuyer = findBuyer(username, email, buyers);
                            if (!existingBuyer) {
                                cout << "\nBuyer account not found! You need to create a buyer account first." << endl;
                                cout << "Do you want to create a buyer account now? (y/n): ";
                                char choice;
                                cin >> choice;
                                if (choice == 'y' || choice == 'Y') {
                                    regPrompt = CREATE_BUYER;
                                }
                                break;
                            }

                            if (!existingBuyer->getAccount()) {
                                cout << "\nError: Your buyer account needs to have a bank account linked first!" << endl;
                                break;
                            }

                            // Collect seller information
                            string sellerName, storeName, storeAddress, storePhone, storeEmail;
                            double initialDeposit;
                            static int sellerIdCounter = 0;

                            cout << "\nEnter Seller Details:" << endl;
                            cout << "Enter Seller Name: ";
                            getline(cin, sellerName);
                            cout << "Enter Store Name: ";
                            getline(cin, storeName);
                            cout << "Enter Store Address: ";
                            getline(cin, storeAddress);
                            cout << "Enter Store Phone: ";
                            getline(cin, storePhone);
                            cout << "Enter Store Email: ";
                            getline(cin, storeEmail);
                            cout << "Enter Initial Store Balance: $";
                            cin >> initialDeposit;
                            cin.ignore();

                            if (initialDeposit <= 0) {
                                cout << "Error: Initial deposit must be positive!" << endl;
                                break;
                            }

                            // Withdraw initial deposit from buyer's bank account
                            if (!existingBuyer->getAccount()->withdrawBalance(initialDeposit)) {
                                cout << "Error: Insufficient funds in your bank account!" << endl;
                                break;
                            }

                            // Create seller account
                            sellerIdCounter++;
                            Buyer buyerCopy(*existingBuyer); // Create a copy of the buyer
                            seller* newSeller = new seller(
                                buyerCopy,
                                sellerIdCounter,
                                sellerName,
                                storeName,
                                storeAddress,
                                storePhone,
                                storeEmail,
                                initialDeposit
                            );

                            // Auto-login as seller
                            loggedInSeller = newSeller;
                            currentRole = UserRole::SELLER;
                            cout << "\nSeller account created successfully!" << endl;
                            cout << "Logged in as seller." << endl;
                            newSeller->printStoreInfo();
                            break;
                        }
                        case BACK:
                            cout << "Back selected." << endl;
                            break;
                        default:
                            cout << "Invalid option." << endl;
                            break;
                    }
                }
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
}


Buyer createBuyer(BankCustomer *account)
{
    string username, password, address, phoneNumber, email;
    static int buyerIdCounter = 0;

    cout << "Enter Name: ";
    getline(cin, username);
    cout << "Enter Address: ";
    getline(cin, address);
    cout << "Enter Phone Number: ";
    getline(cin, phoneNumber);
    cout << "Enter Email: ";
    getline(cin, email);
    buyerIdCounter++;
    Buyer newBuyer(buyerIdCounter, username, address, email, phoneNumber, account);
    return newBuyer;
}

BankCustomer* createBankCustomer(int id, string name, double balance, std::vector<std::unique_ptr<BankCustomer>> &store)
{
    auto bc = std::make_unique<BankCustomer>(id, name, balance);
    BankCustomer* ptr = bc.get();
    store.push_back(std::move(bc));
    return ptr;
}

BankCustomer* verifyBankAccount(const string &name, const std::vector<std::unique_ptr<BankCustomer>> &store)
{
    for (const auto &p : store) {
        if (p && p->getName() == name) return p.get();
    }
    return nullptr;
}
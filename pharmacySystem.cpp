#include "pharmacySystem.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <iomanip>

using namespace std;

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (string::npos == first) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

void clearInputBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getIntInput() {
    int value;
    while (!(cin >> value)) {
        cout << "  > Invalid input. Enter a number: ";
        cin.clear();
        clearInputBuffer();
    }
    clearInputBuffer(); 
    return value;
}

double getDoubleInput() {
    double value;
    while (!(cin >> value)) {
        cout << "  > Invalid input. Enter a price: ";
        cin.clear();
        clearInputBuffer();
    }
    clearInputBuffer();
    return value;
}

Date getCurrentDate() {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    return Date(now->tm_mon + 1, now->tm_year + 1900);
}

bool isValidDate(int m, int y) {
    if (m < 1 || m > 12) return false;
    if (y < 2000 || y > 3000) return false; 
    return true;
}

bool isExpired(const Date& expiry) {
    Date now = getCurrentDate();
    if (expiry.year < now.year) return true;
    if (expiry.year == now.year && expiry.month < now.month) return true;
    return false;
}

bool compareDates(const Date& date1, const Date& date2) {
    if (date1.year != date2.year) return date1.year > date2.year;
    return date1.month > date2.month;
}

string extractToken(const string& line, int& pos, char delim = '|') {
    string token;
    while (pos < (int)line.length() && line[pos] != delim) {
        token += line[pos++];
    }
    pos++; 
    return trim(token); 
}

PharmacyManagementSystem::PharmacyManagementSystem()
    : medicineCount(0), supplierCount(0), billCount(0),
      userCount(0), currentUserId(-1), isLoggedIn(false) {

    srand((unsigned)time(nullptr));
    for (int i = 0; i < MAX_USERS; i++) pharmacistSales[i] = 0.0;

    if (!loadDataFromFile()) {
        preloadData();
    }
}

void PharmacyManagementSystem::preloadData() {
    cout << "Initializing system with sample data...\n";
    
    users[userCount++] = User(111111, "Admin", "admin", "Manager");
    users[userCount++] = User(222222, "Pharma", "1234", "Pharmacist");
    
    medicines[medicineCount++] = Medicine(101, "Panadol", "Painkiller", 50, 15.0, Date(12, 2026));
    medicines[medicineCount++] = Medicine(102, "Brufen", "Painkiller", 4, 25.5, Date(11, 2025)); 
    medicines[medicineCount++] = Medicine(103, "Augmentin", "Antibiotic", 20, 85.0, Date(1, 2024)); 
    medicines[medicineCount++] = Medicine(104, "Omega3", "Vitamins", 100, 150.0, Date(5, 2027));

    saveDataToFile();
}

int PharmacyManagementSystem::findMedicineIndexById(int id) {
    for (int i = 0; i < medicineCount; i++)
        if (medicines[i].id == id) return i;
    return -1;
}

int PharmacyManagementSystem::generateUserId() {
    return 100000 + rand() % 900000;
}

bool PharmacyManagementSystem::registerUser() {
    if (userCount >= MAX_USERS) { cout << "User limit reached.\n"; return false; }

    string name, password, role;
    cout << "Enter Name: "; getline(cin, name);
    name = trim(name);
    if (name.empty()) { cout << "Name cannot be empty.\n"; return false; }

    for (int i = 0; i < userCount; i++)
        if (users[i].name == name) { cout << "User exists.\n"; return false; }

    cout << "Enter Password: "; getline(cin, password);
    cout << "Enter Role (Manager/Pharmacist): "; getline(cin, role);
    role = trim(role);

    if (role != "Manager" && role != "Pharmacist") {
        cout << "Invalid role.\n"; return false;
    }

    users[userCount] = User(generateUserId(), name, password, role);
    pharmacistSales[userCount] = 0.0;
    userCount++;
    saveDataToFile();
    cout << "Registered. ID: " << users[userCount-1].id << "\n";
    return true;
}

bool PharmacyManagementSystem::login() {
    string name, pass;
    cout << "Name: "; getline(cin, name);
    cout << "Password: "; getline(cin, pass);
    name = trim(name);

    for (int i = 0; i < userCount; i++)
        if (users[i].name == name && users[i].password == pass) {
            currentUserId = i;
            currentUserRole = users[i].role;
            isLoggedIn = true;
            cout << "Login successful.\n";
            return true;
        }

    cout << "Invalid credentials.\n";
    return false;
}

void PharmacyManagementSystem::logout() {
    currentUserId = -1;
    currentUserRole = "";
    isLoggedIn = false;
    saveDataToFile();
}

void PharmacyManagementSystem::managerMenu() {
    int choice;
    do {
        cout << "\n=== Manager Menu ===\n"
             << "1. Add Medicine\n"
             << "2. Update Stock\n"
             << "3. Low Stock Report\n"
             << "4. Expiry Report\n"
             << "5. Total Sales Report\n"
             << "6. Factory Reset\n"
             << "0. Logout\n"
             << "Choice: ";
        choice = getIntInput();

        switch (choice) {
            case 1: addMedicine(); break;
            case 2: updateStock(); break;
            case 3: viewLowStockReport(); break;
            case 4: viewExpiryReport(); break;
            case 5: viewTotalSalesReport(); break;
            case 6: resetSystem(); break;
            case 0: logout(); break;
            default: cout << "Invalid choice.\n";
        }
    } while (isLoggedIn);
}

void PharmacyManagementSystem::addMedicine() {
    if (medicineCount >= MAX_MEDICINES) { cout << "Inventory full.\n"; return; }
    Medicine m;
    cout << "ID: "; m.id = getIntInput();
    if (m.id <= 0 || findMedicineIndexById(m.id) != -1) { cout << "Invalid/Duplicate ID.\n"; return; }

    cout << "Name: "; getline(cin, m.name);
    m.name = trim(m.name);
    cout << "Category: "; getline(cin, m.category);
    m.category = trim(m.category);
    cout << "Quantity: "; m.quantity = getIntInput();
    cout << "Price: "; m.price = getDoubleInput();
    cout << "Expiry Month (1-12): "; m.expiryDate.month = getIntInput();
    cout << "Expiry Year (YYYY): "; m.expiryDate.year = getIntInput();

    if (!isValidDate(m.expiryDate.month, m.expiryDate.year)) { cout << "Invalid Date.\n"; return; }

    medicines[medicineCount++] = m;
    saveDataToFile();
    cout << "Medicine added.\n";
}

void PharmacyManagementSystem::updateStock() {
    cout << "Medicine ID: ";
    int id = getIntInput();
    int idx = findMedicineIndexById(id);
    if (idx == -1) { cout << "Not found.\n"; return; }

    cout << "Current Qty: " << medicines[idx].quantity << "\nAdd: ";
    int qty = getIntInput();
    if (qty < 0) return;

    medicines[idx].quantity += qty;
    saveDataToFile();
    cout << "Stock updated.\n";
}

void PharmacyManagementSystem::viewLowStockReport() {
    cout << "\n--- Low Stock (<= 5) ---\n";
    cout << left << setw(6) << "ID" << setw(20) << "Name" << "Qty\n";
    cout << "--------------------------------\n";
    bool found = false;
    for (int i = 0; i < medicineCount; i++)
        if (medicines[i].quantity <= 5) {
            cout << left << setw(6) << medicines[i].id << setw(20) << medicines[i].name << medicines[i].quantity << "\n";
            found = true;
        }
    if (!found) cout << "No low stock items.\n";
}

void PharmacyManagementSystem::viewExpiryReport() {
    cout << "\n--- Expired Medicines ---\n";
    cout << left << setw(6) << "ID" << setw(20) << "Name" << "Exp Date\n";
    cout << "--------------------------------\n";
    bool found = false;
    for (int i = 0; i < medicineCount; i++) {
        if (isExpired(medicines[i].expiryDate)) {
            cout << left << setw(6) << medicines[i].id 
                 << setw(20) << medicines[i].name 
                 << medicines[i].expiryDate.month << "/" << medicines[i].expiryDate.year << " (Expired)\n";
            found = true;
        }
    }
    if (!found) cout << "No expired items.\n";
}

void PharmacyManagementSystem::viewTotalSalesReport() {
    double total = 0;
    for (int i = 0; i < userCount; i++) total += pharmacistSales[i];
    cout << "\nTotal Lifetime Sales: " << fixed << setprecision(2) << total << " EGP\n";
}

void PharmacyManagementSystem::resetSystem() {
    cout << "WARNING: Deleting all data. Confirm (y/n): ";
    string c; getline(cin, c);
    if (c != "y") return;
    
    ofstream out("pharmacy_data.txt", ios::trunc); out.close();
    medicineCount = 0; billCount = 0; userCount = 0; supplierCount = 0;
    for(int i=0; i<MAX_USERS; i++) pharmacistSales[i] = 0.0;
    cout << "System reset. Logging out...\n";
    logout();
}

void PharmacyManagementSystem::pharmacistMenu() {
    int choice;
    do {
        cout << "\n--- Pharmacist Menu ---\n"
             << "1. Search Medicine\n"
             << "2. Create Bill (Sell)\n"
             << "3. Check Price\n"
             << "0. Logout\n"
             << "Choice: ";
        choice = getIntInput();
        switch (choice) {
            case 1: searchMedicine(); break;
            case 2: createBill(); break;
            case 3: checkItemPrice(); break;
            case 0: logout(); break;
            default: cout << "Invalid choice.\n";
        }
    } while (isLoggedIn);
}

void PharmacyManagementSystem::searchMedicine() {
    cout << "Search by:\n1. ID\n2. Name or Category\nChoice: ";
    int c = getIntInput();
    if (c == 1) {
        cout << "ID: "; int id = getIntInput();
        int idx = findMedicineIndexById(id);
        if (idx != -1) displayMedicine(medicines[idx]); else cout << "Not found.\n";
    } else if (c == 2) {
        cout << "Keyword: "; string key; getline(cin, key); key = trim(key);
        bool found = false;
        for (int i = 0; i < medicineCount; i++) {
            if (medicines[i].name.find(key) != string::npos || medicines[i].category.find(key) != string::npos) {
                displayMedicine(medicines[i]); found = true;
            }
        }
        if (!found) cout << "No matches.\n";
    }
}

void PharmacyManagementSystem::checkItemPrice() {
    cout << "ID: "; int id = getIntInput();
    int idx = findMedicineIndexById(id);
    if(idx!=-1) cout << medicines[idx].name << ": " << medicines[idx].price << " EGP\n";
    else cout << "Not found.\n";
}

void PharmacyManagementSystem::createBill() {
    if (billCount >= MAX_BILLS) return;
    Bill b(billCount + 1);
    b.pharmacistName = users[currentUserId].name;
    b.date = getCurrentDate();

    cout << "Customer Name: "; getline(cin, b.customerName);
    if(trim(b.customerName).empty()) b.customerName = "Guest";

    while (b.medicineCount < MAX_MEDICINES_PER_BILL) {
        cout << "Medicine ID (0 to finish): "; int id = getIntInput();
        if (id == 0) break;
        int idx = findMedicineIndexById(id);
        if (idx == -1) { cout << "Invalid ID.\n"; continue; }
        
        Medicine& m = medicines[idx];
        if (isExpired(m.expiryDate)) { cout << "Expired!\n"; continue; }
        if (m.quantity <= 0) { cout << "Out of stock.\n"; continue; }

        cout << "Avail: " << m.quantity << " | Price: " << m.price << " | Sell Qty: ";
        int qty = getIntInput();
        if (qty <= 0 || qty > m.quantity) continue;

        Medicine sold = m; sold.quantity = qty;
        b.medicinesSold[b.medicineCount++] = sold;
        b.totalPrice += qty * m.price; 
        m.quantity -= qty; 
        cout << "Added.\n";
    }

    if (b.medicineCount > 0) {
        double discount = 0.0;
        if (b.totalPrice > 500.0) {
            discount = b.totalPrice * 0.10;
            cout << "\n*** BONUS: 10% Discount Applied! ***\n";
        }
        
        double finalPrice = b.totalPrice - discount;
        bills[billCount++] = b;
        pharmacistSales[currentUserId] += finalPrice;

        cout << "\n=== RECEIPT #" << b.id << " ===\nCustomer: " << b.customerName << "\n";
        for(int i=0; i<b.medicineCount; i++) {
            cout << left << setw(15) << b.medicinesSold[i].name << " x" << b.medicinesSold[i].quantity 
                 << " (" << (b.medicinesSold[i].quantity * b.medicinesSold[i].price) << ")\n";
        }
        cout << "---------------------\nSubtotal: " << b.totalPrice << "\nDiscount: -" << discount 
             << "\nTOTAL:    " << finalPrice << " EGP\n=====================\n";
        
        saveDataToFile();
    } else { cout << "Cancelled.\n"; }
}

void PharmacyManagementSystem::displayMedicine(const Medicine& m) {
    cout << left << setw(6) << m.id << "| " << setw(15) << m.name 
         << "| " << setw(12) << m.category << "| Qty:" << setw(4) << m.quantity 
         << "| " << fixed << setprecision(2) << m.price << " EGP | Exp:" 
         << m.expiryDate.month << "/" << m.expiryDate.year << "\n";
}

void PharmacyManagementSystem::displayBill(const Bill& b) {
    cout << "Bill #" << b.id << " Total: " << b.totalPrice << "\n";
}

bool PharmacyManagementSystem::saveDataToFile() {
    ofstream out("pharmacy_data.txt");
    if (!out) return false;

    out << "COUNTS|" << medicineCount << "|" << supplierCount << "|" << billCount << "|" << userCount << "\n";

    out << "\n=== MEDICINES INVENTORY ===\n";
    out << left << setw(6) << "ID" << " | " << setw(20) << "Name" << " | " << setw(15) << "Category" 
        << " | " << setw(5) << "Qty" << " | " << setw(8) << "Price" << " | " << "Exp(M|Y)\n";
    out << string(80, '-') << "\n";

    for (int i = 0; i < medicineCount; i++) {
        out << left << setw(6) << medicines[i].id << " | " 
            << setw(20) << medicines[i].name << " | "
            << setw(15) << medicines[i].category << " | "
            << setw(5) << medicines[i].quantity << " | "
            << setw(8) << fixed << setprecision(2) << medicines[i].price << " | "
            << medicines[i].expiryDate.month << "|" << medicines[i].expiryDate.year << "\n";
    }

    out << "\n=== REGISTERED USERS ===\n";
    out << left << setw(6) << "ID" << " | " << setw(20) << "Name" << " | " 
        << setw(15) << "Password" << " | " << "Role\n";
    out << string(60, '-') << "\n";

    for (int i = 0; i < userCount; i++) {
        out << left << setw(6) << users[i].id << " | " << setw(20) << users[i].name << " | "
            << setw(15) << users[i].password << " | " << users[i].role << "\n";
    }

    out << "\n=== SALES DATA ===\nDATA";
    for (int i = 0; i < userCount; i++) out << "|" << pharmacistSales[i];
    out << "\n";

    return true;
}

bool PharmacyManagementSystem::loadDataFromFile() {
    ifstream in("pharmacy_data.txt");
    if (!in) return false;

    string line;
    if(!getline(in, line)) return false; 
    int pos = (line.find("COUNTS|") != string::npos) ? 7 : 0;

    int mCount = atoi(extractToken(line, pos).c_str());
    extractToken(line, pos); 
    extractToken(line, pos); 
    billCount = 0; 
    int uCount = atoi(extractToken(line, pos).c_str());

    mCount = min(mCount, MAX_MEDICINES);
    uCount = min(uCount, MAX_USERS);
    medicineCount = 0; userCount = 0;

    while(medicineCount < mCount && getline(in, line)) {
        line = trim(line);
        if(line.empty() || line[0] == '=' || line[0] == '-' || line.find("ID") == 0) continue;
        pos = 0;
        Medicine m;
        m.id = atoi(extractToken(line, pos).c_str());
        m.name = extractToken(line, pos);
        m.category = extractToken(line, pos);
        m.quantity = atoi(extractToken(line, pos).c_str());
        m.price = atof(extractToken(line, pos).c_str());
        m.expiryDate.month = atoi(extractToken(line, pos).c_str());
        m.expiryDate.year = atoi(extractToken(line, pos).c_str());
        medicines[medicineCount++] = m;
    }

    while(userCount < uCount && getline(in, line)) {
        line = trim(line);
        if(line.empty() || line[0] == '=' || line[0] == '-' || line.find("ID") == 0) continue;
        pos = 0;
        User u;
        u.id = atoi(extractToken(line, pos).c_str());
        u.name = extractToken(line, pos);
        u.password = extractToken(line, pos);
        u.role = extractToken(line, pos);
        users[userCount++] = u;
    }

    while(getline(in, line)) {
        if(line.find("DATA") != string::npos) {
            pos = (line.find("DATA|") != string::npos) ? 5 : 4;
            for(int i=0; i<userCount; i++) {
                string val = extractToken(line, pos);
                pharmacistSales[i] = val.empty() ? 0.0 : atof(val.c_str());
            }
            break;
        }
    }
    return true;
}
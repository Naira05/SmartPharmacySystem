#include "pharmacySystem.h"
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

/* ---------- Utility functions (global) ---------- */

Date getCurrentDate() {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    int month = now->tm_mon + 1;
    int year = now->tm_year + 1900;
    return Date(month, year);
}

bool compareDates(const Date& date1, const Date& date2) {
    if (date1.year < date2.year) return true;
    if (date1.year > date2.year) return false;
    return date1.month <= date2.month;
}

int dateDifference(const Date& date1, const Date& date2) {
    return (date2.year - date1.year) * 12 + (date2.month - date1.month);
}

/* ---------- Private helper implementations ---------- */

int PharmacyManagementSystem::findMedicineIndexById(int id) {
    for (int i = 0; i < medicineCount; ++i) {
        if (medicines[i].id == id) return i;
    }
    return -1;
}

int PharmacyManagementSystem::findUserIndexByName(const string& name) {
    for (int i = 0; i < userCount; ++i) {
        if (users[i].name == name) return i;
    }
    return -1;
}

int PharmacyManagementSystem::findUserIndexById(int id) {
    for (int i = 0; i < userCount; ++i) {
        if (users[i].id == id) return i;
    }
    return -1;
}

vector<Medicine> PharmacyManagementSystem::searchMedicinesByName(const string& query) {
    vector<Medicine> results;
    string queryLower = query;
    transform(queryLower.begin(), queryLower.end(), queryLower.begin(), ::tolower);

    for (int i = 0; i < medicineCount; ++i) {
        string nameLower = medicines[i].name;
        transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);

        if (nameLower.find(queryLower) != string::npos) {
            results.push_back(medicines[i]);
        }
    }
    return results;
}

/* ---------- Constructor ---------- */

PharmacyManagementSystem::PharmacyManagementSystem()
    : medicineCount(0), billCount(0), userCount(0),
    currentUserId(-1), currentUserRole(""), isLoggedIn(false) {
    for (int i = 0; i < MAX_USERS; ++i) pharmacistSales[i] = 0.0;
    loadDataFromFile();
}

/* ---------- User Management ---------- */

int PharmacyManagementSystem::generateUserId() {
    static int nextId = 1;
    int maxId = 0;
    for (int i = 0; i < userCount; ++i) {
        if (users[i].id > maxId) maxId = users[i].id;
    }
    if (nextId <= maxId) nextId = maxId + 1;
    return nextId++;
}

bool PharmacyManagementSystem::registerUser(const string& name, const string& password, const string& role) {
    if (userCount >= MAX_USERS) return false;
    if (name.empty() || password.empty()) return false;
    if (!(role == "Manager" || role == "Pharmacist")) return false;
    if (findUserIndexByName(name) != -1) return false;

    int id = generateUserId();
    users[userCount] = User(id, name, password, role);
    if (role == "Pharmacist") pharmacistSales[userCount] = 0.0;
    userCount++;
    saveDataToFile();
    return true;
}

bool PharmacyManagementSystem::login(const string& name, const string& password) {
    int idx = findUserIndexByName(name);
    if (idx == -1) return false;
    if (users[idx].password != password) return false;

    currentUserId = idx;
    currentUserRole = users[idx].role;
    isLoggedIn = true;
    return true;
}

void PharmacyManagementSystem::logout() {
    currentUserId = -1;
    currentUserRole = "";
    isLoggedIn = false;
    saveDataToFile();
}

string PharmacyManagementSystem::getCurrentUserName() const {
    if (currentUserId >= 0 && currentUserId < userCount) {
        return users[currentUserId].name;
    }
    return "";
}

string PharmacyManagementSystem::getCurrentUserRole() const {
    return currentUserRole;
}

/* ---------- Medicine Functions ---------- */

bool PharmacyManagementSystem::addMedicine(const string& name, const string& category,
                                           int quantity, double price, int expMonth, int expYear) {
    if (medicineCount >= MAX_MEDICINES) return false;
    if (name.empty()) return false;
    if (quantity < 0 || price < 0) return false;
    if (expMonth < 1 || expMonth > 12 || expYear < 1900) return false;

    int maxId = 0;
    for (int i = 0; i < medicineCount; ++i) {
        if (medicines[i].id > maxId) maxId = medicines[i].id;
    }
    int newId = maxId + 1;

    medicines[medicineCount] = Medicine(newId, name, category, quantity, price, Date(expMonth, expYear));
    medicineCount++;
    saveDataToFile();
    return true;
}

bool PharmacyManagementSystem::updateStock(int id, int delta) {
    int idx = findMedicineIndexById(id);
    if (idx == -1) return false;

    int newQty = medicines[idx].quantity + delta;
    if (newQty < 0) return false;

    medicines[idx].quantity = newQty;
    saveDataToFile();
    return true;
}

vector<Medicine> PharmacyManagementSystem::getMedicines() const {
    vector<Medicine> result;
    for (int i = 0; i < medicineCount; ++i) {
        result.push_back(medicines[i]);
    }
    return result;
}

vector<Medicine> PharmacyManagementSystem::getLowStockMedicines(int threshold) const {
    vector<Medicine> result;
    for (int i = 0; i < medicineCount; ++i) {
        if (medicines[i].quantity <= threshold) {
            result.push_back(medicines[i]);
        }
    }
    return result;
}

Medicine* PharmacyManagementSystem::findMedicineById(int id) {
    int idx = findMedicineIndexById(id);
    if (idx == -1) return nullptr;
    return &medicines[idx];
}

vector<Medicine> PharmacyManagementSystem::searchMedicines(const string& query) {
    if (query.empty()) {
        return getMedicines();
    }

    // Try to parse as ID
    if (all_of(query.begin(), query.end(), ::isdigit)) {
        int id = stoi(query);
        Medicine* med = findMedicineById(id);
        if (med) {
            return {*med};
        }
    }

    // Search by name
    return searchMedicinesByName(query);
}

bool PharmacyManagementSystem::isMedicineExpired(const Medicine& med) const {
    Date now = getCurrentDate();
    if (med.expiryDate.year < now.year) return true;
    if (med.expiryDate.year == now.year && med.expiryDate.month < now.month) return true;
    return false;
}

/* ---------- Bill Functions ---------- */

Bill PharmacyManagementSystem::createBill(const string& customerName,
                                          const vector<pair<int, int>>& medicineQuantities) {
    Bill newBill(billCount + 1);
    newBill.pharmacistName = getCurrentUserName();
    newBill.customerName = customerName.empty() ? "Customer" : customerName;
    newBill.date = getCurrentDate();

    for (const auto& item : medicineQuantities) {
        int medicineId = item.first;
        int quantity = item.second;

        int idx = findMedicineIndexById(medicineId);
        if (idx == -1) continue;

        Medicine& available = medicines[idx];
        if (quantity <= 0 || quantity > available.quantity) continue;
        if (isMedicineExpired(available)) continue;

        Medicine sold = available;
        sold.quantity = quantity;
        newBill.medicinesSold[newBill.medicineCount] = sold;
        newBill.medicineCount++;
        newBill.totalPrice += quantity * available.price;

        available.quantity -= quantity;
    }

    if (newBill.medicineCount > 0) {
        bills[billCount] = newBill;
        billCount++;

        if (currentUserId >= 0 && currentUserId < MAX_USERS) {
            pharmacistSales[currentUserId] += newBill.totalPrice;
        }

        saveDataToFile();
    }

    return newBill;
}

vector<Bill> PharmacyManagementSystem::getBills() const {
    vector<Bill> result;
    for (int i = 0; i < billCount; ++i) {
        result.push_back(bills[i]);
    }
    return result;
}

double PharmacyManagementSystem::calculateTotalSales() const {
    double total = 0.0;
    for (int i = 0; i < billCount; ++i) {
        total += bills[i].totalPrice;
    }
    return total;
}

double PharmacyManagementSystem::getPharmacistSales() const {
    if (currentUserId >= 0 && currentUserId < MAX_USERS) {
        return pharmacistSales[currentUserId];
    }
    return 0.0;
}

/* ---------- Utility Functions ---------- */

void PharmacyManagementSystem::displayMedicine(const Medicine& med) {
    cout << "ID: " << med.id << ", Name: " << med.name
         << ", Category: " << med.category << ", Qty: " << med.quantity
         << ", Price: $" << fixed << setprecision(2) << med.price
         << ", Expiry: " << med.expiryDate.toString() << endl;
}

void PharmacyManagementSystem::displayBill(const Bill& bill) {
    cout << "Bill ID: " << bill.id << ", Customer: " << bill.customerName
         << ", Pharmacist: " << bill.pharmacistName
         << ", Date: " << bill.date.toString()
         << ", Total: $" << fixed << setprecision(2) << bill.totalPrice << endl;
}

/* ---------- File I/O ---------- */

bool PharmacyManagementSystem::saveDataToFile() {
    ofstream fout("pharmacy_data.txt");
    if (!fout.is_open()) return false;

    fout << medicineCount << " " << billCount << " " << userCount << "\n";

    // medicines
    for (int i = 0; i < medicineCount; ++i) {
        Medicine& m = medicines[i];
        fout << m.id << "|" << m.name << "|" << m.category << "|"
             << m.quantity << "|" << m.price << "|"
             << m.expiryDate.month << "|" << m.expiryDate.year << "\n";
    }

    // bills
    for (int i = 0; i < billCount; ++i) {
        Bill& b = bills[i];
        fout << b.id << "|" << b.pharmacistName << "|" << b.customerName << "|"
             << b.medicineCount << "|" << b.totalPrice << "|"
             << b.date.month << "|" << b.date.year;

        for (int j = 0; j < b.medicineCount; ++j) {
            Medicine& m = b.medicinesSold[j];
            fout << "|" << m.id << "," << m.name << "," << m.category << ","
                 << m.quantity << "," << m.price << ","
                 << m.expiryDate.month << "," << m.expiryDate.year;
        }
        fout << "\n";
    }

    // users
    for (int i = 0; i < userCount; ++i) {
        User& u = users[i];
        fout << u.id << "|" << u.name << "|" << u.password << "|" << u.role << "\n";
    }

    // pharmacistSales
    fout << "PHARM_SALES";
    for (int i = 0; i < userCount; ++i) fout << "|" << pharmacistSales[i];
    fout << "\n";

    fout.close();
    return true;
}

bool PharmacyManagementSystem::loadDataFromFile() {
    ifstream fin("pharmacy_data.txt");
    if (!fin.is_open()) return false;

    medicineCount = billCount = userCount = 0;
    for (int i = 0; i < MAX_USERS; ++i) pharmacistSales[i] = 0.0;

    string line;
    if (!getline(fin, line)) { fin.close(); return false; }

    istringstream iss(line);
    int mCount, bCount, uCount;
    if (!(iss >> mCount >> bCount >> uCount)) {
        fin.close();
        return false;
    }

    // Load medicines
    for (int i = 0; i < mCount; ++i) {
        if (!getline(fin, line)) break;

        vector<string> parts;
        stringstream ss(line);
        string token;
        while (getline(ss, token, '|')) {
            parts.push_back(token);
        }

        if (parts.size() >= 7) {
            int id = stoi(parts[0]);
            string name = parts[1];
            string category = parts[2];
            int quantity = stoi(parts[3]);
            double price = stod(parts[4]);
            int month = stoi(parts[5]);
            int year = stoi(parts[6]);

            medicines[medicineCount++] = Medicine(id, name, category, quantity, price, Date(month, year));
        }
    }

    // Load bills
    for (int i = 0; i < bCount; ++i) {
        if (!getline(fin, line)) break;

        vector<string> parts;
        stringstream ss(line);
        string token;
        while (getline(ss, token, '|')) {
            parts.push_back(token);
        }

        if (parts.size() >= 7) {
            int id = stoi(parts[0]);
            string pharmacist = parts[1];
            string customer = parts[2];
            int medCount = stoi(parts[3]);
            double total = stod(parts[4]);
            int month = stoi(parts[5]);
            int year = stoi(parts[6]);

            Bill bill(id, pharmacist, customer, Date(month, year));
            bill.totalPrice = total;

            for (int j = 0; j < medCount && (7 + j) < parts.size(); ++j) {
                string medStr = parts[7 + j];
                vector<string> medParts;
                stringstream medSS(medStr);
                string medToken;
                while (getline(medSS, medToken, ',')) {
                    medParts.push_back(medToken);
                }

                if (medParts.size() >= 7) {
                    int medId = stoi(medParts[0]);
                    string medName = medParts[1];
                    string medCat = medParts[2];
                    int medQty = stoi(medParts[3]);
                    double medPrice = stod(medParts[4]);
                    int medMonth = stoi(medParts[5]);
                    int medYear = stoi(medParts[6]);

                    Medicine med(medId, medName, medCat, medQty, medPrice, Date(medMonth, medYear));
                    bill.medicinesSold[bill.medicineCount++] = med;
                }
            }

            bills[billCount++] = bill;
        }
    }

    // Load users
    for (int i = 0; i < uCount; ++i) {
        if (!getline(fin, line)) break;

        vector<string> parts;
        stringstream ss(line);
        string token;
        while (getline(ss, token, '|')) {
            parts.push_back(token);
        }

        if (parts.size() >= 4) {
            int id = stoi(parts[0]);
            string name = parts[1];
            string password = parts[2];
            string role = parts[3];

            users[userCount++] = User(id, name, password, role);
        }
    }

    // Load pharmacist sales
    if (getline(fin, line)) {
        if (line.rfind("PHARM_SALES", 0) == 0) {
            vector<string> parts;
            stringstream ss(line);
            string token;
            while (getline(ss, token, '|')) {
                parts.push_back(token);
            }

            for (int i = 1; i < parts.size() && (i-1) < userCount; ++i) {
                pharmacistSales[i-1] = stod(parts[i]);
            }
        }
    }

    fin.close();
    return true;
}

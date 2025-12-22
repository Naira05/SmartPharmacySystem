#ifndef PHARMACY_MANAGEMENT_SYSTEM_H
#define PHARMACY_MANAGEMENT_SYSTEM_H

#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <vector>
#include <utility>
#include <algorithm>
#include <sstream>
#include <cctype>

// Constants
const int MAX_MEDICINES = 1000;
const int MAX_SUPPLIERS = 100;
const int MAX_BILLS = 500;
const int MAX_USERS = 50;
const int MAX_MEDICINES_PER_SUPPLIER = 50;
const int MAX_MEDICINES_PER_BILL = 20;

// Date structure for expiry and bill dates
struct Date {
    int month;
    int year;

    Date(int m = 1, int y = 2025) : month(m), year(y) {}

    std::string toString() const {
        return std::to_string(month) + "/" + std::to_string(year);
    }
};

// Medicine structure
struct Medicine {
    int id;
    std::string name;
    std::string category;
    int quantity;
    double price;
    Date expiryDate;

    // Constructor
    Medicine(int i = 0, std::string n = "", std::string cat = "", int q = 0,
             double p = 0.0, Date exp = Date())
        : id(i), name(n), category(cat), quantity(q), price(p), expiryDate(exp) {}
};

// Bill struct
struct Bill {
    int id;
    std::string pharmacistName;
    std::string customerName;
    Medicine medicinesSold[MAX_MEDICINES_PER_BILL];
    int medicineCount;
    double totalPrice;
    Date date;

    // Constructor
    Bill(int i = 0, std::string pharm = "", std::string cust = "", Date d = Date())
        : id(i), pharmacistName(pharm), customerName(cust),
        medicineCount(0), totalPrice(0.0), date(d) {}
};

// Base User structure
struct User {
    int id;
    std::string name;
    std::string password;
    std::string role;

    // Constructor
    User(int i = 0, std::string n = "", std::string pwd = "", std::string r = "")
        : id(i), name(n), password(pwd), role(r) {}
};

// Main System Class
class PharmacyManagementSystem {
private:
    Medicine medicines[MAX_MEDICINES];
    Bill bills[MAX_BILLS];
    User users[MAX_USERS];
    double pharmacistSales[MAX_USERS];

    int medicineCount;
    int billCount;
    int userCount;

    int currentUserId;
    std::string currentUserRole;
    bool isLoggedIn;

    // Internal helpers
    int findMedicineIndexById(int id);
    int findUserIndexByName(const std::string& name);
    int findUserIndexById(int id);
    std::vector<Medicine> searchMedicinesByName(const std::string& query);

public:
    // Constructor
    PharmacyManagementSystem();

    // User Management Functions
    bool registerUser(const std::string& name, const std::string& password, const std::string& role);
    bool login(const std::string& name, const std::string& password);
    void logout();
    int generateUserId();
    std::string getCurrentUserName() const;
    std::string getCurrentUserRole() const;
    bool isLoggedInNow() const { return isLoggedIn; }

    // Medicine Functions
    bool addMedicine(const std::string& name, const std::string& category,
                     int quantity, double price, int expMonth, int expYear);
    bool updateStock(int id, int delta);
    std::vector<Medicine> getMedicines() const;
    std::vector<Medicine> getLowStockMedicines(int threshold = 5) const;
    Medicine* findMedicineById(int id);
    std::vector<Medicine> searchMedicines(const std::string& query);
    bool isMedicineExpired(const Medicine& med) const;
    int getMedicineCount() const { return medicineCount; }

    // Bill Functions
    Bill createBill(const std::string& customerName,
                    const std::vector<std::pair<int, int>>& medicineQuantities);
    std::vector<Bill> getBills() const;
    int getBillCount() const { return billCount; }
    double calculateTotalSales() const;
    double getPharmacistSales() const;
    void displayMedicine(const Medicine& med);
    void displayBill(const Bill& bill);

    // File I/O
    bool saveDataToFile();
    bool loadDataFromFile();
};

#endif // PHARMACY_MANAGEMENT_SYSTEM_H

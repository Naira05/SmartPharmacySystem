#ifndef PHARMACY_MANAGEMENT_SYSTEM_H
#define PHARMACY_MANAGEMENT_SYSTEM_H

#include <iostream>
#include <string>
#include <ctime>

using namespace std;

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
};

// Medicine structure
struct Medicine {
    int id;
    string name;
    string category; 
    int quantity;
    double price;
    Date expiryDate;
    
    // Constructor
    Medicine(int i = 0, string n = "", string cat = "", int q = 0, double p = 0.0, Date exp = Date())
        : id(i), name(n), category(cat), quantity(q), price(p), expiryDate(exp) {}
};

// Supplier struct
struct Supplier {
    int id;
    string name;
    string contactInfo;
    int medicineIDs[MAX_MEDICINES_PER_SUPPLIER];
    int medicineCount;
    
    // Constructor
    Supplier(int i = 0, string n = "", string contact = "") 
        : id(i), name(n), contactInfo(contact), medicineCount(0) {}
};

// Bill struct
struct Bill {
    int id;
    string pharmacistName;
    string customerName;
    Medicine medicinesSold[MAX_MEDICINES_PER_BILL];
    int medicineCount;
    double totalPrice;
    Date date;
    
    // Constructor
    Bill(int i = 0, string pharm = "", string cust = "", Date d = Date())
        : id(i), pharmacistName(pharm), customerName(cust), medicineCount(0), totalPrice(0.0), date(d) {}
};

// Base User structure
struct User {
    int id;
    string name;
    string password;
    string role; 
    
    // Constructor
    User(int i = 0, string n = "", string pwd = "", string r = "") 
        : id(i), name(n), password(pwd), role(r) {}
};

// Manager structure 
struct Manager : User {
    // Additional manager-specific fields can be added here
    
    Manager(int i = 0, string n = "", string pwd = "") 
        : User(i, n, pwd, "Manager") {}
};

// Pharmacist structure 
struct Pharmacist : User {
    double totalSalesAmount;
    
    Pharmacist(int i = 0, string n = "", string pwd = "", double sales = 0.0) 
        : User(i, n, pwd, "Pharmacist"), totalSalesAmount(sales) {}
};

// Main System Class
class PharmacyManagementSystem {
private:
    Medicine medicines[MAX_MEDICINES];
    Supplier suppliers[MAX_SUPPLIERS];
    Bill bills[MAX_BILLS];
    User users[MAX_USERS];
    
    int medicineCount;
    int supplierCount;
    int billCount;
    int userCount;
    
    int currentUserId;
    string currentUserRole;
    bool isLoggedIn;

public:
    // Constructor
    PharmacyManagementSystem();
    
    // User Management Functions
    bool registerUser();
    bool login();
    void logout();
    int generateUserId();
    
    // Manager Functions
    void managerMenu();
    void addMedicine();
    void updateStock();
    void viewLowStockReport();
    void viewTotalSalesReport();
    
    // Pharmacist Functions
    void pharmacistMenu();
    void searchMedicine();
    void createBill();
    void checkItemPrice();
    
    // Utility Functions
    void displayMedicine(const Medicine& med);
    void displaySupplier(const Supplier& sup);
    void displayBill(const Bill& bill);
    bool isMedicineExpired(const Medicine& med);
    double calculateTotalSales();
    
    // File I/O Functions 
    bool saveDataToFile();
    bool loadDataFromFile();
    
    // Main system loop
    void run();
};

// Utility Functions
Date getCurrentDate();
bool compareDates(const Date& date1, const Date& date2);
int dateDifference(const Date& date1, const Date& date2);

#endif 
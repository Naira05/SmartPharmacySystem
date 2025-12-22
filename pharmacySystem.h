#ifndef PHARMACY_MANAGEMENT_SYSTEM_H
#define PHARMACY_MANAGEMENT_SYSTEM_H

#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <iomanip>

using namespace std;

const int MAX_MEDICINES = 1000;
const int MAX_SUPPLIERS = 100;
const int MAX_BILLS = 500;
const int MAX_USERS = 50;
const int MAX_MEDICINES_PER_SUPPLIER = 50;
const int MAX_MEDICINES_PER_BILL = 20;

struct Date {
    int month;
    int year;
    Date(int m = 1, int y = 2025) : month(m), year(y) {}
};

struct Medicine {
    int id;
    string name;
    string category;
    int quantity;
    double price;
    Date expiryDate;

    Medicine(int i = 0, string n = "", string cat = "", int q = 0, double p = 0.0, Date exp = Date())
        : id(i), name(n), category(cat), quantity(q), price(p), expiryDate(exp) {}
};

struct Supplier {
    int id;
    string name;
    string contactInfo;
    int medicineIDs[MAX_MEDICINES_PER_SUPPLIER];
    int medicineCount;

    Supplier(int i = 0, string n = "", string contact = "")
        : id(i), name(n), contactInfo(contact), medicineCount(0) {}
};

struct Bill {
    int id;
    string pharmacistName;
    string customerName;
    Medicine medicinesSold[MAX_MEDICINES_PER_BILL];
    int medicineCount;
    double totalPrice; 
    Date date;

    Bill(int i = 0, string pharm = "", string cust = "", Date d = Date())
        : id(i), pharmacistName(pharm), customerName(cust), medicineCount(0), totalPrice(0.0), date(d) {}
};

struct User {
    int id;
    string name;
    string password;
    string role;

    User(int i = 0, string n = "", string pwd = "", string r = "")
        : id(i), name(n), password(pwd), role(r) {}
};

Date getCurrentDate();
bool isExpired(const Date& expiry);
bool compareDates(const Date& date1, const Date& date2);

class PharmacyManagementSystem {
private:
    Medicine medicines[MAX_MEDICINES];
    Supplier suppliers[MAX_SUPPLIERS];
    Bill bills[MAX_BILLS];
    User users[MAX_USERS];

    double pharmacistSales[MAX_USERS];

    int medicineCount;
    int supplierCount;
    int billCount;
    int userCount;

    int currentUserId;
    string currentUserRole;
    bool isLoggedIn;

    int findMedicineIndexById(int id);
    void preloadData(); 

public:
    PharmacyManagementSystem();

    void run();
    bool registerUser();
    bool login();
    void logout();
    int generateUserId();

    void managerMenu();
    void addMedicine();
    void updateStock();
    void viewLowStockReport();
    void viewExpiryReport(); 
    void viewTotalSalesReport();
    void resetSystem();      

    void pharmacistMenu();
    void searchMedicine();
    void createBill();       
    void checkItemPrice();

    void displayMedicine(const Medicine& med);
    void displayBill(const Bill& bill);

    bool saveDataToFile();
    bool loadDataFromFile();
};

#endif
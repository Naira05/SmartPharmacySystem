#include "pharmacySystem.h"
#ifndef PHARMACY_MANAGEMENT_SYSTEM_H
#define PHARMACY_MANAGEMENT_SYSTEM_H

#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>


int medicineCount = 0
Medicine medicines[MAX_MEDICINES];
void addMedicine (Meedicine medicines[] , &medicineCount ) {
    if (medicinecount >= MAX_MEDICINES) {
        cout << " Inventory is full! cannot add more medicines. \n  ";
        return ;
    }  
    Medicine m ; 
    cout << " Enter Medicine ID: \n ";
    cin>> m.id ;
    cout << " Enter Medicine Name: \n ";
    cin>> m.name;
    cout << " Enter Medicine Category: \n";
    cin >> m.category;
    cout << " Enter Medicine Quantity \n";
    cin >> m.quantity;
    cout << " Enter Medicine Price \n";
    cin >> m.price;
    cout << " Enter Medicine Expiry Date \n";
    cin >> m.expiryDate;
    Medicine m ( id , name , category , quantity , price , expiryDate);
  
    medicines [ medicineCount]=m ;
    medicineCount++;
    cout << " medicine added successfuly! \n"
}

void updateStock(Medicine medicines[], int medicineCount) {
    int id;
    cout << " Enter Medicine ID to Update : ";
    cin >> id;
    bool found = false;
    for (int i = 0; i < medicineCount; i++) {
        if (medicine[i].id == id) {
            found = true;
            int addQuantity;
            cout << " Enter quantity to add : ";
            cin >> addQuantity;
            medicine[i].quantity += addQuantity;
            cout << " Stock updated successfully. \n";
            cout << " new quantity of " << medicines[i].name << ": " << medicines[i].quantity << endl;
            break;
        }
    }
    if (!found) {
        cout << " Medicine not found. \n";
    }
}


void viewLowStockReport(Medicine medicines[],int medicineCount ); { 
cout << " ***** low stock report ***** \n";
bool found = false;
for (int i = 0; i < medicineCount; i++) {
    if(medicines[i].quantity <= 5) {
    found = true ;
    cout << " ID :" << medicines[i].id;
    cout << " | Name :" << medicines[i].name;
    cout << " | category :" << medicines[i].name;
    cout << " | Quantity :" << medicines[i].quantity;
    cout << " | price :" << medicines[i].price;
    cout << " | Expiry Date :" << medicines[i].expiryDate << endl;
   }
}
if (!found) {
    cout << " No medicines with low stock . \n "
     }
}

void viewTotalSalesReport(Bill Bills[], int billCount) {
double totalPrice;
     for ( int i=0 ; i< billCount ; i++){|
totalSales+= bills[i].totalPrice;
cout << " ***** Total Sales Report *****\n ";
cout << " Total Sales Amount : " << totalSales << " EGP \n"
     }
}

void managerMenu(Medicine medicines[], &medicineCount, Bill Bills[], int billCount) {
int choise ; 
do {
cout << " *****Manager Menu***** \n";
cout << " 1. Add Medicine \n";
cout << " 2. Update Stock \n";
cout << " 3. View Low stock Report \n";
cout << " 4. View Total Price Report \n";
cout << " 0. Log Out \n";
cout " Enter Choise : "
cin>> choise ;
  
switch (choise) {
case 1 :
       addMedicine(medicines ,medicineCount );
     break;
case 2:
    updatestock(medicines, medicineCount);
    break;
case 1:
    viewLowStockReport(medicines, medicineCount);
    break;
case 1:
    viewTotalsalesReport(Bills, billCount);
    break;
case 0 :
    cout << " Logging out .....\n";
    break;
default :
cout << " Invalid choise , Please try agin.\n";

  }
}
while ( choise!=0 );

}

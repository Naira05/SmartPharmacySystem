#include "pharmacySystem.h"

void PharmacyManagementSystem::run() {
    while (true) {
        cout << "\n=== Pharmacy Management System ===\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Choose an option: ";
        
        string choice; 
        getline(cin, choice);

        if (choice == "1") {
            registerUser();
        } else if (choice == "2") {
            if (login()) {
                if (currentUserRole == "Manager") {
                    managerMenu();
                }
                else if (currentUserRole == "Pharmacist") {
                    pharmacistMenu();
                }
                else {
                    cout << "Error: Role '" << currentUserRole << "' has no menu.\n";
                    logout();
                }
            }
        } else if (choice == "3") {
            cout << "Exiting. Saving data...\n";
            saveDataToFile();
            break;
        } else {
            cout << "Invalid option. Please try again.\n";
        }
    }
}

int main() {
    PharmacyManagementSystem system;
    system.run();
    return 0;
}
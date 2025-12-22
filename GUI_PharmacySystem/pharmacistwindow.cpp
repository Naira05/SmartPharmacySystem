#include "pharmacistwindow.h"
#include "medicineswindow.h"
#include "billswindow.h"
#include <QStatusBar>

PharmacistWindow::PharmacistWindow(PharmacyManagementSystem* system, QWidget *parent)
    : QMainWindow(parent), system(system) {
    setupUI();
    setWindowTitle("Pharmacy System - Pharmacist Dashboard");
    resize(800, 600);

    // Status bar
    statusBar()->showMessage(QString("Logged in as: %1 | Total Sales: $%2").arg(
                                                                               QString::fromStdString(system->getCurrentUserName())).arg(system->getPharmacistSales()));
}

void PharmacistWindow::setupUI() {
    QWidget* centralWidget = new QWidget();
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Header
    QLabel* headerLabel = new QLabel("Pharmacist Dashboard");
    headerLabel->setStyleSheet("font-size: 24px; font-weight: bold; padding: 20px;");
    headerLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(headerLabel);

    mainLayout->addSpacing(30);

    // Buttons
    QPushButton* searchButton = new QPushButton("Search Medicines");
    searchButton->setStyleSheet("padding: 15px; font-size: 16px;");
    connect(searchButton, &QPushButton::clicked, this, &PharmacistWindow::onSearchMedicinesClicked);

    QPushButton* billButton = new QPushButton("Create Bill");
    billButton->setStyleSheet("padding: 15px; font-size: 16px;");
    connect(billButton, &QPushButton::clicked, this, &PharmacistWindow::onCreateBillClicked);

    QPushButton* logoutButton = new QPushButton("Logout");
    logoutButton->setStyleSheet("padding: 10px; font-size: 14px; background-color: #f44336; color: white;");
    connect(logoutButton, &QPushButton::clicked, this, &PharmacistWindow::onLogoutClicked);

    mainLayout->addWidget(searchButton);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(billButton);
    mainLayout->addStretch();
    mainLayout->addWidget(logoutButton);
}

void PharmacistWindow::onSearchMedicinesClicked() {
    MedicinesWindow* medicinesWindow = new MedicinesWindow(system, this);
    medicinesWindow->setAttribute(Qt::WA_DeleteOnClose);
    medicinesWindow->setPharmacistMode(true);
    medicinesWindow->show();
}

void PharmacistWindow::onCreateBillClicked() {
    BillsWindow* billsWindow = new BillsWindow(system, this);
    billsWindow->setAttribute(Qt::WA_DeleteOnClose);
    billsWindow->show();
}

void PharmacistWindow::onLogoutClicked() {
    system->logout();
    close();
}

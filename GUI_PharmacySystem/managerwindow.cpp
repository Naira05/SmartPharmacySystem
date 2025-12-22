#include "managerwindow.h"
#include "medicineswindow.h"
#include "reportswindow.h"
#include <QStatusBar>

ManagerWindow::ManagerWindow(PharmacyManagementSystem* system, QWidget *parent)
    : QMainWindow(parent), system(system) {
    setupUI();
    setWindowTitle("Pharmacy System - Manager Dashboard");
    resize(800, 600);

    // Status bar
    statusBar()->showMessage(QString("Logged in as: %1 (Manager)").arg(
        QString::fromStdString(system->getCurrentUserName())));
}

void ManagerWindow::setupUI() {
    QWidget* centralWidget = new QWidget();
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Header
    QLabel* headerLabel = new QLabel("Manager Dashboard");
    headerLabel->setStyleSheet("font-size: 24px; font-weight: bold; padding: 20px;");
    headerLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(headerLabel);

    mainLayout->addSpacing(30);

    // Buttons
    QPushButton* medicinesButton = new QPushButton("Manage Medicines");
    medicinesButton->setStyleSheet("padding: 15px; font-size: 16px;");
    connect(medicinesButton, &QPushButton::clicked, this, &ManagerWindow::onManageMedicinesClicked);

    QPushButton* reportsButton = new QPushButton("View Reports");
    reportsButton->setStyleSheet("padding: 15px; font-size: 16px;");
    connect(reportsButton, &QPushButton::clicked, this, &ManagerWindow::onViewReportsClicked);

    QPushButton* logoutButton = new QPushButton("Logout");
    logoutButton->setStyleSheet("padding: 10px; font-size: 14px; background-color: #f44336; color: white;");
    connect(logoutButton, &QPushButton::clicked, this, &ManagerWindow::onLogoutClicked);

    mainLayout->addWidget(medicinesButton);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(reportsButton);
    mainLayout->addStretch();
    mainLayout->addWidget(logoutButton);
}

void ManagerWindow::onManageMedicinesClicked() {
    MedicinesWindow* medicinesWindow = new MedicinesWindow(system, this);
    medicinesWindow->setAttribute(Qt::WA_DeleteOnClose);
    medicinesWindow->show();
}

void ManagerWindow::onViewReportsClicked() {
    ReportsWindow* reportsWindow = new ReportsWindow(system, this);
    reportsWindow->setAttribute(Qt::WA_DeleteOnClose);
    reportsWindow->show();
}

void ManagerWindow::onLogoutClicked() {
    system->logout();
    close();
}

#include "billswindow.h"
#include <QHeaderView>
#include <vector>
#include <utility>
#include <algorithm>

using namespace std;

BillsWindow::BillsWindow(PharmacyManagementSystem* system, QWidget *parent)
    : QMainWindow(parent), system(system) {
    setupUI();
    setWindowTitle("Create Bill");
    resize(1200, 700);

    refreshMedicinesTable();
    refreshBillsList();
}

void BillsWindow::setupUI() {
    QWidget* centralWidget = new QWidget();
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Customer info
    QHBoxLayout* customerLayout = new QHBoxLayout();
    customerLayout->addWidget(new QLabel("Customer Name:"));
    customerNameEdit = new QLineEdit();
    customerNameEdit->setPlaceholderText("Enter customer name");
    customerLayout->addWidget(customerNameEdit);
    customerLayout->addStretch();
    mainLayout->addLayout(customerLayout);

    // Two columns layout
    QHBoxLayout* columnsLayout = new QHBoxLayout();

    // Left column - Available medicines
    QVBoxLayout* leftLayout = new QVBoxLayout();

    QHBoxLayout* searchLayout = new QHBoxLayout();
    searchLayout->addWidget(new QLabel("Search Medicine:"));
    searchMedicineEdit = new QLineEdit();
    searchMedicineEdit->setPlaceholderText("Enter medicine name or ID");
    searchLayout->addWidget(searchMedicineEdit);

    QPushButton* searchButton = new QPushButton("Search");
    searchButton->setStyleSheet("padding: 5px;");
    connect(searchButton, &QPushButton::clicked, this, &BillsWindow::onSearchMedicineClicked);
    searchLayout->addWidget(searchButton);

    leftLayout->addLayout(searchLayout);

    medicinesTable = new QTableWidget();
    medicinesTable->setColumnCount(5);
    medicinesTable->setHorizontalHeaderLabels({"ID", "Name", "Price", "Available", "Quantity"});
    medicinesTable->horizontalHeader()->setStretchLastSection(true);
    medicinesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    medicinesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    leftLayout->addWidget(medicinesTable);

    columnsLayout->addLayout(leftLayout, 2);

    // Middle column - Cart
    QVBoxLayout* middleLayout = new QVBoxLayout();
    middleLayout->addWidget(new QLabel("Shopping Cart"));

    cartTable = new QTableWidget();
    cartTable->setColumnCount(4);
    cartTable->setHorizontalHeaderLabels({"ID", "Name", "Price", "Qty"});
    cartTable->horizontalHeader()->setStretchLastSection(true);
    cartTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    middleLayout->addWidget(cartTable);

    QHBoxLayout* cartButtonsLayout = new QHBoxLayout();
    QPushButton* addButton = new QPushButton("Add Selected");
    addButton->setStyleSheet("padding: 5px; background-color: #4CAF50; color: white;");
    connect(addButton, &QPushButton::clicked, this, &BillsWindow::onAddToCartClicked);

    QPushButton* removeButton = new QPushButton("Remove Selected");
    removeButton->setStyleSheet("padding: 5px; background-color: #f44336; color: white;");
    connect(removeButton, &QPushButton::clicked, this, &BillsWindow::onRemoveFromCartClicked);

    cartButtonsLayout->addWidget(addButton);
    cartButtonsLayout->addWidget(removeButton);
    middleLayout->addLayout(cartButtonsLayout);

    totalLabel = new QLabel("Total: $0.00");
    totalLabel->setStyleSheet("font-size: 16px; font-weight: bold; padding: 10px;");
    middleLayout->addWidget(totalLabel);

    QPushButton* createBillButton = new QPushButton("Create Bill");
    createBillButton->setStyleSheet("padding: 10px; font-size: 14px; background-color: #2196F3; color: white;");
    connect(createBillButton, &QPushButton::clicked, this, &BillsWindow::onCreateBillClicked);
    middleLayout->addWidget(createBillButton);

    columnsLayout->addLayout(middleLayout, 1);

    // Right column - Recent bills
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->addWidget(new QLabel("Recent Bills"));

    billsList = new QListWidget();
    rightLayout->addWidget(billsList);

    QPushButton* refreshBillsButton = new QPushButton("Refresh Bills");
    refreshBillsButton->setStyleSheet("padding: 5px;");
    connect(refreshBillsButton, &QPushButton::clicked, this, &BillsWindow::onRefreshBillsClicked);
    rightLayout->addWidget(refreshBillsButton);

    columnsLayout->addLayout(rightLayout, 1);

    mainLayout->addLayout(columnsLayout);
}

void BillsWindow::refreshMedicinesTable() {
    medicinesTable->setRowCount(0);

    vector<Medicine> medicines = system->getMedicines();

    for (const Medicine& med : medicines) {
        if (med.quantity <= 0 || system->isMedicineExpired(med)) {
            continue;
        }

        int row = medicinesTable->rowCount();
        medicinesTable->insertRow(row);

        medicinesTable->setItem(row, 0, new QTableWidgetItem(QString::number(med.id)));
        medicinesTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(med.name)));
        medicinesTable->setItem(row, 2, new QTableWidgetItem(QString("$%1").arg(med.price, 0, 'f', 2)));
        medicinesTable->setItem(row, 3, new QTableWidgetItem(QString::number(med.quantity)));

        // Add spinbox in quantity column
        QSpinBox* quantitySpin = new QSpinBox();
        quantitySpin->setMinimum(1);
        quantitySpin->setMaximum(med.quantity);
        quantitySpin->setValue(1);
        medicinesTable->setCellWidget(row, 4, quantitySpin);
    }
}

void BillsWindow::refreshCart() {
    cartTable->setRowCount(cart.size());

    double total = 0.0;
    for (int i = 0; i < cart.size(); ++i) {
        const CartItem& item = cart.at(i);

        cartTable->setItem(i, 0, new QTableWidgetItem(QString::number(item.id)));
        cartTable->setItem(i, 1, new QTableWidgetItem(item.name));
        cartTable->setItem(i, 2, new QTableWidgetItem(QString("$%1").arg(item.price, 0, 'f', 2)));
        cartTable->setItem(i, 3, new QTableWidgetItem(QString::number(item.quantity)));

        total += item.price * item.quantity;
    }

    totalLabel->setText(QString("Total: $%1").arg(total, 0, 'f', 2));
}

void BillsWindow::refreshBillsList() {
    billsList->clear();

    vector<Bill> bills = system->getBills();
    for (const Bill& bill : bills) {
        QString itemText = QString("Bill #%1 - %2 - $%3")
        .arg(bill.id)
            .arg(QString::fromStdString(bill.customerName))
            .arg(bill.totalPrice, 0, 'f', 2);
        billsList->addItem(itemText);
    }
}

void BillsWindow::calculateTotal() {
    double total = 0.0;
    for (const CartItem& item : cart) {
        total += item.price * item.quantity;
    }
    totalLabel->setText(QString("Total: $%1").arg(total, 0, 'f', 2));
}

void BillsWindow::onSearchMedicineClicked() {
    QString query = searchMedicineEdit->text();
    if (query.isEmpty()) {
        refreshMedicinesTable();
        return;
    }

    medicinesTable->setRowCount(0);
    vector<Medicine> results = system->searchMedicines(query.toStdString());

    for (const Medicine& med : results) {
        if (med.quantity <= 0 || system->isMedicineExpired(med)) {
            continue;
        }

        int row = medicinesTable->rowCount();
        medicinesTable->insertRow(row);

        medicinesTable->setItem(row, 0, new QTableWidgetItem(QString::number(med.id)));
        medicinesTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(med.name)));
        medicinesTable->setItem(row, 2, new QTableWidgetItem(QString("$%1").arg(med.price, 0, 'f', 2)));
        medicinesTable->setItem(row, 3, new QTableWidgetItem(QString::number(med.quantity)));

        QSpinBox* quantitySpin = new QSpinBox();
        quantitySpin->setMinimum(1);
        quantitySpin->setMaximum(med.quantity);
        quantitySpin->setValue(1);
        medicinesTable->setCellWidget(row, 4, quantitySpin);
    }
}

void BillsWindow::onAddToCartClicked() {
    QList<QTableWidgetItem*> selected = medicinesTable->selectedItems();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a medicine first");
        return;
    }

    int row = selected.first()->row();
    int id = medicinesTable->item(row, 0)->text().toInt();
    QString name = medicinesTable->item(row, 1)->text();
    double price = medicinesTable->item(row, 2)->text().remove('$').toDouble();
    int available = medicinesTable->item(row, 3)->text().toInt();

    QSpinBox* quantitySpin = qobject_cast<QSpinBox*>(medicinesTable->cellWidget(row, 4));
    if (!quantitySpin) {
        QMessageBox::warning(this, "Error", "Failed to get quantity");
        return;
    }

    int quantity = quantitySpin->value();

    // Check if already in cart
    for (int i = 0; i < cart.size(); ++i) {
        if (cart.at(i).id == id) {
            cart[i].quantity += quantity;
            if (cart.at(i).quantity > available) {
                cart[i].quantity = available;
                QMessageBox::information(this, "Notice",
                                         QString("Quantity adjusted to maximum available: %1").arg(available));
            }
            refreshCart();
            return;
        }
    }

    // Add new item to cart
    CartItem item;
    item.id = id;
    item.name = name;
    item.price = price;
    item.quantity = quantity;
    item.available = available;
    cart.append(item);

    refreshCart();
    QMessageBox::information(this, "Success", QString("%1 added to cart").arg(name));
}

void BillsWindow::onRemoveFromCartClicked() {
    QList<QTableWidgetItem*> selected = cartTable->selectedItems();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select an item from cart");
        return;
    }

    int row = selected.first()->row();
    QString itemName = cart.at(row).name;
    cart.removeAt(row);
    refreshCart();
    QMessageBox::information(this, "Success", QString("%1 removed from cart").arg(itemName));
}

void BillsWindow::onCreateBillClicked() {
    if (cart.isEmpty()) {
        QMessageBox::warning(this, "Error", "Cart is empty");
        return;
    }

    QString customerName = customerNameEdit->text().trimmed();
    if (customerName.isEmpty()) {
        customerName = "Walk-in Customer";
    }

    // Convert cart to vector of pairs
    vector<pair<int, int>> medicineQuantities;
    for (const CartItem& item : cart) {
        medicineQuantities.push_back(make_pair(item.id, item.quantity));
    }

    Bill bill = system->createBill(customerName.toStdString(), medicineQuantities);

    if (bill.medicineCount > 0) {
        // Show bill details
        QString billDetails = QString("Bill Created Successfully!\n\n"
                                      "Bill ID: %1\n"
                                      "Customer: %2\n"
                                      "Items: %3\n"
                                      "Total: $%4\n\n"
                                      "Thank you for your purchase!")
                                  .arg(bill.id)
                                  .arg(QString::fromStdString(bill.customerName))
                                  .arg(bill.medicineCount)
                                  .arg(bill.totalPrice, 0, 'f', 2);

        QMessageBox::information(this, "Bill Created", billDetails);

        // Clear cart and refresh
        cart.clear();
        refreshCart();
        refreshMedicinesTable();
        refreshBillsList();
        customerNameEdit->clear();
    } else {
        QMessageBox::critical(this, "Error", "Failed to create bill. Please check stock availability.");
    }
}

void BillsWindow::onRefreshBillsClicked() {
    refreshBillsList();
    QMessageBox::information(this, "Refreshed", "Bills list has been updated");
}

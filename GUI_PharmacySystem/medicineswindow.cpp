#include "medicineswindow.h"
#include <QStatusBar>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <vector>
#include <string>

using namespace std;

MedicinesWindow::MedicinesWindow(PharmacyManagementSystem* system, QWidget *parent)
    : QMainWindow(parent), system(system), pharmacistMode(false) {
    setupUI();
    setWindowTitle(pharmacistMode ? "Search Medicines" : "Manage Medicines");
    resize(1000, 700);
    refreshTable();
}

void MedicinesWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Search section
    QHBoxLayout* searchLayout = new QHBoxLayout();
    searchLayout->addWidget(new QLabel("Search:"));
    searchEdit = new QLineEdit();
    searchLayout->addWidget(searchEdit);
    QPushButton* searchBtn = new QPushButton("Search");
    searchLayout->addWidget(searchBtn);
    QPushButton* refreshBtn = new QPushButton("Refresh");
    searchLayout->addWidget(refreshBtn);
    mainLayout->addLayout(searchLayout);

    // Table
    medicinesTable = new QTableWidget();
    medicinesTable->setColumnCount(7);
    medicinesTable->setHorizontalHeaderLabels({"ID", "Name", "Category", "Quantity", "Price", "Expiry Date", "Status"});
    medicinesTable->horizontalHeader()->setStretchLastSection(true);
    medicinesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    medicinesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    medicinesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(medicinesTable);

    // Form section (only for admin mode)
    if (!pharmacistMode) {
        QGroupBox* formGroup = new QGroupBox("Medicine Details");
        QFormLayout* formLayout = new QFormLayout();

        nameEdit = new QLineEdit();
        formLayout->addRow("Name:", nameEdit);

        categoryEdit = new QLineEdit();
        formLayout->addRow("Category:", categoryEdit);

        quantityEdit = new QSpinBox();
        quantityEdit->setRange(0, 10000);
        formLayout->addRow("Quantity:", quantityEdit);

        priceEdit = new QDoubleSpinBox();
        priceEdit->setRange(0.0, 10000.0);
        priceEdit->setDecimals(2);
        priceEdit->setPrefix("$");
        formLayout->addRow("Price:", priceEdit);

        QHBoxLayout* expiryLayout = new QHBoxLayout();
        expiryMonthEdit = new QSpinBox();
        expiryMonthEdit->setRange(1, 12);
        expiryLayout->addWidget(new QLabel("Month:"));
        expiryLayout->addWidget(expiryMonthEdit);

        expiryYearEdit = new QSpinBox();
        expiryYearEdit->setRange(2024, 2050);
        expiryLayout->addWidget(new QLabel("Year:"));
        expiryLayout->addWidget(expiryYearEdit);
        formLayout->addRow("Expiry Date:", expiryLayout);

        stockDeltaEdit = new QSpinBox();
        stockDeltaEdit->setRange(-1000, 1000);
        formLayout->addRow("Stock Change:", stockDeltaEdit);

        QHBoxLayout* buttonLayout = new QHBoxLayout();
        QPushButton* addBtn = new QPushButton("Add Medicine");
        buttonLayout->addWidget(addBtn);
        QPushButton* updateBtn = new QPushButton("Update Stock");
        buttonLayout->addWidget(updateBtn);
        QPushButton* clearBtn = new QPushButton("Clear");
        buttonLayout->addWidget(clearBtn);
        formLayout->addRow(buttonLayout);

        formGroup->setLayout(formLayout);
        mainLayout->addWidget(formGroup);

        // Connect form buttons
        connect(addBtn, &QPushButton::clicked, this, &MedicinesWindow::onAddMedicineClicked);
        connect(updateBtn, &QPushButton::clicked, this, &MedicinesWindow::onUpdateStockClicked);
        connect(clearBtn, &QPushButton::clicked, this, &MedicinesWindow::clearForm);
    }

    // Connect signals
    connect(searchBtn, &QPushButton::clicked, this, &MedicinesWindow::onSearchClicked);
    connect(refreshBtn, &QPushButton::clicked, this, &MedicinesWindow::onRefreshClicked);
    connect(medicinesTable, &QTableWidget::itemSelectionChanged, this, &MedicinesWindow::onTableSelectionChanged);
}

void MedicinesWindow::refreshTable() {
    medicinesTable->setRowCount(0);
    vector<Medicine> medicinesList = system->getMedicines();

    for (const Medicine& med : medicinesList) {
        int row = medicinesTable->rowCount();
        medicinesTable->insertRow(row);

        medicinesTable->setItem(row, 0, new QTableWidgetItem(QString::number(med.id)));
        medicinesTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(med.name)));
        medicinesTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(med.category)));
        medicinesTable->setItem(row, 3, new QTableWidgetItem(QString::number(med.quantity)));
        medicinesTable->setItem(row, 4, new QTableWidgetItem(QString("$%1").arg(med.price, 0, 'f', 2)));
        medicinesTable->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(med.expiryDate.toString())));

        // Status column
        QString status;
        QColor color;
        if (system->isMedicineExpired(med)) {
            status = "Expired";
            color = Qt::red;
        } else if (med.quantity <= 10) {
            status = "Low Stock";
            color = QColor(255, 165, 0);
        } else {
            status = "Available";
            color = Qt::darkGreen;
        }

        QTableWidgetItem* statusItem = new QTableWidgetItem(status);
        statusItem->setForeground(color);
        medicinesTable->setItem(row, 6, statusItem);
    }
}

void MedicinesWindow::onSearchClicked() {
    QString query = searchEdit->text();
    if (query.isEmpty()) {
        refreshTable();
        return;
    }

    vector<Medicine> results = system->searchMedicines(query.toStdString());
    medicinesTable->setRowCount(0);

    for (const Medicine& med : results) {
        int row = medicinesTable->rowCount();
        medicinesTable->insertRow(row);

        medicinesTable->setItem(row, 0, new QTableWidgetItem(QString::number(med.id)));
        medicinesTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(med.name)));
        medicinesTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(med.category)));
        medicinesTable->setItem(row, 3, new QTableWidgetItem(QString::number(med.quantity)));
        medicinesTable->setItem(row, 4, new QTableWidgetItem(QString("$%1").arg(med.price, 0, 'f', 2)));
        medicinesTable->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(med.expiryDate.toString())));

        QString status = med.quantity > 10 ? "Available" : "Low Stock";
        medicinesTable->setItem(row, 6, new QTableWidgetItem(status));
    }

    statusBar()->showMessage(QString("Found %1 medicine(s)").arg(results.size()), 3000);
}

void MedicinesWindow::onAddMedicineClicked() {
    if (pharmacistMode) return;

    QString name = nameEdit->text().trimmed();
    QString category = categoryEdit->text().trimmed();

    if (name.isEmpty() || category.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill in all required fields.");
        return;
    }

    // Call addMedicine with all 6 parameters
    bool success = system->addMedicine(
        name.toStdString(),
        category.toStdString(),
        quantityEdit->value(),
        priceEdit->value(),
        expiryMonthEdit->value(),
        expiryYearEdit->value()
        );

    if (success) {
        QMessageBox::information(this, "Success", "Medicine added successfully!");
        refreshTable();
        clearForm();
    } else {
        QMessageBox::critical(this, "Error", "Failed to add medicine.");
    }
}

void MedicinesWindow::onUpdateStockClicked() {
    if (pharmacistMode) return;

    QList<QTableWidgetItem*> selected = medicinesTable->selectedItems();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Selection Error", "Please select a medicine from the table.");
        return;
    }

    int medId = medicinesTable->item(selected[0]->row(), 0)->text().toInt();
    int delta = stockDeltaEdit->value();

    if (delta == 0) {
        QMessageBox::warning(this, "Input Error", "Stock change cannot be zero.");
        return;
    }

    // Use updateStock instead of updateMedicineStock
    bool success = system->updateStock(medId, delta);

    if (success) {
        QMessageBox::information(this, "Success", "Stock updated successfully!");
        refreshTable();
        stockDeltaEdit->setValue(0);
    } else {
        QMessageBox::critical(this, "Error", "Failed to update stock. Check if medicine exists and quantity is valid.");
    }
}

void MedicinesWindow::onRefreshClicked() {
    searchEdit->clear();
    refreshTable();
    statusBar()->showMessage("Table refreshed", 2000);
}

void MedicinesWindow::onTableSelectionChanged() {
    if (pharmacistMode) return;

    QList<QTableWidgetItem*> selected = medicinesTable->selectedItems();
    if (!selected.isEmpty()) {
        int row = selected[0]->row();
        int medId = medicinesTable->item(row, 0)->text().toInt();

        vector<Medicine> medicines = system->getMedicines();
        for (const Medicine& med : medicines) {
            if (med.id == medId) {
                populateForm(med);
                break;
            }
        }
    }
}

void MedicinesWindow::populateForm(const Medicine& medicine) {
    if (pharmacistMode) return;

    nameEdit->setText(QString::fromStdString(medicine.name));
    categoryEdit->setText(QString::fromStdString(medicine.category));
    quantityEdit->setValue(medicine.quantity);
    priceEdit->setValue(medicine.price);
    expiryMonthEdit->setValue(medicine.expiryDate.month);
    expiryYearEdit->setValue(medicine.expiryDate.year);
}

void MedicinesWindow::clearForm() {
    if (pharmacistMode) return;

    nameEdit->clear();
    categoryEdit->clear();
    quantityEdit->setValue(0);
    priceEdit->setValue(0.0);
    expiryMonthEdit->setValue(1);
    expiryYearEdit->setValue(2024);
    stockDeltaEdit->setValue(0);
    medicinesTable->clearSelection();
}

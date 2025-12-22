#ifndef BILLSWINDOW_H
#define BILLSWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QMessageBox>
#include <QListWidget>
#include <QVector>
#include <utility>
#include <vector>
#include <string>
#include "pharmacySystem.h"

class BillsWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit BillsWindow(PharmacyManagementSystem* system, QWidget *parent = nullptr);

private slots:
    void onSearchMedicineClicked();
    void onAddToCartClicked();
    void onRemoveFromCartClicked();
    void onCreateBillClicked();
    void onRefreshBillsClicked();

private:
    PharmacyManagementSystem* system;

    QLineEdit* customerNameEdit;
    QLineEdit* searchMedicineEdit;
    QTableWidget* medicinesTable;
    QTableWidget* cartTable;
    QListWidget* billsList;
    QLabel* totalLabel;

    struct CartItem {
        int id;
        QString name;
        double price;
        int quantity;
        int available;
    };

    QVector<CartItem> cart;

    void setupUI();
    void refreshMedicinesTable();
    void refreshCart();
    void refreshBillsList();
    void calculateTotal();
};

#endif // BILLSWINDOW_H

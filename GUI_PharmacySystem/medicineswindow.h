#ifndef MEDICINESWINDOW_H
#define MEDICINESWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QMessageBox>
#include <QHeaderView>
#include "pharmacySystem.h"

class MedicinesWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MedicinesWindow(PharmacyManagementSystem* system, QWidget *parent = nullptr);
    void setPharmacistMode(bool mode) { pharmacistMode = mode; }

private slots:
    void onSearchClicked();
    void onAddMedicineClicked();
    void onUpdateStockClicked();
    void onRefreshClicked();
    void onTableSelectionChanged();

private:
    PharmacyManagementSystem* system;
    bool pharmacistMode;

    QTableWidget* medicinesTable;
    QLineEdit* searchEdit;
    QLineEdit* nameEdit;
    QLineEdit* categoryEdit;
    QSpinBox* quantityEdit;
    QDoubleSpinBox* priceEdit;
    QSpinBox* expiryMonthEdit;
    QSpinBox* expiryYearEdit;
    QSpinBox* stockDeltaEdit;

    void setupUI();
    void refreshTable();
    void populateForm(const Medicine& medicine);
    void clearForm();
};

#endif // MEDICINESWINDOW_H

#ifndef REPORTSWINDOW_H
#define REPORTSWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QLayout>
#include "pharmacySystem.h"

class ReportsWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit ReportsWindow(PharmacyManagementSystem* system, QWidget *parent = nullptr);

private slots:
    void onRefreshLowStockClicked();
    void onRefreshSalesClicked();
    void onRefreshExpiredClicked();

private:
    PharmacyManagementSystem* system;

    QTabWidget* tabWidget;
    QTableWidget* lowStockTable;
    QTableWidget* salesTable;
    QTableWidget* expiredTable;
    QSpinBox* thresholdSpin;
    QLabel* totalSalesLabel;

    void setupUI();
    void refreshLowStockReport();
    void refreshSalesReport();
    void refreshExpiredReport();
};

#endif // REPORTSWINDOW_H

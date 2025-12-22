#include "reportswindow.h"
#include <QHeaderView>
#include <vector>
#include <string>
#include <utility>

using namespace std;

void ReportsWindow::refreshLowStockReport() {
    lowStockTable->setRowCount(0);

    int threshold = thresholdSpin->value();
    vector<Medicine> lowStock = system->getLowStockMedicines(threshold);

    for (const Medicine& med : lowStock) {
        int row = lowStockTable->rowCount();
        lowStockTable->insertRow(row);

        lowStockTable->setItem(row, 0, new QTableWidgetItem(QString::number(med.id)));
        lowStockTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(med.name)));
        lowStockTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(med.category)));
        lowStockTable->setItem(row, 3, new QTableWidgetItem(QString::number(med.quantity)));
        lowStockTable->setItem(row, 4, new QTableWidgetItem(QString("$%1").arg(med.price, 0, 'f', 2)));

        QString status;
        QColor color;
        if (med.quantity == 0) {
            status = "Out of Stock";
            color = Qt::red;
        } else if (med.quantity <= 5) {
            status = "Critical";
            color = QColor(255, 0, 0);
        } else {
            status = "Low";
            color = QColor(255, 165, 0);
        }

        QTableWidgetItem* statusItem = new QTableWidgetItem(status);
        statusItem->setForeground(color);
        lowStockTable->setItem(row, 5, statusItem);
    }
}

void ReportsWindow::refreshSalesReport() {
    salesTable->setRowCount(0);

    double totalSales = system->calculateTotalSales();
    totalSalesLabel->setText(QString("Total Sales: $%1").arg(totalSales, 0, 'f', 2));

    vector<Bill> billsList = system->getBills();

    for (const Bill& bill : billsList) {
        int row = salesTable->rowCount();
        salesTable->insertRow(row);

        salesTable->setItem(row, 0, new QTableWidgetItem(QString::number(bill.id)));
        salesTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(bill.customerName)));
        salesTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(bill.pharmacistName)));
        salesTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(bill.date.toString())));
        salesTable->setItem(row, 4, new QTableWidgetItem(QString("$%1").arg(bill.totalPrice, 0, 'f', 2)));
    }
}

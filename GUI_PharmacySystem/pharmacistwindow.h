#ifndef PHARMACISTWINDOW_H
#define PHARMACISTWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include "pharmacySystem.h"

class PharmacistWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit PharmacistWindow(PharmacyManagementSystem* system, QWidget *parent = nullptr);

private slots:
    void onSearchMedicinesClicked();
    void onCreateBillClicked();
    void onLogoutClicked();

private:
    PharmacyManagementSystem* system;

    void setupUI();
};

#endif // PHARMACISTWINDOW_H

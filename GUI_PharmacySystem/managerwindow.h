#ifndef MANAGERWINDOW_H
#define MANAGERWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include "pharmacySystem.h"

class ManagerWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit ManagerWindow(PharmacyManagementSystem* system, QWidget *parent = nullptr);

private slots:
    void onManageMedicinesClicked();
    void onViewReportsClicked();
    void onLogoutClicked();

private:
    PharmacyManagementSystem* system;

    void setupUI();
};

#endif // MANAGERWINDOW_H

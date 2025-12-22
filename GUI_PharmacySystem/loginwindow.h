#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QLayout>
#include "pharmacySystem.h"

class LoginWindow : public QWidget {
    Q_OBJECT

public:
    explicit LoginWindow(PharmacyManagementSystem* system, QWidget *parent = nullptr);

signals:
    void loginSuccessful();
    void registerRequested();

private slots:
    void onLoginClicked();
    void onRegisterClicked();

private:
    PharmacyManagementSystem* system;

    QLineEdit* usernameEdit;
    QLineEdit* passwordEdit;
    QPushButton* loginButton;
    QPushButton* registerButton;

    void setupUI();
};

#endif // LOGINWINDOW_H

#include "loginwindow.h"

LoginWindow::LoginWindow(PharmacyManagementSystem* system, QWidget *parent)
    : QWidget(parent), system(system) {
    setupUI();
    setWindowTitle("Pharmacy System - Login");
    resize(400, 300);
}

void LoginWindow::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Title
    QLabel* titleLabel = new QLabel("Pharmacy Management System");
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; padding: 20px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Form
    QFormLayout* formLayout = new QFormLayout();

    usernameEdit = new QLineEdit();
    usernameEdit->setPlaceholderText("Enter username");
    formLayout->addRow("Username:", usernameEdit);

    passwordEdit = new QLineEdit();
    passwordEdit->setPlaceholderText("Enter password");
    passwordEdit->setEchoMode(QLineEdit::Password);
    formLayout->addRow("Password:", passwordEdit);

    mainLayout->addLayout(formLayout);
    mainLayout->addSpacing(20);

    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    loginButton = new QPushButton("Login");
    loginButton->setStyleSheet("padding: 10px; font-size: 14px;");
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);

    registerButton = new QPushButton("Register");
    registerButton->setStyleSheet("padding: 10px; font-size: 14px;");
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::onRegisterClicked);

    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(registerButton);

    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();
}

void LoginWindow::onLoginClicked() {
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter both username and password");
        return;
    }

    if (system->login(username.toStdString(), password.toStdString())) {
        emit loginSuccessful();
    } else {
        QMessageBox::critical(this, "Error", "Invalid username or password");
    }
}

void LoginWindow::onRegisterClicked() {
    emit registerRequested();
}

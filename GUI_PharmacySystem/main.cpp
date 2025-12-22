#include <QApplication>
#include <QFile>
#include <QStyleFactory>
#include "pharmacySystem.h"
#include "loginwindow.h"
#include "managerwindow.h"
#include "pharmacistwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Set application style
    app.setStyle(QStyleFactory::create("Fusion"));

    // Set application info
    app.setApplicationName("Pharmacy Management System");
    app.setOrganizationName("Pharmacy Corp");

    // Load stylesheet
    QFile styleFile(":/styles.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        app.setStyleSheet(styleSheet);
    }

    // Create pharmacy system
    PharmacyManagementSystem* system = new PharmacyManagementSystem();

    // Create login window
    LoginWindow* loginWindow = new LoginWindow(system);

    // Connect login signals
    QObject::connect(loginWindow, &LoginWindow::loginSuccessful, [system, loginWindow]() {
        loginWindow->hide();

        QString role = QString::fromStdString(system->getCurrentUserRole());

        if (role == "Manager") {
            ManagerWindow* managerWindow = new ManagerWindow(system);
            managerWindow->show();

            QObject::connect(managerWindow, &ManagerWindow::destroyed, [loginWindow]() {
                loginWindow->show();
            });
        } else if (role == "Pharmacist") {
            PharmacistWindow* pharmacistWindow = new PharmacistWindow(system);
            pharmacistWindow->show();

            QObject::connect(pharmacistWindow, &PharmacistWindow::destroyed, [loginWindow]() {
                loginWindow->show();
            });
        }
    });

    loginWindow->show();

    return app.exec();
}

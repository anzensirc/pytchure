#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    setWindowTitle("Login Page");

    connect(ui->loginButton, &QPushButton::clicked, this, &LoginWindow::handleLogin);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::handleLogin()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    if (username == "admin" && password == "1234") {
        QMessageBox::information(this, "Login Berhasil", "Selamat datang, admin!");
    } else {
        QMessageBox::warning(this, "Login Gagal", "Username atau password salah!");
    }
}

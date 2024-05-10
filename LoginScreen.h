#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_LoginScreen.h"
#include "AdminPage.h"
#include "EmployeePage.h"



class LoginScreen : public QMainWindow
{
    Q_OBJECT

public:
    LoginScreen(QWidget *parent = nullptr);
    ~LoginScreen();

public slots:
    void on_pushButton_Login_clicked();
    void on_pushButton_Quit_clicked();
    void connect_to_QTimer();
private:
    AdminPage* adminpage;
    EmployeePage* employee_page;
    QTimer *timer;
    Ui::LoginScreenClass ui;
    QSqlDatabase loginDatabase;
};

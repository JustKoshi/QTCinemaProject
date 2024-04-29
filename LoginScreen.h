#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_LoginScreen.h"
#include <qmessagebox.h>
#include "AdminPage.h"
#include "EmployeePage.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>

class LoginScreen : public QMainWindow
{
    Q_OBJECT

public:
    LoginScreen(QWidget *parent = nullptr);
    ~LoginScreen();

public slots:
    void on_pushButton_Login_clicked();
    void on_pushButton_Quit_clicked();
	
private:
    Ui::LoginScreenClass ui;
    QSqlDatabase loginDatabase;
};

#pragma once

#include <QMainWindow>
#include "ui_EmployeePage.h"
#include <QTimer>
#include <QDateTime>
#include <qmessagebox.h>
#include "CheckOutScreen.h"
class EmployeePage : public QMainWindow
{
	Q_OBJECT

public:
	EmployeePage(QWidget *parent = nullptr);
	~EmployeePage();
	void setName(int name);
	void setLoginDatabase(QSqlDatabase db);
public slots:
	void on_pushButton_checkOut_clicked();
	void showEmployeePage();
	void connect_to_QTimer();
private:
	Ui::EmployeePageClass ui;
	CheckOutScreen* checkoutscreen;
	QTimer* timer;
	int id;
	QString label;
	QSqlDatabase loginDatabase;
};

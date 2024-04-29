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
public slots:
	void on_pushButton_checkOut_clicked();
	void showEmployeePage();
	void connect_to_QTimer();
private:
	Ui::EmployeePageClass ui;
	CheckOutScreen* checkoutscreen;
	QTimer* timer;
};

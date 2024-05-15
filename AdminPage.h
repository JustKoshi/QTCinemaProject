#pragma once

#include <QMainWindow>
#include "ui_AdminPage.h"
#include <QTimer>
#include <QDateTime>
#include <qmessagebox.h>
#include "CheckOutScreen.h"

class AdminPage : public QMainWindow
{
	Q_OBJECT

public:
	AdminPage(QWidget *parent = nullptr);
	~AdminPage();
	void setName(int name);
	void setLoginDatabase(QSqlDatabase db);
public slots:
	void on_pushButton_checkOut_clicked();
	void showEmployeePage();
	void connect_to_QTimer();

private:
	Ui::AdminPageClass ui;
	CheckOutScreen* checkoutscreen;
	QTimer* timer;
	int id;
	QString label;
	QSqlDatabase loginDatabase;
};
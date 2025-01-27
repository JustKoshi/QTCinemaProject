#pragma once

#include <QMainWindow>
#include "ui_EmployeePage.h"
#include "CheckOutScreen.h"
#include "ScreeningSelection.h"
#include "CancelReservation.h"
#include "AccountSetting.h"
#include "ReportPage.h"

class EmployeePage : public QMainWindow
{
	Q_OBJECT

public:
	EmployeePage(QWidget *parent = nullptr);
	~EmployeePage();
	void setName(int name);
	void setLoginDatabase(QSqlDatabase db);
	void setTodaysScreenings();
public slots:
	void on_pushButton_checkOut_clicked();
	void showEmployeePage();
	void connect_to_QTimer();
	void on_pushButton_ticket_sale_clicked();
	void on_pushButton_snack_sale_clicked();
	void on_pushButton_cancel_reservation_clicked();
	void on_pushButton_logout_clicked();
	void on_pushButton_account_settings_clicked();
	void on_pushButton_reports_clicked();

signals:
	void return_To_loginPage();
private:
	Ui::EmployeePageClass ui;
	CheckOutScreen* checkoutscreen;
	QTimer* timer;
	int id;
	QString label;
	QSqlDatabase loginDatabase;
	ScreeningSelection* screeningselection;
};

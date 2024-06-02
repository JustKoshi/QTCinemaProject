#pragma once

#include <QMainWindow>
#include "CheckOutScreen.h"
#include "MovieManager.h"
#include "ScreeningManager.h"
#include "CancelReservation.h"
#include "AccountSetting.h"
#include "ReportPage.h"
#include "EmployeeManager.h"
#include "ui_AdminPage.h"

class AdminPage : public QMainWindow
{
	Q_OBJECT

public:
	AdminPage(QWidget *parent = nullptr);
	~AdminPage();
	void setName(int name);
	void setLoginDatabase(QSqlDatabase db);
	void setTodaysScreenings();
public slots:

	void on_pushButton_checkOut_clicked();
	void on_pushButton_movieManager_clicked();
	void on_pushButton_screeningManager_clicked();
	void on_pushButton_cancel_reservation_clicked();
	void showEmployeePage();
	void connect_to_QTimer();
	void on_pushButton_logout_clicked();
	void on_pushButton_account_settings_clicked();
	void on_pushButton_reports_clicked();
	void on_pushButton_employee_manager_clicked();

signals:
	void return_To_loginPage();
private:
	Ui::AdminPageClass ui;
	CheckOutScreen* checkoutscreen;
	MovieManager* moviemanager;
	ScreeningManager* screeningmanager;
	QTimer* timer;
	int id;
	QString label;
	QSqlDatabase loginDatabase;
	
	
};

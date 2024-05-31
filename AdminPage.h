#pragma once

#include <QMainWindow>
#include "ui_AdminPage.h"
#include "CheckOutScreen.h"
#include "MovieManager.h"
#include "ScreeningManager.h"
#include "CancelReservation.h"

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
	void on_pushButton_movieManager_clicked();
	void on_pushButton_screeningManager_clicked();
	void on_pushButton_cancel_reservation_clicked();
	void showEmployeePage();
	void connect_to_QTimer();

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

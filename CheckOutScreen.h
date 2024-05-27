#pragma once

#include <QMainWindow>
#include "ui_CheckOutScreen.h"
#include <QTimer>
#include <QDateTime>
#include <QMessageBox>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>

class CheckOutScreen : public QMainWindow
{
	Q_OBJECT

public:
	CheckOutScreen(QWidget *parent = nullptr);
	~CheckOutScreen();
	void setName(QString name);
	void setLoginDatabase(QSqlDatabase db);
	void set_elapsed_shift_time(int time);
signals:
	void return_To_EmployeePage();

public slots:

	void on_pushButton_return_clicked();
	void connect_to_QCurrent_Time();
	void update_shift_time();
	void update_break_time();
	void on_pushButton_break_start_clicked();
	void on_pushButton_break_end_clicked();
	void on_pushButton_shift_start_clicked();
	void on_pushButton_shift_end_clicked();

private:
	Ui::CheckOutScreenClass ui;
	QTimer* timer;
	QTimer* current_time;
	QTimer* shift_timer;
	QString name;
	QTime elapsed_break_time;
	QTime elapsed_shift_time;
	QSqlDatabase loginDb;
	bool timer_started;
	bool shift_timer_started;

};

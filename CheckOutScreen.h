#pragma once

#include <QMainWindow>
#include "ui_CheckOutScreen.h"
#include <QTimer>
#include <QDateTime>
#include <QMessageBox>

class CheckOutScreen : public QMainWindow
{
	Q_OBJECT

public:
	CheckOutScreen(QWidget *parent = nullptr);
	~CheckOutScreen();
	void setName(QString name);
signals:
	void return_To_EmployeePage();

public slots:

	void on_pushButton_return_clicked();
	void connect_to_QCurrent_Time();
	void update_break_time();
	void on_pushButton_break_start_clicked();
	void on_pushButton_break_end_clicked();

private:
	Ui::CheckOutScreenClass ui;
	QTimer* timer;
	QTimer* current_time;
	QString name;
	QTime elapsed_break_time;
	QTime elapsed_shift_time;
	bool timer_started;

};

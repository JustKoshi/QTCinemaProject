#pragma once

#include <QMainWindow>
#include "ui_CheckOutScreen.h"
#include <QTimer>
#include <QDateTime>

class CheckOutScreen : public QMainWindow
{
	Q_OBJECT

public:
	CheckOutScreen(QWidget *parent = nullptr);
	~CheckOutScreen();
signals:
	void return_To_EmployeePage();

public slots:

	void on_PushButton_return_clicked();
	void connect_to_QCurrent_Time();
private:
	Ui::CheckOutScreenClass ui;
	QTimer* timer;
	QTimer* current_time;
};

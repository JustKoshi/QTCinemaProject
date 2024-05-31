#pragma once

#include <QMainWindow>
#include "ui_CancelReservation.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QDialog>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>


class CancelReservation : public QMainWindow
{
	Q_OBJECT

public:
	CancelReservation(QWidget *parent = nullptr, QSqlDatabase db = QSqlDatabase(), int id=0);
	~CancelReservation();
public slots:
	void on_pushButton_return_clicked();
	void on_pushButton_cancel_clicked();
signals:
	void return_To_AdminPage();
	void return_to_employee_page();

private:
	Ui::CancelReservationClass ui;
	QSqlDatabase Database;
	int id;
	void setTableView();
};

#pragma once

#include <QWidget>
#include "ui_EmployeeManager.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QMessageBox>

class EmployeeManager : public QWidget
{
	Q_OBJECT

public:
	EmployeeManager(QWidget *parent = nullptr, QSqlDatabase db = QSqlDatabase());
	~EmployeeManager();
public slots:
	void on_pushButton_return_clicked();
	void on_pushButton_add_employee_clicked();
	void on_pushButton_delete_employee_clicked();
signals:
	void return_to_admin_page();

private:
	Ui::EmployeeManagerClass ui;
	QSqlDatabase logindb;
	void set_table_view();
};

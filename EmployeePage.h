#pragma once

#include <QMainWindow>
#include "ui_EmployeePage.h"

class EmployeePage : public QMainWindow
{
	Q_OBJECT

public:
	EmployeePage(QWidget *parent = nullptr);
	~EmployeePage();

private:
	Ui::EmployeePageClass ui;
};

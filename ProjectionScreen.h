#pragma once

#include <QWidget>
#include "ui_ProjectionScreen.h"
#include "SnackSales.h"

class ProjectionScreen : public QWidget
{
	Q_OBJECT

public:
	ProjectionScreen(QWidget *parent = nullptr, QString new_id = "0", QSqlDatabase db = QSqlDatabase());
	~ProjectionScreen();
public slots:
	void on_pushButton_proceed_clicked();
signals:
	void return_to_employee_page();
private:
	Ui::ProjectionScreenClass ui;
	QString id;
	QSqlDatabase loginDb;
	std::vector<QString> seats_ids;
	QString hall_id;
	SnackSales* snacksalespage;
	double total_price;
private slots:
	void handle_button_click();
};

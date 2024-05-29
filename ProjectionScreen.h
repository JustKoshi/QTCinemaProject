#pragma once

#include <QWidget>
#include "ui_ProjectionScreen.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlDatabase>
#include <vector>

class ProjectionScreen : public QWidget
{
	Q_OBJECT

public:
	ProjectionScreen(QWidget *parent = nullptr, QString new_id = "0", QSqlDatabase db = QSqlDatabase());
	~ProjectionScreen();
public slots:
	void on_pushButton_proceed_clicked();

private:
	Ui::ProjectionScreenClass ui;
	QString id;
	QSqlDatabase loginDb;
	std::vector<QString> seats_ids;
	QString hall_id;
private slots:
	void handle_button_click();
};

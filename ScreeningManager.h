#pragma once

#include <QWidget>
#include "ui_ScreeningManager.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QFileDialog>
#include <QCalendarWidget>
#include <QScreen>


class ScreeningManager : public QWidget
{
	Q_OBJECT

public:
	ScreeningManager(QWidget *parent = nullptr);
	void getDb(QSqlDatabase db);
	~ScreeningManager();

public slots:
	void on_pushButton_return_clicked();
	void onButtonClicked();
	void on_pushButton_add_screening_clicked();
	void on_pushButton_delete_screening_clicked();
	void on_pushButton_calendar_clicked();
signals:
		void return_To_AdminPage();
private:
	Ui::ScreeningManagerClass ui;
	QSqlDatabase loginDb;
	QString time;
};

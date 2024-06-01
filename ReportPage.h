#pragma once

#include <QMainWindow>
#include "ui_ReportPage.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QtCharts\QChartView>
#include <QtCharts\QPieSeries>
#include <QtCharts\QPieSlice>
#include <QtCharts\QBarSet>
#include <QtCharts\QBarSeries>
#include <QtCharts\QBarCategoryAxis>
#include <QtCharts\QLegendMarker>



class ReportPage : public QMainWindow
{
	Q_OBJECT

public:
	ReportPage(QWidget *parent = nullptr, QSqlDatabase db = QSqlDatabase());
	~ReportPage();
public slots:
	void on_pushButton_return_clicked();
	void on_pushButton_seat_sales_clicked();
	void on_pushButton_snack_sales_clicked();
signals:
	void return_To_AdminPage();
private:
	Ui::ReportPageClass ui;
	QSqlDatabase db;
};

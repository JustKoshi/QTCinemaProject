#pragma once

#include <QMainWindow>
#include "ui_SnackSales.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlDatabase>
#include <vector>
#include <QTextDocument>
#include <QPrinter>
#include <QPrintDialog>
#include <QFileDialog>
#include <QTextStream>

class SnackSales : public QMainWindow
{
	Q_OBJECT

public:
	SnackSales(QWidget *parent = nullptr, QSqlDatabase db = QSqlDatabase());
	~SnackSales();
	void setSeats(std::vector<QString> seats);
	void addItemsToWidgetList(const QStringList& items);
	void setTotalPrice(double price);
	void setScreeningId(QString id);
public slots:
	void on_pushButton_add_popcorn_clicked();
	void on_pushButton_add_soda_clicked();
	void on_pushButton_add_nachos_clicked();
	void on_pushButton_add_candy_clicked();
	void on_pushButton_add_slushy_clicked();
	void on_pushButton_remove_clicked();
	void on_pushButton_check_out_clicked();

 signals:
	void return_to_employee_page();
	
private:
	Ui::SnackSalesClass ui;
	std::vector<QString>seats_ids;
	QSqlDatabase loginDb;
	QString screening_id;
	double total_price;
	std::vector<std::pair<QString, int>> snack_ids_and_amouts;
	int sale_id;
	//void generateReceipt();
	void confirm_sale();
	void update_total_price();
};

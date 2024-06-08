#include "CancelReservation.h"

/**
 * @file CancelReservation.cpp
 * * @brief This file contains the implementation of the functions to cancel a reservation. 
 */


CancelReservation::CancelReservation(QWidget *parent, QSqlDatabase db,int id)
	: QMainWindow(parent) , Database(db), id(id)
{
	ui.setupUi(this);
	setTableView();

}

CancelReservation::~CancelReservation()
{}

/**
 * @brief This function returns to the employee menu page and closes this page.
 */

void CancelReservation::on_pushButton_return_clicked() {
	emit return_To_AdminPage();
	emit return_to_employee_page();
	this->close();
}


/**
 * @brief This function cancels the reservation selected in the table view and if employee is not a manager, asks for manager confirmation. Updates the table view after the operation. Updates database.

 */

void CancelReservation::on_pushButton_cancel_clicked() {
	
	QSqlQuery query(Database);
	query.prepare("SELECT access_level FROM LoginData WHERE employee_id = :employee_id");
	query.bindValue(":employee_id", id);
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
		qDebug() << "1";
	}
	if (query.next()) {
		if (query.value(0).toInt() != 2) {
			QMessageBox::information(this, "Confirmation Required", "Need manager confirmation for this operation.");

			QDialog dialog(this);
			QVBoxLayout layout(&dialog);
			dialog.setMinimumSize(200, 100);
			QLineEdit loginEdit(&dialog);
			loginEdit.setPlaceholderText("Enter manager login");
			layout.addWidget(&loginEdit);

			QLineEdit passwordEdit(&dialog);
			passwordEdit.setPlaceholderText("Enter manager password");
			passwordEdit.setEchoMode(QLineEdit::Password);
			layout.addWidget(&passwordEdit);

			QPushButton confirmButton("Confirm", &dialog);
			layout.addWidget(&confirmButton);

			QObject::connect(&confirmButton, &QPushButton::clicked, [&]() {
				QSqlQuery confirmQuery(Database);
				confirmQuery.prepare("SELECT COUNT(*) FROM LoginData WHERE login = :login AND password = :password AND access_level = 2");
				confirmQuery.bindValue(":login", loginEdit.text());
				confirmQuery.bindValue(":password", passwordEdit.text());
				if (!confirmQuery.exec()) {
					QMessageBox::critical(this, "Error", "Query error");
					qDebug() << "2";
				}
				if (confirmQuery.next() && confirmQuery.value(0).toInt() > 0) {
					QMessageBox::information(this, "Success", "Confirmation successful.");
					dialog.accept();
				}
				else {
					QMessageBox::critical(this, "Error", "Invalid login or password.");
				}
				});

			dialog.exec();
		

		}
	}
	

	QModelIndexList selectedList = ui.tableView->selectionModel()->selectedRows();
	if (selectedList.size() == 0) {
		QMessageBox::information(this, "Error", "No reservation selected.");
		return;
	}
	QModelIndex index = selectedList.at(0);
	QString sale_id = index.sibling(index.row(), 0).data().toString();
	query.prepare("DELETE FROM seat_sales WHERE sale_id = :sale_id");
	query.bindValue(":sale_id", sale_id);
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
		qDebug() << "3";
	}
	query.clear();
	query.prepare("DELETE FROM Sales WHERE sale_id = :sale_id");
	query.bindValue(":sale_id", sale_id);
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
		qDebug() << "4";
	}
	QMessageBox::information(this, "Success", "Reservation canceled.");

	setTableView();
}


/**
 * @brief This function sets the table view to show the reservations. 
 */

void CancelReservation::setTableView() {
	QSqlQuery query(Database);
	query.prepare("SELECT seat_sales.sale_id, seat_id, Movies.title, Screenings.hall_id, Screenings.date_start, Sales.sale_date AS 'Ticket_sale_date' \
		FROM seat_sales \
		JOIN Screenings ON seat_sales.screening_id = Screenings.screening_id \
		JOIN Movies ON Screenings.movie_id = Movies.movie_id \
		JOIN Sales ON seat_sales.sale_id = Sales.sale_id \
		WHERE DATE(Screenings.date_start) >= DATE('now') \
		ORDER BY Screenings.date_start ASC; ");
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
	}
	QSqlQueryModel* model = new QSqlQueryModel();
	model->setQuery(query);
	ui.tableView->setModel(model);
	ui.tableView->resizeColumnsToContents();
	ui.tableView->show();
}
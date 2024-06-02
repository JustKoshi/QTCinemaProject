#include "EmployeePage.h"

EmployeePage::EmployeePage(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	id = 0;
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(connect_to_QTimer()));
	timer->start(1000);
	checkoutscreen = new CheckOutScreen();
	checkoutscreen->setLoginDatabase(loginDatabase);
	connect(checkoutscreen, SIGNAL(return_To_EmployeePage()), this, SLOT(showEmployeePage()));
	screeningselection = nullptr;
	
}

EmployeePage::~EmployeePage()
{
delete checkoutscreen;
delete timer;
delete screeningselection;
}

void EmployeePage::on_pushButton_checkOut_clicked() {
	this->hide();
	checkoutscreen->setName(label);
	checkoutscreen->show();
}

void EmployeePage::showEmployeePage() {
	this->show();
}

void EmployeePage::connect_to_QTimer() {
	QTime time = QTime::currentTime();
	QString time_text = time.toString("hh : mm : ss");
	ui.label_Time->setText(time_text);
}

void EmployeePage::setName(int new_name) {
	id = new_name;
	QSqlQuery query(loginDatabase);
	query.prepare("SELECT name, surname FROM EmployeeData WHERE employee_id = :id");
	query.bindValue(":id", id);
	if (query.exec()) {
		if (query.next()) {
			QString name = query.value(0).toString();
			QString surname = query.value(1).toString();
			label = "Employee:\n" + name + " " + surname;
			ui.label_name->setText(label);
		}
	}
	query.clear();
	query.prepare("SELECT shift_time FROM EmployeeData WHERE employee_id = :id");
	query.bindValue(":id", id);
	if (!query.exec()) {
	QMessageBox::critical(this, "Error", "Query error");
	}
	if (query.next()) {
			int etat = query.value(0).toInt();
			checkoutscreen->set_elapsed_shift_time(etat);
	}
	

}

void EmployeePage::on_pushButton_ticket_sale_clicked() {
	screeningselection = new ScreeningSelection();
	screeningselection->getDb(loginDatabase);
	connect(screeningselection, &ScreeningSelection::return_to_employee_page, this, &EmployeePage::showEmployeePage);
	screeningselection->show();
	this->hide();
}

void EmployeePage::on_pushButton_snack_sale_clicked() {
	SnackSales* snackSales = new SnackSales(nullptr, loginDatabase);
	connect(snackSales, &SnackSales::return_to_employee_page, this, &EmployeePage::showEmployeePage);
	snackSales->show();
	this->hide();
}

void EmployeePage::setLoginDatabase(QSqlDatabase db) {
	loginDatabase = db;
}

void EmployeePage::setTodaysScreenings() {
	QSqlQueryModel* model = new QSqlQueryModel();
	QSqlQuery query(loginDatabase);
	query.prepare("SELECT Screenings.screening_id, Movies.title , Screenings.hall_id, Screenings.date_start "
		"FROM Screenings, Movies "
		"WHERE Screenings.movie_id = Movies.movie_id "
		"AND DATE(Screenings.date_start) = DATE('now') "
		"ORDER BY TIME(Screenings.date_start) ASC");
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
	}
	model->setQuery(query);
	ui.tableView->setModel(model);
	ui.tableView->resizeColumnsToContents();
	ui.tableView->show();
}

void EmployeePage::on_pushButton_cancel_reservation_clicked() {
	CancelReservation* cancelReservation = new CancelReservation(nullptr, loginDatabase, id);
	connect(cancelReservation, &CancelReservation::return_to_employee_page, this, &EmployeePage::showEmployeePage);
	cancelReservation->show();
	this->hide();
}

void EmployeePage::on_pushButton_logout_clicked() {
	this->hide();
	emit return_To_loginPage();
}

void EmployeePage::on_pushButton_account_settings_clicked() {
	QMessageBox msgBox;
	msgBox.setWindowTitle("Change login or password");
	msgBox.setText("Do you want to change login or password?");
	QPushButton* loginButton = msgBox.addButton(tr("Login"), QMessageBox::ActionRole);
	QPushButton* passwordButton = msgBox.addButton(tr("Password"), QMessageBox::ActionRole);
	msgBox.addButton(QMessageBox::Cancel);
	msgBox.exec();

	if (msgBox.clickedButton() == loginButton) {
		changeLogin(loginDatabase, id);
	}
	else if (msgBox.clickedButton() == passwordButton) {
		changePassword(loginDatabase, id);
	}
}

void EmployeePage::on_pushButton_reports_clicked() {
	ReportPage * reportPage = new ReportPage(nullptr, loginDatabase);
	connect(reportPage, &ReportPage::return_To_AdminPage, this, &EmployeePage::showEmployeePage);
	reportPage->show();
}

#include "AdminPage.h"

/**
 * @file AdminPage.cpp
 * @brief This file contains the implementation of the functions to manage the admin page.
 */

AdminPage::AdminPage(QWidget *parent)
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
	moviemanager = nullptr;
	screeningmanager = nullptr;
}

AdminPage::~AdminPage()
{
delete checkoutscreen;
delete moviemanager;
delete screeningmanager;
delete timer;

}


/**
 * @brief This function shows the checkout screen.
 */
void AdminPage::on_pushButton_checkOut_clicked() {
	this->hide();
	checkoutscreen->setName(label);
	checkoutscreen->show();
}

/**
 * @brief This function shows the movie manager page and transfers database to it.
 */

void AdminPage::on_pushButton_movieManager_clicked() {
	this->hide();
	moviemanager = new MovieManager();
	moviemanager->GetDb(loginDatabase);
	moviemanager->show();
	connect(moviemanager, SIGNAL(return_To_AdminPage()), this, SLOT(showEmployeePage()));
}

/**
 * @brief This function shows the employee page. 
*/

void AdminPage::showEmployeePage() {
	setTodaysScreenings();
	this->show();
	
}

/**
 * @brief This function connects to QTimer.
 */

void AdminPage::connect_to_QTimer() {
	QTime time = QTime::currentTime();
	QString time_text = time.toString("hh : mm : ss");
	ui.label_Time->setText(time_text);
}

/**
 * @brief This function creates, transfers database and shows screening manager screeen 
 */

void AdminPage::on_pushButton_screeningManager_clicked() {
	this->hide();
	screeningmanager = new ScreeningManager();
	screeningmanager->getDb(loginDatabase);
	screeningmanager->show();
	connect(screeningmanager, SIGNAL(return_To_AdminPage()), this, SLOT(showEmployeePage()));
}

/**
 * @brief This function sets the name of the manager in the label.
 * * @param new_name The name of the manager.
 * */

void AdminPage::setName(int new_name) {
	id = new_name;
	QSqlQuery query(loginDatabase);
	query.prepare("SELECT name, surname FROM EmployeeData WHERE employee_id = :id");
	query.bindValue(":id", id);
	if (query.exec()) {
		if (query.next()) {
			QString name = query.value(0).toString();
			QString surname = query.value(1).toString();
			label = "Manager:\n" + name + " " + surname;
			ui.label_name->setText(label);
		}
	}
	query.clear();
	query.prepare("SELECT shift_time FROM EmployeeData WHERE employee_id = :id");
	query.bindValue(":id", id);
	if (query.exec()) {
		if (query.next()) {
			int etat = query.value(0).toInt();
			checkoutscreen->set_elapsed_shift_time(etat);
		}
	
	}
}

/**
 * @brief This function sets the database.
 * * @param db The database object that we use in whole project.
 * */

void AdminPage::setLoginDatabase(QSqlDatabase db) {
	loginDatabase = db;
}

/**
 * @brief This function shows the cancel reservation page.
 */

void AdminPage::on_pushButton_cancel_reservation_clicked() {
	this->hide();
	CancelReservation* cancelreservation = new CancelReservation(nullptr, loginDatabase, id);
	cancelreservation->show();
	connect(cancelreservation, SIGNAL(return_To_AdminPage()), this, SLOT(showEmployeePage()));
}

/**
 * @brief This function logs out the user and returns to the login page.
 */

void AdminPage::on_pushButton_logout_clicked() {
	this->hide();
	emit return_To_loginPage();
}

/**
 * @brief This function allows user to change login or password.
 */

void AdminPage::on_pushButton_account_settings_clicked() {
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

/**
 * @brief This function shows the report page and hides current one.
 */

void AdminPage::on_pushButton_reports_clicked() {
	ReportPage* reportpage = new ReportPage();
	reportpage->show();
	connect(reportpage, &ReportPage::return_To_AdminPage, this, &AdminPage::showEmployeePage);
}

/**
 * @brief This function sets the table view with today's screenings, gets data from database.
 */

void AdminPage::setTodaysScreenings() {
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

/**
 * @brief This function shows the employee manager page.
 */

void AdminPage::on_pushButton_employee_manager_clicked() {
	this->hide();
	EmployeeManager* employeemanager = new EmployeeManager(nullptr, loginDatabase);
	employeemanager->show();
	connect(employeemanager, &EmployeeManager::return_to_admin_page, this, &AdminPage::showEmployeePage);
}
#include "AdminPage.h"

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
}

AdminPage::~AdminPage()
{}

void AdminPage::on_pushButton_checkOut_clicked() {
	this->hide();
	checkoutscreen->setName(label);
	checkoutscreen->show();
}

void AdminPage::on_pushButton_movieManager_clicked() {
	this->hide();
	moviemanager = new MovieManager();
	moviemanager->GetDb(loginDatabase);
	moviemanager->show();
	connect(moviemanager, SIGNAL(return_To_AdminPage()), this, SLOT(showEmployeePage()));
}

void AdminPage::showEmployeePage() {
	this->show();
}

void AdminPage::connect_to_QTimer() {
	QTime time = QTime::currentTime();
	QString time_text = time.toString("hh : mm : ss");
	ui.label_Time->setText(time_text);
}

void AdminPage::setName(int new_name) {
	id = new_name;
	QSqlQuery query(loginDatabase);
	query.prepare("SELECT name, surname FROM EmployeeData WHERE employee_id = :id");
	query.bindValue(":id", id);
	if (query.exec()) {
		if (query.next()) {
			QString name = query.value(0).toString();
			QString surname = query.value(1).toString();
			label = "Manager: " + name + " " + surname;
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

void AdminPage::setLoginDatabase(QSqlDatabase db) {
	loginDatabase = db;
}
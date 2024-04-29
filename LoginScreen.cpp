#include "LoginScreen.h"

LoginScreen::LoginScreen(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	loginDatabase = QSqlDatabase::addDatabase("QSQLITE");
	loginDatabase.setDatabaseName("F:/Project Qt/Kino - projekt/login.db");
	if (loginDatabase.open()) {
		qDebug() << "Database connected";
	}
	else
		{
		qDebug() << "Database not connected";
	}
	

}

void LoginScreen::on_pushButton_Login_clicked() {
	QString username = ui.lineEdit_Username->text();
	QString password = ui.lineEdit_Password->text();
	bool success_manager = false;
	bool success_employee = false;
	QSqlQuery query;
	QString stanowisko;
	query.prepare("SELECT * FROM LoginData WHERE LOGIN = :username AND PASSWORD = :password");
	query.bindValue(":username", username);
	query.bindValue(":password", password);
	if (query.exec()) {
		if (query.next()) {
			stanowisko = query.value(3).toString();
			if (stanowisko == "kierownik") {
				success_manager = true;
			}
			else if (stanowisko == "pracownik") {
				success_employee = true;
			}
		}
	}
	if (success_manager) {
		QMessageBox::information(this, "Login", "Username and password is correct");
		this->hide();
		AdminPage* adminpage = new AdminPage();
		adminpage->show();
	}
	else if(success_employee)
	{
		QMessageBox::information(this, "Login", "Username and password is correct");
		this->hide();
		EmployeePage* employee_page = new EmployeePage();
		employee_page->show();
	}
	else
		QMessageBox::warning(this, "Login", "Username and password is not correct");


}

void LoginScreen::on_pushButton_Quit_clicked() {
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, "Quit", "Are you sure you want to quit?",
		QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::Yes) {
		QApplication::quit();
	}
}


LoginScreen::~LoginScreen()
{}

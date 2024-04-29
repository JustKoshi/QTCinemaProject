#include "LoginScreen.h"

LoginScreen::LoginScreen(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	adminpage = new AdminPage();
	employee_page = new EmployeePage();
	loginDatabase = QSqlDatabase::addDatabase("QSQLITE");
	loginDatabase.setDatabaseName("F:/Project Qt/Kino - projekt/login.db");
	if (loginDatabase.open()) {
		qDebug() << "Database connected";
	}
	else
		{
		qDebug() << "Database not connected";
	}
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(connect_to_QTimer()));
	timer->start(1000);
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
		adminpage->show();
		employee_page->close();
	}
	else if(success_employee)
	{
		QMessageBox::information(this, "Login", "Username and password is correct");
		this->hide();
		adminpage->close();
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

void LoginScreen::connect_to_QTimer() {
	QTime time = QTime::currentTime(); 
	QString time_text = time.toString("hh : mm : ss");
	ui.label_Time->setText(time_text);
}

LoginScreen::~LoginScreen()
{}

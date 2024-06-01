#include "LoginScreen.h"

LoginScreen::LoginScreen(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	adminpage = new AdminPage();
	employee_page = new EmployeePage();
	adminpage->setLoginDatabase(loginDatabase);
	employee_page->setLoginDatabase(loginDatabase);
	
	loginDatabase = QSqlDatabase::addDatabase("QSQLITE");
	QString pathToDB = QDir(QCoreApplication::applicationDirPath()).filePath("login.db");
	loginDatabase.setDatabaseName(pathToDB);

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
	int stanowisko;
	int id;
	query.prepare("SELECT * FROM LoginData WHERE login = :username AND password = :password");
	query.bindValue(":username", username);
	query.bindValue(":password", password);
	if (query.exec()) {
		if (query.next()) {
			stanowisko = query.value(3).toInt();
			if (stanowisko == 2) {
				success_manager = true;
				id = query.value(0).toInt();
			}
			else if (stanowisko == 1) {
				success_employee = true;
				id = query.value(0).toInt();
			}
		}
	}
	if (success_manager) {

		adminpage->setName(id);
		QMessageBox::information(this, "Login", "Username and password is correct");
		this->hide();
		adminpage->show();
		employee_page->close();
		connect(adminpage, &AdminPage::return_To_loginPage, this, &LoginScreen::showLoginPage);
	}
	else if(success_employee)
	{

		employee_page->setName(id);
		QMessageBox::information(this, "Login", "Username and password is correct");
		employee_page->setTodaysScreenings();
		this->hide();
		adminpage->close();
		employee_page->show();
		connect(employee_page, &EmployeePage::return_To_loginPage, this, &LoginScreen::showLoginPage);
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
{
	delete adminpage;
	delete employee_page;
}

void LoginScreen::showLoginPage() {
	
	if(adminpage!=nullptr)
		adminpage->close();
	if(employee_page!=nullptr)
		employee_page->close();
	ui.lineEdit_Username->clear();
	ui.lineEdit_Password->clear();
	this->show();
}

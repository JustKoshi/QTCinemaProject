#include "AdminPage.h"

AdminPage::AdminPage(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	name = "Manager: ";
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(connect_to_QTimer()));
	timer->start(1000);
	checkoutscreen = new CheckOutScreen();
	checkoutscreen->setName(name);
	connect(checkoutscreen, SIGNAL(return_To_EmployeePage()), this, SLOT(showEmployeePage()));
}

AdminPage::~AdminPage()
{}

void AdminPage::on_pushButton_checkOut_clicked() {
	this->hide();
	checkoutscreen->setName(name);
	checkoutscreen->show();
}

void AdminPage::showEmployeePage() {
	this->show();
}

void AdminPage::connect_to_QTimer() {
	QTime time = QTime::currentTime();
	QString time_text = time.toString("hh : mm : ss");
	ui.label_Time->setText(time_text);
}

void AdminPage::setName(QString new_name) {
	name += new_name;
	ui.label_name->setText(name);
}
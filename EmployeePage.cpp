#include "EmployeePage.h"

EmployeePage::EmployeePage(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	name = "Employee: ";
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(connect_to_QTimer()));
	timer->start(1000);
	checkoutscreen = new CheckOutScreen();
	connect(checkoutscreen, SIGNAL(return_To_EmployeePage()), this, SLOT(showEmployeePage()));

	
}

EmployeePage::~EmployeePage()
{}

void EmployeePage::on_pushButton_checkOut_clicked() {
	this->hide();
	checkoutscreen->setName(name);
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

void EmployeePage::setName(QString new_name) {
	name += new_name;
	ui.label_name->setText(name);
}
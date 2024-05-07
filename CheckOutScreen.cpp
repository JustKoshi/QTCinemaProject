#include "CheckOutScreen.h"

CheckOutScreen::CheckOutScreen(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	current_time = new QTimer(this);
	connect(current_time, SIGNAL(timeout()), this, SLOT(connect_to_QCurrent_Time()));
	current_time->start(1000);
	connect(ui.pushButton_return, SIGNAL(clicked()), this, SLOT(on_pushButton_return_clicked()));
}

CheckOutScreen::~CheckOutScreen()
{}

void CheckOutScreen::on_pushButton_return_clicked() {
	this->hide();
	emit return_To_EmployeePage();
}

void CheckOutScreen::connect_to_QCurrent_Time() {
	QTime time = QTime::currentTime();
	QString time_text = time.toString("hh : mm : ss");
	ui.label_time_current->setText(time_text);
}

void CheckOutScreen::setName(QString newname) {
	name = newname;
	ui.label_name->setText(name);
}

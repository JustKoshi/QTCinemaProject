#include "CheckOutScreen.h"

CheckOutScreen::CheckOutScreen(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	current_time = new QTimer(this);
	timer = new QTimer(this);
	elapsed_break_time = QTime(0, 0, 0);
	timer_started = false;
	connect(current_time, SIGNAL(timeout()), this, SLOT(connect_to_QCurrent_Time()));
	connect(timer, SIGNAL(timeout()), this, SLOT(update_break_time()));
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

void CheckOutScreen::update_break_time() {
	elapsed_break_time = elapsed_break_time.addSecs(1);
	QString time_text = elapsed_break_time.toString("hh:mm:ss");
	
	ui.label_break_time->setText(time_text);
}

void CheckOutScreen::on_pushButton_break_start_clicked() {
	if(timer_started) {
		QMessageBox::warning(this, "B³¹d", "Przerwa zosta³a ju¿ rozpoczêta.");
		return;
	}
	else {
		timer->start(1000);
		timer_started = true;
	}
}

void CheckOutScreen::on_pushButton_break_end_clicked() {
	if (timer_started) {
		timer->stop();
		
		if (elapsed_break_time.msecsSinceStartOfDay() < 180000) {
			QMessageBox::information(this, "Przerwa trwa", "Przerwa nadal trwa.");
			timer->start();
		}
	}
	else {
		
		QMessageBox::warning(this, "B³¹d", "Przerwa nie zosta³a jeszcze rozpoczêta.");
	}
	
}
#include "CheckOutScreen.h"

CheckOutScreen::CheckOutScreen(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	shift_timer = new QTimer(this);
	current_time = new QTimer(this);
	timer = new QTimer(this);
	elapsed_shift_time = QTime(0, 0, 0);
	elapsed_break_time = QTime(0, 0, 0);
	timer_started = false;
	shift_timer_started = false;
	current_time->start(1000);
	connect(current_time, SIGNAL(timeout()), this, SLOT(connect_to_QCurrent_Time()));
	connect(timer, SIGNAL(timeout()), this, SLOT(update_break_time()));
	connect(shift_timer, SIGNAL(timeout()), this, SLOT(update_shift_time()));
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

void CheckOutScreen::update_shift_time() {
	elapsed_shift_time = elapsed_shift_time.addSecs(-1);
	QString time_text = elapsed_shift_time.toString("hh:mm:ss");
	ui.label_shift_time->setText(time_text);
}

void CheckOutScreen::on_pushButton_shift_start_clicked() {
	if (shift_timer_started) {
		QMessageBox::warning(this, "Blad", "Zmiana zostala juz rozpoczeta.");
		return;
	}
	else {
		shift_timer_started = true;
		shift_timer->start(1000);
	}

}

void CheckOutScreen::on_pushButton_shift_end_clicked() {
	if (shift_timer_started) {
		shift_timer->stop();
		if (elapsed_shift_time.msecsSinceStartOfDay() > 0) {
			QMessageBox::information(this, "Zmiana trwa", "Zmiana nadal trwa.");
			shift_timer->start();
		}
		else {
			QMessageBox::information(this, "Zmiana zakonczona", "Zmiana zostala zakonczona.");
		}
	}
	
	
}


void CheckOutScreen::update_break_time() {
	elapsed_break_time = elapsed_break_time.addSecs(1);
	QString time_text = elapsed_break_time.toString("hh:mm:ss");
	ui.label_break_time->setText(time_text);
}

void CheckOutScreen::on_pushButton_break_start_clicked() {
	if(shift_timer_started){
		if(timer_started) {
			QMessageBox::warning(this, "Blad", "Przerwa zostala juz rozpoczeta.");
			return;
		}
		else {
			timer->start(1000);
			shift_timer->stop();
			shift_timer_started = false;
			timer_started = true;
		}
	}
	else if (!shift_timer_started&& timer_started){
		QMessageBox::warning(this, "Blad", "Przerwa zostala juz rozpoczeta.");
	}
	else {
		QMessageBox::warning(this, "Blad", "Zmiana nie zostala jeszcze rozpoczeta.");
	}
}

void CheckOutScreen::on_pushButton_break_end_clicked() {
	if (timer_started) {
		timer->stop();
		if (elapsed_break_time.msecsSinceStartOfDay() < 180000) {
			QMessageBox::information(this, "Przerwa trwa", "Przerwa nadal trwa.");
			timer->start();
		}
		else {
			QMessageBox::information(this, "Przerwa zakonczona", "Przerwa zostala zakonczona.");
			shift_timer->start();
			shift_timer_started = true;
		}
	}
	else {
		
		QMessageBox::warning(this, "Blad", "Przerwa nie zostala jeszcze rozpoczeta.");
	}
	
}

void CheckOutScreen::setLoginDatabase(QSqlDatabase db) {
	loginDb = db;
}

void CheckOutScreen::setName(QString newname) {
	name = newname;
	ui.label_name->setText(name);
}

void CheckOutScreen::set_elapsed_shift_time(int time) {
	elapsed_shift_time = QTime(time,0,0);
	QString time_text = elapsed_shift_time.toString("hh:mm:ss");
	ui.label_shift_time->setText(time_text);
}
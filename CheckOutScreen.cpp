#include "CheckOutScreen.h"

/**
 * @file CheckOutScreen.cpp
 * @brief This file contains the implementation of the functions to manage the checkout screen that allows user to start and end/shifts and breaks.
 */

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

/**
 * @brief This function returns to the employee menu page and closes this page. 
 * 
 * This function emits the return_To_EmployeePage signal.
 */

void CheckOutScreen::on_pushButton_return_clicked() {
	this->hide();
	emit return_To_EmployeePage();
}

/**
 * @brief This function connects to the QTimer to update the current time.
 * 
 * This function connects to the QTimer to update the current time. It then displays the updated current time in the QLabel.
 */

void CheckOutScreen::connect_to_QCurrent_Time() {
	QTime time = QTime::currentTime();
	QString time_text = time.toString("hh : mm : ss");
	ui.label_time_current->setText(time_text);
}


/**
 * @brief This function updates the shift time.
 * 
 * This function updates the shift time by subtracting one second from the elapsed shift time. It then displays the updated shift time in the QLabel.
 */

void CheckOutScreen::update_shift_time() {
	elapsed_shift_time = elapsed_shift_time.addSecs(-1);
	QString time_text = elapsed_shift_time.toString("hh:mm:ss");
	ui.label_shift_time->setText(time_text);
}


/**
 * @brief This function starts the shift timer.
 * 
 * This function starts the shift timer and checks if the shift timer has already been started. If the shift timer has already been started, it displays a message box.
 */

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


/**
 * @brief This function ends the shift timer and checks if shift time has ended. 
 * 
 * This function stops the shift timer and checks if the shift time has ended. If the shift time has not ended, it displays a message box.
 */

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

/**
 * @brief This function updates the break time.
 * 
 */

void CheckOutScreen::update_break_time() {
	elapsed_break_time = elapsed_break_time.addSecs(1);
	QString time_text = elapsed_break_time.toString("hh:mm:ss");
	ui.label_break_time->setText(time_text);
}

/**
 * @brief This function starts the break timer. 
 * 
 * This function starts the break timer and stops the shift timer.
 */

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

/**
 * @brief This function ends the break timer and checks if break time has ended.
 */

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

/**
 * @brief This function sets the login database.
 * * @param db The database object.
 */

void CheckOutScreen::setLoginDatabase(QSqlDatabase db) {
	loginDb = db;
}

/**
 * @brief This function sets the name of the employee.
 * * @param newname The name of the employee.
 */

void CheckOutScreen::setName(QString newname) {
	name = newname;
	ui.label_name->setText(name);
}

/**
 * @brief This function sets the elapsed shift time.
 * * @param time The time of the shift saved in database.
 */

void CheckOutScreen::set_elapsed_shift_time(int time) {
	elapsed_shift_time = QTime(time,0,0);
	QString time_text = elapsed_shift_time.toString("hh:mm:ss");
	ui.label_shift_time->setText(time_text);
}


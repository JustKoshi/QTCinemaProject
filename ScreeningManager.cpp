#include "ScreeningManager.h"

/**
 * @file ScreeningManager.cpp
 * @brief This file contains the implementation of the functions to manage the screening manager page.
 * @class ScreeningManager
 */


ScreeningManager::ScreeningManager(QWidget *parent)
	: QWidget(parent)
{
	
	ui.setupUi(this);
	ui.dateEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	connect(ui.pushButton_return, SIGNAL(clicked()), this, SLOT(on_pushButton_return_clicked()));
	ui.dateEdit->setDate(QDate::currentDate());
	QSqlQuery query(loginDb);
	
	query.prepare("SELECT title FROM Movies");
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
	}
	while (query.next()) {
		ui.comboBox_movies->addItem(query.value(0).toString());
	}
	query.prepare("SELECT DISTINCT hall_id FROM Seats");
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
	}
	while (query.next()) {
		ui.comboBox_hall->addItem(query.value(0).toString());
	}

	query.prepare("SELECT screening_id FROM Screenings");
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
	}
	while (query.next()) {
		ui.comboBox_screening_id->addItem(query.value(0).toString());
	}

	connect(ui.pushButton_12_00, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
	connect(ui.pushButton_15_00, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
	connect(ui.pushButton_18_00, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
	connect(ui.pushButton_21_00, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
	
	QList<QPushButton*> buttons = findChildren<QPushButton*>();


}

ScreeningManager::~ScreeningManager()
{
	for (QPushButton* button : findChildren<QPushButton*>()) {
		delete button;
	}
}


/**
* @brief This function changes the color of the button when clicked and sets the time of the screening.
*/

void ScreeningManager::onButtonClicked() {
	QPushButton* button = qobject_cast<QPushButton*>(sender());
	if (button) {
		for (QPushButton* otherButton : findChildren<QPushButton*>()) {
			if (otherButton->objectName() == "pushButton_calendar")
				continue;
			otherButton->setStyleSheet("QPushButton {"
				"background-color: rgb(255, 255, 242);"
				"border: 1px solid black;"
				"border-radius: 6px;"
				"padding: 6px;"
				"}"
				"QPushButton:hover {"
				"background-color: rgb(182, 178, 135);"
				"}"
				"QPushButton:pressed {"
				"background-color: #d0d0d0;"
				"}");
		}
		button->setStyleSheet("QPushButton {"
			"background-color: green;"
			"border: none;"
			"border-radius: 6px;"
			"padding: 6px;"
			"color: white;"
			"}"
			"QPushButton:hover {"
			"background-color: #008000;"
			"}"
			"QPushButton:pressed {"
			"background-color: #006400;"
			"}");
		time = button->text();
		
	}
}

/**
* @brief This function adds the screening to the database.
* 
*/


void ScreeningManager::on_pushButton_add_screening_clicked() {
	if (time == "") {
		QMessageBox::critical(this, "Error", "Choose time");
		return;
	}
	QDateTime currentDateTime = QDateTime::currentDateTime();
	QDateTime screeningDateTime = QDateTime::fromString(ui.dateEdit->date().toString("yyyy-MM-dd") + " " + time, "yyyy-MM-dd hh:mm");
	if (screeningDateTime < currentDateTime) {
		QMessageBox::critical(this, "Error", "You can't add screening in the past");
		return;
	}
	QSqlQuery query(loginDb);
	
	query.prepare("SELECT * FROM Screenings WHERE hall_id = :hall_id AND date_start = :date_start");
	query.bindValue(":hall_id", ui.comboBox_hall->currentText());
	query.bindValue(":date_start", ui.dateEdit->date().toString("yyyy-MM-dd") + " " + time);
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
	}
	if (query.next()) {
		QMessageBox::critical(this, "Error", "There is already screening at this time and hall");
		return;
	}
	query.prepare("SELECT movie_id FROM Movies WHERE title = :title");
	query.bindValue(":title", ui.comboBox_movies->currentText());
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
	}
	query.next();
	int movie_id = query.value(0).toInt();
	query.prepare("INSERT INTO Screenings (movie_id, hall_id, date_start) VALUES (:movie_id, :hall_id, :date_start)");
	query.bindValue(":movie_id", movie_id);
	query.bindValue(":hall_id", ui.comboBox_hall->currentText());
	query.bindValue(":date_start", ui.dateEdit->date().toString("yyyy-MM-dd") + " " + time);
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
	}

	QMessageBox::information(this, "Success", "Screening added");
	QSqlQueryModel* model = new QSqlQueryModel();
	query.prepare("SELECT Screenings.screening_id, Movies.title , Screenings.hall_id, Screenings.date_start "
		"FROM Screenings, Movies "
		"WHERE Screenings.movie_id = Movies.movie_id "
		"AND DATE(Screenings.date_start) >= DATE('now') "
		"AND TIME(Screenings.date_start) > TIME('now')"
		"ORDER BY TIME(Screenings.date_start) ASC"
	);
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
	}
	model->setQuery(query);
	ui.screening_table->setModel(model);
	ui.screening_table->resizeColumnsToContents();
	ui.screening_table->show();
}

/**
* @brief This function deletes the screening from the database after user confirmation.
*/

void ScreeningManager::on_pushButton_delete_screening_clicked() {
	QSqlQuery query(loginDb);
	if (ui.comboBox_screening_id->currentText() == "") {
		QMessageBox::critical(this, "Error", "There is no screening to delete.");
		return;
	}
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, "Delete", "Are you sure you want to delete this screening?",
				QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::No) {
		return;
	}
	query.prepare("DELETE FROM Screenings WHERE screening_id = :screening_id");
	query.bindValue(":screening_id", ui.comboBox_screening_id->currentText());
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
	}
	QMessageBox::information(this, "Success", "Screening deleted");
	QSqlQueryModel* model = new QSqlQueryModel();
	query.prepare("SELECT Screenings.screening_id, Movies.title , Screenings.hall_id, Screenings.date_start "
			"FROM Screenings, Movies "
			"WHERE Screenings.movie_id = Movies.movie_id "
			"AND DATE(Screenings.date_start) >= DATE('now') "
			"AND TIME(Screenings.date_start) > TIME('now')"
			"ORDER BY TIME(Screenings.date_start) ASC");

	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
	}
	model->setQuery(query);
	ui.screening_table->setModel(model);
	ui.screening_table->resizeColumnsToContents();
	ui.screening_table->show();
	query.prepare("SELECT screening_id FROM Screenings");
	while (query.next()) {
		ui.comboBox_screening_id->addItem(query.value(0).toString());
	}
}

/**
* @brief This function gets the database and sets the table view to show the screenings.
* @param db - database
* 
* This function gets the database and sets the table view to show the screenings. It creates a query to get the screenings that are in the future and orders them by time.
*/

void ScreeningManager::getDb(QSqlDatabase db)
{
	loginDb = db;
	QSqlQueryModel* model = new QSqlQueryModel();
	QSqlQuery query(loginDb);
	query.prepare("SELECT Screenings.screening_id, Movies.title , Screenings.hall_id, Screenings.date_start "
		"FROM Screenings, Movies "
		"WHERE Screenings.movie_id = Movies.movie_id "
		"AND DATE(Screenings.date_start) >= DATE('now') "
		"AND TIME(Screenings.date_start) > TIME('now')"
		"ORDER BY TIME(Screenings.date_start) ASC");
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
	}
	model->setQuery(query);
	ui.screening_table->setModel(model);
	ui.screening_table->resizeColumnsToContents();
	ui.screening_table->show();
}


/**
* @brief This function returns to the admin page.
*/

void ScreeningManager::on_pushButton_return_clicked() {
	this->close();
	emit return_To_AdminPage();
}

/**
* @brief This function shows the calendar to choose the date.
* 
* This function creates a calendar widget and shows it when the button is clicked. It then sets the date in the dateEdit to the selected date.
*/

void ScreeningManager::on_pushButton_calendar_clicked() {
	QCalendarWidget* calendar = new QCalendarWidget();
	calendar->setMinimumDate(QDate::currentDate());
	calendar->setSelectedDate(QDate::currentDate());
	calendar->setWindowFlags(Qt::Window);
	calendar->setStyleSheet("background-color: white; \
                             font-size: 12px; \
                             border: 1px solid #ccc; \
                             color: #333;");
	calendar->resize(400, 300); 

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(calendar);

	QWidget* window = new QWidget();
	window->setWindowFlags(Qt::Window | Qt::FramelessWindowHint); 
	window->setLayout(layout);
	window->resize(400, 300); 
	QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
	int x = (screenGeometry.width() - window->width()) / 2;
	int y = (screenGeometry.height() - window->height()) / 2;
	window->move(x, y);
	connect(calendar, &QCalendarWidget::selectionChanged, this, [this, calendar, window]() {
		ui.dateEdit->setDate(calendar->selectedDate());
		window->close();
		});

	window->show();
}
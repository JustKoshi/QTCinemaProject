#include "ScreeningManager.h"

ScreeningManager::ScreeningManager(QWidget *parent)
	: QWidget(parent)
{
	
	ui.setupUi(this);
	connect(ui.pushButton_return, SIGNAL(clicked()), this, SLOT(on_pushButton_return_clicked()));
	ui.dateEdit->setDate(QDate::currentDate());
	QSqlQuery query(loginDb);
	
	query.prepare("DELETE FROM Screenings WHERE DATE(date_start) < DATE('now')");
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
	}

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
	//add screening ids to combobox
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
	for (QPushButton* button : findChildren<QPushButton*>()) {
		button->setStyleSheet("QPushButton {"
			"background-color: none;"
			"border: 1px solid #8f8f91;"
			"border-radius: 6px;"
			"padding: 6px;"
			"}"
			"QPushButton:hover {"
			"background-color: #b8b8b8;"
			"}"
			"QPushButton:pressed {"
			"background-color: #d0d0d0;"
			"}");
	}


}

ScreeningManager::~ScreeningManager()
{
	for (QPushButton* button : findChildren<QPushButton*>()) {
		delete button;
	}
}

void ScreeningManager::onButtonClicked() {
	QPushButton* button = qobject_cast<QPushButton*>(sender());
	if (button) {
		for (QPushButton* otherButton : findChildren<QPushButton*>()) {
			otherButton->setStyleSheet("QPushButton {"
				"background-color: none;"
				"border: 1px solid #8f8f91;"
				"border-radius: 6px;"
				"padding: 6px;"
				"}"
				"QPushButton:hover {"
				"background-color: #b8b8b8;"
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
	//check if there is already screening at this time and hall
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
	ui.screening_table->show();
}

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
	ui.screening_table->show();
	query.prepare("SELECT screening_id FROM Screenings");
	while (query.next()) {
		ui.comboBox_screening_id->addItem(query.value(0).toString());
	}
}

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
	ui.screening_table->show();
}


void ScreeningManager::on_pushButton_return_clicked() {
	this->close();
	emit return_To_AdminPage();
}
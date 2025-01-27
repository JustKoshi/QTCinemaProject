#include "ScreeningSelection.h"

/**
 * @file ScreeningSelection.cpp
 * @brief This file contains the implementation of the functions to manage the screening selection page.
 * @class ScreeningSelection
 * 
 */


ScreeningSelection::ScreeningSelection(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	QWidget* centralWidget = new QWidget(this);
	setCentralWidget(centralWidget);

	mainlayout = new QVBoxLayout(centralWidget);

	scrollArea = new QScrollArea(this);
	scrollAreaWidgetContents = new QWidget(scrollArea);
	scrolllayout = new QVBoxLayout(scrollAreaWidgetContents);

	scrollArea->setWidget(scrollAreaWidgetContents);
	scrollArea->setWidgetResizable(true);
	mainlayout->addWidget(scrollArea);
	



	QSqlQuery query(loginDb);

	query.prepare(
		"SELECT title, poster FROM Movies "
		"WHERE movie_id IN ("
		"    SELECT movie_id FROM Screenings "
		"    WHERE DATE(date_start) = DATE('now') "
		"    AND datetime(date_start) >= datetime('now', '-30 minutes')"
		")"
	);


	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
		qDebug() << query.lastError().text();
		return;
	}
	while (query.next()) {
		QString title = query.value(0).toString();
		QByteArray bytes = query.value(1).toByteArray();
		QPixmap image;
		image.loadFromData(bytes);
		createPage(title, image);
	}
	this->setMinimumSize(800, 600);
	this->setStyleSheet(R"(
		QWidget {
			background-color: rgb(50,50,50);
			font-size: 10pt;
		}
		QPushButton {
			background-color: white; 
			border: none; 
			padding: 5px 10px; 
			border-radius: 3px; 
			color: black; 
		}

		QPushButton:hover {
			background-color: #005F91;
		}

		QPushButton:pressed {
			background-color: #003F5F; 
		}
		QGroupBox {
			color: white;
			font-size: 12pt;
		}
	)");
	this->setMaximumWidth(800);

}

ScreeningSelection::~ScreeningSelection()
{}

/**
 * @brief This function gets the database.
 * @param db - the database to be loaded
 */

void ScreeningSelection::getDb(QSqlDatabase db) {
	loginDb = db;
}

/**
 * @brief This function creates a page with the movie title and the screening times depending on how many screenings is in the database.
 * @param title - the title of the movie
 * @param image - the image of the movie
 * @return void
 * @exception none
 * 
 * This function creates a groupbox with the title of the movie and the image of the movie. It then creates a query to get the screening times of the movie. It then creates a button for each screening time and connects the button to the onButtonClicked function.
 */

void ScreeningSelection::createPage(const QString& title, const QPixmap& image) {
	
	QGroupBox* groupbox = new QGroupBox(title, this);
	QHBoxLayout* layout = new QHBoxLayout(groupbox);
	QLabel* label = new QLabel(groupbox);

	label->setPixmap(image);
	label->setMinimumSize(400, 200);
	layout->addWidget(label);
	
	QSqlQuery query(loginDb);
	query.prepare("SELECT date_start, screening_id FROM Screenings WHERE movie_id = (SELECT movie_id FROM Movies WHERE title = :title) AND DATE(date_start) = DATE('now')");
	query.bindValue(":title", title);
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
		qDebug() << query.lastError().text();
		return;
	}
	
	while (query.next()) {
		QString date = query.value(0).toString();
		QString time = date.mid(11, 5);
		QString screeningId = query.value(1).toString();
		auto button = new QPushButton(time, groupbox);
		button->setObjectName(screeningId);
		connect(button, &QPushButton::clicked, this, [this, screeningId]() {
			this->onButtonClicked(screeningId);
			});
		layout->addWidget(button);
	}


	groupbox->setLayout(layout);
	scrolllayout->addWidget(groupbox);
	scrollAreaWidgetContents->setLayout(scrolllayout);
}

/**
 * @brief This function opens the projection screen. It is called when the button is clicked.
 */


void ScreeningSelection::onButtonClicked(QString id) {
	ProjectionScreen* projectionScreen = new ProjectionScreen(nullptr, id, this->loginDb);
	connect(projectionScreen, &ProjectionScreen::return_to_employee_page, this, &ScreeningSelection::return_to_employee_page);
	projectionScreen->show();
	this->hide();
}

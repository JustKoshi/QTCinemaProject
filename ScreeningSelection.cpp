#include "ScreeningSelection.h"

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
	
	query.prepare("SELECT title, poster FROM Movies");
	
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
		QSqlQuery screeningQuery(loginDb);
		screeningQuery.prepare("SELECT COUNT(*) FROM Screenings WHERE movie_id = (SELECT movie_id FROM Movies WHERE title = :title) AND DATE(date_start) = DATE('now')");
		screeningQuery.bindValue(":title", title);
		if (!screeningQuery.exec()) {
			QMessageBox::critical(this, "Error", "Query error");
			qDebug() << screeningQuery.lastError().text();
			continue;

		}
		if (screeningQuery.next()) {
			int count = screeningQuery.value(0).toInt();
			createPage(title, image);
		}

	}
	//set minim size of the window
	this->setMinimumSize(800, 600);


}

ScreeningSelection::~ScreeningSelection()
{}

void ScreeningSelection::getDb(QSqlDatabase db) {
	loginDb = db;
}

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

void ScreeningSelection::onButtonClicked(QString id) {
	ProjectionScreen* projectionScreen = new ProjectionScreen(nullptr, id, this->loginDb);
	connect(projectionScreen, &ProjectionScreen::return_to_employee_page, this, &ScreeningSelection::return_to_employee_page);
	projectionScreen->show();
	this->hide();
}

#include "ProjectionScreen.h"

ProjectionScreen::ProjectionScreen(QWidget *parent, QString new_id, QSqlDatabase db)
	: QWidget(parent), id(new_id), loginDb(db)
{
	ui.setupUi(this);
	QSqlQuery query(loginDb);
	query.prepare("SELECT title, date_start, hall_id FROM Screenings, Movies WHERE screening_id = :id AND Screenings.movie_id = Movies.movie_id");
	query.bindValue(":id", id);
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
	}
	if (query.next()) {
		QString title = query.value(0).toString();
		QString date = query.value(1).toString();
		hall_id = query.value(2).toString();
		ui.label->setText(title + " " + date + " in hall: " + hall_id);
	}
	connect(ui.A1, SIGNAL(clicked()), this, SLOT(handle_button_click()));
	connect(ui.A2, SIGNAL(clicked()), this, SLOT(handle_button_click()));
	connect(ui.A3, SIGNAL(clicked()), this, SLOT(handle_button_click()));
	connect(ui.A4, SIGNAL(clicked()), this, SLOT(handle_button_click()));
	connect(ui.A5, SIGNAL(clicked()), this, SLOT(handle_button_click()));
	connect(ui.B1, SIGNAL(clicked()), this, SLOT(handle_button_click()));
	connect(ui.B2, SIGNAL(clicked()), this, SLOT(handle_button_click()));
	connect(ui.B3, SIGNAL(clicked()), this, SLOT(handle_button_click()));
	connect(ui.B4, SIGNAL(clicked()), this, SLOT(handle_button_click()));
	connect(ui.B5, SIGNAL(clicked()), this, SLOT(handle_button_click()));
	connect(ui.C1, SIGNAL(clicked()), this, SLOT(handle_button_click()));
	connect(ui.C2, SIGNAL(clicked()), this, SLOT(handle_button_click()));
	connect(ui.C3, SIGNAL(clicked()), this, SLOT(handle_button_click()));
	connect(ui.C4, SIGNAL(clicked()), this, SLOT(handle_button_click()));
	connect(ui.C5, SIGNAL(clicked()), this, SLOT(handle_button_click()));
	connect(ui.D1, SIGNAL(clicked()), this, SLOT(handle_button_click()));
	connect(ui.D2, SIGNAL(clicked()), this, SLOT(handle_button_click()));
	connect(ui.D3, SIGNAL(clicked()), this, SLOT(handle_button_click()));
	connect(ui.D4, SIGNAL(clicked()), this, SLOT(handle_button_click()));
	connect(ui.D5, SIGNAL(clicked()), this, SLOT(handle_button_click()));
	

}

ProjectionScreen::~ProjectionScreen()
{}

void ProjectionScreen::handle_button_click() {
	QPushButton* button = qobject_cast<QPushButton*>(sender());
	if (button) {
		QString seat_id = hall_id + button->objectName();
		QSqlQuery query(loginDb);
		query.prepare("SELECT price FROM Seats WHERE seat_id = :id");
		query.bindValue(":id", seat_id);
		qDebug() << seat_id;
		if (!query.exec()) {
			QMessageBox::critical(this, "Error", "Query error");
		}
		query.next();
		QString price = query.value(0).toString();
		QString ticket = "Ticket for seat " + button->objectName() + " - " + price + " zl";
		if (button->styleSheet() == "border-image: url(:/ProjectionScreen/Resource files/seat3.png);") {
			button->setStyleSheet("border-image: url(:/ProjectionScreen/Resource files/seat.png);");
			QList<QListWidgetItem*> items = ui.listWidget->findItems(ticket, Qt::MatchExactly);
			for (auto item : items) {
				int row = ui.listWidget->row(item);
				delete ui.listWidget->takeItem(row);
			}
			if (!seats_ids.empty()) {
				for (auto item : seats_ids)
					if (item == hall_id + button->objectName())
						seats_ids.erase(std::remove(seats_ids.begin(), seats_ids.end(), item), seats_ids.end());
		
			}
			ui.listWidget->update();
			
		}
		else {
			button->setStyleSheet("border-image: url(:/ProjectionScreen/Resource files/seat3.png);");
			seats_ids.push_back(hall_id+button->objectName());
			QListWidgetItem* item = new QListWidgetItem(ticket);
			ui.listWidget->addItem(item);
			ui.listWidget->update();
		}
	}
}

void ProjectionScreen::on_pushButton_proceed_clicked() {
	//if seats_ids is empty pop up a waring and ask if want to proceed
	if (seats_ids.empty()) {
		QMessageBox::StandardButton reply = QMessageBox::question(this, "Warning", "No seats selected. Do you want to proceed?", QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::No) {
			return;
		}
		else {
			;
		}
	}
	else {
		QMessageBox::StandardButton reply = QMessageBox::question(this, "Proceed", "Are you sure you want to proceed?", QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::No) {
			return;
		}
		else {
			;
		}
	}
}
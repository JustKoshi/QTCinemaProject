#include "ProjectionScreen.h"


/**
 * @file ProjectionScreen.cpp
 * @brief This file contains the implementation of the functions to manage the projection screen.
 */


ProjectionScreen::ProjectionScreen(QWidget *parent, QString new_id, QSqlDatabase db)
	: QWidget(parent), id(new_id), loginDb(db)
{
	ui.setupUi(this);
	QSqlQuery query(loginDb);
	snacksalespage = nullptr;
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
	
    total_price = 0.;
	QString price_text = "Total price: " + QString::number(total_price) + " zl";
	QListWidgetItem* item = new QListWidgetItem(price_text);
	ui.listWidget_total->addItem(item);
	ui.listWidget_total->update();
	query.clear();
	query.prepare("SELECT seat_id FROM Seat_sales WHERE screening_id = :id");
	query.bindValue(":id", id);
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error - blocked seats");
	}
	while (query.next()) {
		QString seat_id = query.value(0).toString();
		QPushButton* button = findChild<QPushButton*>(seat_id.mid(1));
		button->setStyleSheet("border-image: url(:/ProjectionScreen/Resource files/seat2.png);");
		button->setEnabled(false);
	}

	
}

ProjectionScreen::~ProjectionScreen()
{
	delete snacksalespage;
}

/**
*@brief this function handles the button click event for seats. It changes the seat color and adds the seat to the list of selected seats. Updates the cart.
*/


void ProjectionScreen::handle_button_click() {
	QPushButton* button = qobject_cast<QPushButton*>(sender());
	if (button) {
		QString seat_id = hall_id + button->objectName();
		QSqlQuery query(loginDb);
		query.prepare("SELECT price FROM Seats WHERE seat_id = :id");
		query.bindValue(":id", seat_id);
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
			total_price -= price.toDouble();
			if(seats_ids.empty())
			total_price = 0.;
			QString price_text = "Total price: " + QString::number(total_price) + " zl";
			QListWidgetItem* item = new QListWidgetItem(price_text);
			
			ui.listWidget_total->clear();
			ui.listWidget_total->addItem(item);
			ui.listWidget_total->update();

		    
		}
		else {
			button->setStyleSheet("border-image: url(:/ProjectionScreen/Resource files/seat3.png);");
			seats_ids.push_back(hall_id+button->objectName());
			QListWidgetItem* item = new QListWidgetItem(ticket);
			ui.listWidget->addItem(item);
			total_price += price.toDouble();
			ui.listWidget->update();
			QString price_text = "Total price: " + QString::number(total_price) + " zl";
			QListWidgetItem* item2 = new QListWidgetItem(price_text);
			ui.listWidget_total->clear();
			ui.listWidget_total->addItem(item2);
			ui.listWidget_total->update();

		}
	}
}


/**
* @brief This function proceeds to snack sale page.
*/

void ProjectionScreen::on_pushButton_proceed_clicked() {
	if (seats_ids.empty()) {
		QMessageBox::StandardButton reply = QMessageBox::question(this, "Warning", "No seats selected. Do you want to proceed?", QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::No) {
			return;
		}
		else {
			snacksalespage = new SnackSales(nullptr, loginDb);
			connect(snacksalespage, &SnackSales::return_to_employee_page, this, &ProjectionScreen::return_to_employee_page);
			this->hide();
			snacksalespage->show();
		}
	}
	else {
		QMessageBox::StandardButton reply = QMessageBox::question(this, "Proceed", "Are you sure you want to proceed?", QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::No) {
			return;
		}
		else {
			snacksalespage = new SnackSales(nullptr, loginDb);
			snacksalespage->setTotalPrice(total_price);
			snacksalespage->setSeats(seats_ids);
			snacksalespage->setScreeningId(id);
			connect(snacksalespage, &SnackSales::return_to_employee_page, this, &ProjectionScreen::return_to_employee_page);
			QStringList items;
			for (int i = 0; i < ui.listWidget->count(); i++) {
				items << ui.listWidget->item(i)->text();
			}
			snacksalespage->addItemsToWidgetList(items);
			this->hide();
			snacksalespage->show();
		}
	}
}


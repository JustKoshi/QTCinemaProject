#include "SnackSales.h"

SnackSales::SnackSales(QWidget *parent, QSqlDatabase db)
	: QMainWindow(parent), loginDb(db)
{
	ui.setupUi(this);
	sale_id = 0;
	seats_ids = std::vector<QString>();
	total_price = 0;
	QListWidgetItem* item = new QListWidgetItem("Total price: " + QString::number(total_price) + " zl");
	ui.listWidget_total->addItem(item);
	QSqlQuery query(loginDb);
	query.prepare("SELECT snack_name, size, price FROM Snacks");
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
		qDebug() << query.lastError().text();
		return;
	}
	while (query.next()) {
		QString size = query.value(1).toString()+ " - " + query.value(2).toString()+ " zl";
		QString snack = query.value(0).toString();
		if (snack == "Popcorn") {
			ui.comboBox_popcorn->addItem(size);
		}
		else if (snack == "Soda") {
			ui.comboBox_soda->addItem(size);
		}
		else if (snack == "Nachos") {
			ui.comboBox_nachos->addItem(size);
		}
		else if (snack == "Slushy") {
			ui.comboBox_slushy->addItem(size);
		}
		else if (snack == "Candy") {
			ui.comboBox_candy->addItem(size);
		}
	}
	ui.comboBox_payment->addItem("Cash");
	ui.comboBox_payment->addItem("Card");
}

SnackSales::~SnackSales()
{}

void SnackSales::setSeats(std::vector<QString> seats)
{
	seats_ids = seats;
	
}

void SnackSales::addItemsToWidgetList(const QStringList& items) {
	for (const QString& item : items) {
		ui.listWidget->addItem(item);
	}
}

void SnackSales::setScreeningId(QString id)
{
	screening_id = id;
}

void SnackSales::setTotalPrice(double price)
{
	total_price = price;
	QListWidgetItem* item = new QListWidgetItem("Total price: " + QString::number(total_price) + " zl");
	ui.listWidget_total->clear();
	ui.listWidget_total->addItem(item);
	ui.listWidget_total->update();
}

void SnackSales::on_pushButton_add_candy_clicked() {
	QString size = ui.comboBox_candy->currentText();
	size = size.left(size.indexOf("-") - 1);
	QString item = "Candy - " + size;
	QSqlQuery query(loginDb);
	query.prepare("SELECT price, snack_id FROM Snacks WHERE snack_name = 'Candy' AND size = :size");
	query.bindValue(":size", size);
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
		qDebug() << query.lastError().text();
		return;
	}
	query.next();
	item += " - " + query.value(0).toString() + " zl";
	total_price += query.value(0).toDouble();
	bool found = false;
	for (std::pair<QString, int>& pair : snack_ids_and_amouts) {
		if (pair.first == query.value(1).toString()) {
			pair.second++;
			found = true;
			break;
		}
	}
	if (!found) {
		snack_ids_and_amouts.push_back(std::make_pair(query.value(1).toString(), 1));
	}
	QListWidgetItem* list_item = new QListWidgetItem(item);
	ui.listWidget->addItem(list_item);
	ui.listWidget->update();
	ui.listWidget_total->clear();
	QListWidgetItem* item_total = new QListWidgetItem("Total price: " + QString::number(total_price) + " zl");
	ui.listWidget_total->addItem(item_total);
	ui.listWidget_total->update();

}

void SnackSales::on_pushButton_add_nachos_clicked() {
	QString size = ui.comboBox_nachos->currentText();
	size = size.left(size.indexOf("-") - 1);
	QString item = "Nachos - " + size;
	QSqlQuery query(loginDb);
	query.prepare("SELECT price, snack_id FROM Snacks WHERE snack_name = 'Nachos' AND size = :size");
	query.bindValue(":size", size);
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
		qDebug() << query.lastError().text();
		return;
	}
	query.next();
	item += " - " + query.value(0).toString() + " zl";
	total_price += query.value(0).toDouble();
	bool found = false;
	for (std::pair<QString, int>& pair : snack_ids_and_amouts) {
		if (pair.first == query.value(1).toString()) {
			pair.second++;
			found = true;
			break;
		}
	}
	if (!found) {
		snack_ids_and_amouts.push_back(std::make_pair(query.value(1).toString(), 1));
	}
	QListWidgetItem* list_item = new QListWidgetItem(item);
	ui.listWidget->addItem(list_item);
	ui.listWidget->update();
	ui.listWidget_total->clear();
	QListWidgetItem* item_total = new QListWidgetItem("Total price: " + QString::number(total_price) + " zl");
	ui.listWidget_total->addItem(item_total);
	ui.listWidget_total->update();
}

void SnackSales::on_pushButton_add_popcorn_clicked() {
	QString size = ui.comboBox_popcorn->currentText();
	size = size.left(size.indexOf("-") - 1);
	QString item = "Popcorn - " + size;
	QSqlQuery query(loginDb);
	query.prepare("SELECT price, snack_id FROM Snacks WHERE snack_name = 'Popcorn' AND size = :size");
	query.bindValue(":size", size);
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
		qDebug() << query.lastError().text();
		return;
	}
	query.next();
	item += " - " + query.value(0).toString() + " zl";
	bool found = false;
	for (std::pair<QString, int>& pair : snack_ids_and_amouts) {
		if (pair.first == query.value(1).toString()) {
			pair.second++;
			found = true;
			break;
		}
	}
	if (!found) {
		snack_ids_and_amouts.push_back(std::make_pair(query.value(1).toString(), 1));
	}
	total_price += query.value(0).toDouble();
	QListWidgetItem* list_item = new QListWidgetItem(item);
	ui.listWidget->addItem(list_item);
	ui.listWidget->update();
	ui.listWidget_total->clear();
	QListWidgetItem* item_total = new QListWidgetItem("Total price: " + QString::number(total_price) + " zl");
	ui.listWidget_total->addItem(item_total);
	ui.listWidget_total->update();
}

void SnackSales::on_pushButton_add_slushy_clicked() {
	QString size = ui.comboBox_slushy->currentText();
	size = size.left(size.indexOf("-") - 1);
	QString item = "Slushy - " + size;
	QSqlQuery query(loginDb);
	query.prepare("SELECT price, snack_id FROM Snacks WHERE snack_name = 'Slushy' AND size = :size");
	query.bindValue(":size", size);
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
		qDebug() << query.lastError().text();
		return;
	}
	query.next();
	item += " - " + query.value(0).toString() + " zl";
	bool found = false;
	for (std::pair<QString, int>& pair : snack_ids_and_amouts) {
		if (pair.first == query.value(1).toString()) {
			pair.second++;
			found = true;
			break;
		}
	}
	if (!found) {
		snack_ids_and_amouts.push_back(std::make_pair(query.value(1).toString(), 1));
	}
	total_price += query.value(0).toDouble();
	QListWidgetItem* list_item = new QListWidgetItem(item);
	ui.listWidget->addItem(list_item);
	ui.listWidget->update();
	ui.listWidget_total->clear();
	QListWidgetItem* item_total = new QListWidgetItem("Total price: " + QString::number(total_price) + " zl");
	ui.listWidget_total->addItem(item_total);
	ui.listWidget_total->update();
}

void SnackSales::on_pushButton_add_soda_clicked() {
	QString size = ui.comboBox_soda->currentText();
	size = size.left(size.indexOf("-") - 1);
	QString item = "Soda - " + size;
	QSqlQuery query(loginDb);
	query.prepare("SELECT price, snack_id FROM Snacks WHERE snack_name = 'Soda' AND size = :size");
	query.bindValue(":size", size);
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
		qDebug() << query.lastError().text();
		return;
	}
	query.next();
	item += " - " + query.value(0).toString() + " zl";
	total_price += query.value(0).toDouble();
	bool found = false;
	for (std::pair<QString, int>& pair : snack_ids_and_amouts) {
		if (pair.first == query.value(1).toString()) {
			pair.second++;
			found = true;
			break;
		}
	}
	if (!found) {
		snack_ids_and_amouts.push_back(std::make_pair(query.value(1).toString(), 1));
	}
	QListWidgetItem* list_item = new QListWidgetItem(item);
	ui.listWidget->addItem(list_item);
	ui.listWidget->update();
	ui.listWidget_total->clear();
	QListWidgetItem* item_total = new QListWidgetItem("Total price: " + QString::number(total_price) + " zl");
	ui.listWidget_total->addItem(item_total);
	ui.listWidget_total->update();

}

void SnackSales::on_pushButton_remove_clicked() {

	QListWidgetItem* item = ui.listWidget->currentItem();
	if (item == nullptr) {
		return;
	}
	if (item->text().contains("Ticket for seat")) {
		QMessageBox::critical(this, "Error", "You can't remove a ticket for a seat");
		return;
	}
	QString item_text = item->text();
	QString snack_name = item_text.left(item_text.indexOf("-") - 1);
	QString size = item_text.section(" - ", 1, 1);
	QString price = item_text.mid(item_text.lastIndexOf("-") + 2, item_text.lastIndexOf("z") - item_text.lastIndexOf("-") - 3);
	qDebug()<< snack_name << " " << size << " " << price;
	total_price -= price.toDouble();
	QListWidgetItem* item_total = new QListWidgetItem("Total price: " + QString::number(total_price) + " zl");
	ui.listWidget_total->clear();
	ui.listWidget_total->addItem(item_total);
	ui.listWidget_total->update();
	delete item;
	ui.listWidget->update();
	QSqlQuery query(loginDb);
	query.prepare("SELECT snack_id FROM Snacks WHERE snack_name = :snack_name AND size = :size");
	query.bindValue(":snack_name", snack_name);
	query.bindValue(":size", size);
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
		qDebug() << query.lastError().text();
		return;
	}
	query.next();
	QString snack_id = query.value(0).toString();
	for (std::pair<QString, int>& pair : snack_ids_and_amouts) {
		if (pair.first == snack_id) {
			pair.second--;
			if (pair.second == 0) {
				snack_ids_and_amouts.erase(std::remove(snack_ids_and_amouts.begin(), snack_ids_and_amouts.end(), pair), snack_ids_and_amouts.end());
			}
			break;
		}
	}

}

void SnackSales::on_pushButton_check_out_clicked() {

	
	if ((seats_ids.empty())&&(snack_ids_and_amouts.empty())) {
		QMessageBox::StandardButton reply = QMessageBox::question(this, "Warning", "No seats or snacks selected. Do you want to return?", QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::No) {
			return;
		}
		else {
			emit return_to_employee_page();
			this->close();
		}
	}
	else {
		QMessageBox::StandardButton reply = QMessageBox::question(this, "Warning", "Do you want to confirm purchase", QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::No) {
			return;
		}
		confirm_sale();
		emit return_to_employee_page();
		this->close();
	}
}

void SnackSales::confirm_sale() {
	if (snack_ids_and_amouts.empty()&&seats_ids.empty()) {
		return;
	};
	if(seats_ids.empty()){
		QSqlQuery query(loginDb);
		query.prepare("INSERT INTO Sales (total_price, sale_date, payment_method) VALUES (:total_price, datetime('now'), :payment_method)");
		query.bindValue(":total_price", total_price);
		query.bindValue(":payment_method", ui.comboBox_payment->currentText());
		if (!query.exec()) {
			QMessageBox::critical(this, "Error", "Query error");
			qDebug() << query.lastError().text();
			qDebug() << "1";
			return;
		}
		query.prepare("SELECT MAX(sale_id) FROM Sales");
		if (!query.exec()) {
			QMessageBox::critical(this, "Error", "Query error");
			qDebug() << query.lastError().text();
			qDebug() << "2";
			return;
		}
		if (query.next()) {
			sale_id = query.value(0).toInt();
		}
		else {
			QMessageBox::critical(this, "Error", "Failed to retrieve sale_id");
			qDebug() << query.lastError().text();
			qDebug() << "3";
		};

		for (std::pair<QString, int>& pair : snack_ids_and_amouts) {
			query.prepare("INSERT INTO Snack_Sales (snack_id, quantity, sale_id) VALUES (:snack_id, :quantity, :sale_id)");
			query.bindValue(":snack_id", pair.first);
			query.bindValue(":quantity", pair.second);
			query.bindValue(":sale_id", sale_id);
			if (!query.exec()) {
				QMessageBox::critical(this, "Error", "Query error");
				qDebug() << query.lastError().text();
				return;
			}
		}
	}
	else if (snack_ids_and_amouts.empty()) {
		QSqlQuery query(loginDb);
		query.prepare("INSERT INTO Sales (total_price, sale_date, payment_method) VALUES (:total_price, datetime('now'), :payment_method)");
		query.bindValue(":total_price", total_price);
		query.bindValue(":payment_method", ui.comboBox_payment->currentText());
		if (!query.exec()) {
			QMessageBox::critical(this, "Error", "Query error");
			qDebug() << query.lastError().text();
			qDebug() << "1";
			return;
		}
		query.clear();
		query.prepare("SELECT MAX(sale_id) FROM Sales");
		if (!query.exec()) {
			QMessageBox::critical(this, "Error", "Query error");
			qDebug() << query.lastError().text();
			qDebug() << "2";
			return;
		}
		if (query.next()) {
			sale_id = query.value(0).toInt();
		}

		for (QString& seat_id : seats_ids) {
			query.prepare("INSERT INTO Seat_sales (screening_id, seat_id, sale_id) VALUES (:screening_id, :seat_id, :sale_id)");
			query.bindValue(":screening_id", screening_id);
			query.bindValue(":seat_id", seat_id);
			query.bindValue(":sale_id", sale_id);
			if (!query.exec()) {
				QMessageBox::critical(this, "Error", "Query error");
				qDebug() << query.lastError().text();
				qDebug()<< "3";
				return;
			}
		}
	}
	else {
		QSqlQuery query(loginDb);
		query.prepare("INSERT INTO Sales (total_price, sale_date, payment_method) VALUES (:total_price, datetime('now'), :payment_method)");
		query.bindValue(":total_price", total_price);
		query.bindValue(":payment_method", ui.comboBox_payment->currentText());
		if (!query.exec()) {
			QMessageBox::critical(this, "Error", "Query error");
			qDebug() << query.lastError().text();
			qDebug() << "1";
			return;
		}
		query.clear();
		query.prepare("SELECT MAX(sale_id) FROM Sales");
		if (!query.exec()) {
			QMessageBox::critical(this, "Error", "Query error");
			qDebug() << query.lastError().text();
			qDebug() << "2";
			return;
		}
		if (query.next()) {
			sale_id = query.value(0).toInt();
		}

		for (QString& seat_id : seats_ids) {
			query.prepare("INSERT INTO Seat_sales (screening_id, seat_id, sale_id) VALUES (:screening_id, :seat_id, :sale_id)");
			query.bindValue(":screening_id", screening_id);
			query.bindValue(":seat_id", seat_id);
			query.bindValue(":sale_id", sale_id);
			if (!query.exec()) {
				QMessageBox::critical(this, "Error", "Query error");
				qDebug() << query.lastError().text();
				qDebug() << "3";
				return;
			}
		}

		for (std::pair<QString, int>& pair : snack_ids_and_amouts) {
			query.prepare("INSERT INTO Snack_Sales (snack_id, quantity, sale_id) VALUES (:snack_id, :quantity, :sale_id)");
			query.bindValue(":snack_id", pair.first);
			query.bindValue(":quantity", pair.second);
			query.bindValue(":sale_id", sale_id);
			if (!query.exec()) {
				QMessageBox::critical(this, "Error", "Query error");
				qDebug() << query.lastError().text();
				return;
			}
		}
	}	
	
}
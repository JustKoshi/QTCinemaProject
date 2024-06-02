#include "EmployeeManager.h"

EmployeeManager::EmployeeManager(QWidget *parent, QSqlDatabase db)
	: QWidget(parent), logindb(db)
{
	ui.setupUi(this);
	ui.comboBox_shift_time->addItem("4 hours");
	ui.comboBox_shift_time->addItem("6 hours");
	ui.comboBox_shift_time->addItem("8 hours");
	ui.comboBox_shift_time->addItem("10 hours");
	ui.comboBox_shift_time->addItem("12 hours");
	ui.comboBox_position->addItem("manager");
	ui.comboBox_position->addItem("employee");
	set_table_view();

}

EmployeeManager::~EmployeeManager()
{}

void EmployeeManager::on_pushButton_return_clicked() {
	this->close();
	emit return_to_admin_page();
}

void EmployeeManager::on_pushButton_add_employee_clicked() {
	QString name = ui.lineEdit_name->text();
	QString surname = ui.lineEdit_surname->text();
	QString position = ui.comboBox_position->currentText();
	QString shift_time = ui.comboBox_shift_time->currentText();
	QSqlQuery query(logindb);
	query.prepare("INSERT INTO EmployeeData (name, surname, position, shift_time) VALUES (:name, :surname, :position, :shift_time)");
	query.bindValue(":name", name);
	query.bindValue(":surname", surname);
	query.bindValue(":position", position);
	int shift_time_int = shift_time.split(" ")[0].toInt();
	query.bindValue(":shift_time", shift_time_int);
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
	}
	query.clear();
	query.prepare("SELECT employee_id FROM EmployeeData WHERE name = :name AND surname = :surname");
	query.bindValue(":name", name);
	query.bindValue(":surname", surname);
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
	}
	query.next();
	int employee_id = query.value(0).toInt();
	int acess_level = 0;
	if (ui.comboBox_position->currentText()== "manager") {
		acess_level = 2;
	}
	else {
		acess_level = 1;
	}
	query.prepare("INSERT INTO LoginData (employee_id, login, password, access_level) VALUES (:employee_id, :login, :password, :acess_level)");
	query.bindValue(":employee_id", employee_id);
	query.bindValue(":login", name + surname);
	query.bindValue(":password", "password");
	query.bindValue(":acess_level", acess_level);
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
	}
	set_table_view();
	QMessageBox::information(this, "Success", "Employee added");

}

void EmployeeManager::set_table_view() {
	QSqlQuery query(logindb);
	QSqlQueryModel* model = new QSqlQueryModel();
	query.prepare("SELECT EmployeeData.*, LoginData.login FROM EmployeeData JOIN LoginData ON EmployeeData.employee_id = LoginData.employee_id");
	query.exec();
	model->setQuery(query);
	ui.tableView->setModel(model);
	ui.tableView->resizeColumnsToContents();
	ui.tableView->show();
}

void EmployeeManager::on_pushButton_delete_employee_clicked() {

	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, "Delete", "Are you sure you want to delete this employee?",
				QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::No) {
		return;
	}
	QSqlQuery query(logindb);
	QModelIndexList selectedList = ui.tableView->selectionModel()->selectedRows();
	if (selectedList.size() == 0) {
		QMessageBox::information(this, "Error", "No reservation selected.");
		return;
	}
	QModelIndex index = selectedList.at(0);
	QString employee_id = index.sibling(index.row(), 0).data().toString();
	query.prepare("DELETE FROM EmployeeData WHERE employee_id = :employee_id");
	query.bindValue(":employee_id", employee_id);
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
		qDebug() << "1";
	}
	query.clear();
	query.prepare("DELETE FROM LoginData WHERE employee_id = :employee_id");
	query.bindValue(":employee_id", employee_id);
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
		qDebug() << "2";
	}
	QMessageBox::information(this, "Success", "Employee removed.");

	set_table_view();
}
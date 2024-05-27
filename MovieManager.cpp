#include "MovieManager.h"

MovieManager::MovieManager(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.label_img->setAlignment(Qt::AlignCenter);
	connect(ui.pushButton_return, SIGNAL(clicked()), this, SLOT(on_pushButton_return_clicked()));
	
}

MovieManager::~MovieManager()
{}

void MovieManager::GetDb(QSqlDatabase db) {
	loginDb = db;
	QSqlQueryModel* model = new QSqlQueryModel();
	QSqlQuery query(loginDb);
	query.prepare("SELECT movie_id, title, length, director FROM Movies");
	if (!query.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
	}
	model->setQuery(query);
	ui.movie_table->setModel(model);
	ui.movie_table->show();


}

void MovieManager::on_pushButton_load_img_clicked() {
	QString filename = QFileDialog::getOpenFileName(this, "Open Image", "C:/", "Image Files (*.png *.jpg *.bmp *.jpeg)");
	image.load(filename);
	ui.label_img->setPixmap(QPixmap::fromImage(image));
	ui.label_img->show();
}

void MovieManager::on_pushButton_return_clicked() {
	this->close();
	emit return_To_AdminPage();

}

void  MovieManager::on_pushButton_load_data_clicked() {
	QSqlQuery query(loginDb);
	int id = ui.spinBox_movie_id->value();
	query.prepare("SELECT title, length, director, genre FROM Movies WHERE movie_id = :id");
	query.bindValue(":id", id);
	if (query.exec()) {

		if (query.next()) {
			QString title = query.value(0).toString();
			int length = query.value(1).toInt();
			QString director = query.value(2).toString();
			QString genre = query.value(3).toString();
			ui.lineEdit_title->setText(title);
			ui.lineEdit_director->setText(director);
			ui.lineEdit_genre->setText(genre);
			ui.lineEdit_length->setText(QString::number(length));	
		}
		else {
			QMessageBox::critical(this, "Error", "Movie not found");
		}
	}
	else {
		QMessageBox::critical(this, "Error", "Data not loaded");
	}
	query.clear();
}

void MovieManager::on_pushButton_add_movie_clicked() {
	if (image.isNull()||ui.lineEdit_director->text().isEmpty()||ui.lineEdit_genre->text().isEmpty()||ui.lineEdit_length->text().isEmpty()||ui.lineEdit_title->text().isEmpty()) {
		QMessageBox::critical(this, "Error", "Fill all fields before insertion");
		return;
	}
	else {
		QSqlQuery query(loginDb);
		QString title = ui.lineEdit_title->text();
		QString director = ui.lineEdit_director->text();
		QString genre = ui.lineEdit_genre->text();
		int length = ui.lineEdit_length->text().toInt();
		QByteArray bytes;
		QBuffer buffer(&bytes);
		buffer.open(QIODevice::WriteOnly);
		image.save(&buffer, "PNG");
		
		query.prepare("INSERT INTO Movies (title, length, director, genre, poster) VALUES (:title, :length, :director, :genre, :image)");
		query.bindValue(":title", title);
		query.bindValue(":length", length);
		query.bindValue(":director", director);
		query.bindValue(":genre", genre);
		query.bindValue(":image", bytes);

		if (query.exec()) {
			QMessageBox::information(this, "Success", "Movie added");
		}
		else {
			//write to console error message
			qDebug() << query.lastError().text();
			QMessageBox::critical(this, "Error", "Movie not added");
		}
		query.clear();
	}
	QSqlQueryModel* model = new QSqlQueryModel();
	QSqlQuery query2(loginDb);
	query2.prepare("SELECT movie_id, title, length, director FROM Movies");
	if (!query2.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
	}
	model->setQuery(query2);
	ui.movie_table->setModel(model);
	ui.movie_table->show();
}	

void MovieManager::on_pushButton_update_movie_clicked() {
	if (image.isNull() || ui.lineEdit_director->text().isEmpty() || ui.lineEdit_genre->text().isEmpty() || ui.lineEdit_length->text().isEmpty() || ui.lineEdit_title->text().isEmpty()) {
		QMessageBox::critical(this, "Error", "Fill all fields before updating");
		return;
	}
	else {
		QSqlQuery query(loginDb);
		QString title = ui.lineEdit_title->text();
		QString director = ui.lineEdit_director->text();
		QString genre = ui.lineEdit_genre->text();
		int length = ui.lineEdit_length->text().toInt();
		QByteArray bytes;
		QBuffer buffer(&bytes);
		buffer.open(QIODevice::WriteOnly);
		image.save(&buffer, "PNG");
		query.prepare("UPDATE Movies SET title = :title, length = :length, director = :director, genre = :genre, poster = :image WHERE movie_id = :id");
		query.bindValue(":title", title);
		query.bindValue(":length", length);
		query.bindValue(":director", director);
		query.bindValue(":genre", genre);
		query.bindValue(":image", bytes);
		query.bindValue(":id", ui.spinBox_movie_id->value());
		if (query.exec()) {
			QMessageBox::information(this, "Success", "Movie updated");
		}
		else {
			qDebug() << query.lastError().text();
			QMessageBox::critical(this, "Error", "Movie not updated");
		}
		query.clear();
	}
	QSqlQueryModel* model = new QSqlQueryModel();
	QSqlQuery query2(loginDb);
	query2.prepare("SELECT movie_id, title, length, director FROM Movies");
	if (!query2.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
	}
	model->setQuery(query2);
	ui.movie_table->setModel(model);
	ui.movie_table->show();
}

void MovieManager::on_pushButton_delete_movie_clicked() {
		QSqlQuery query(loginDb);
	query.prepare("DELETE FROM Movies WHERE movie_id = :id");
	query.bindValue(":id", ui.spinBox_movie_id->value());

	QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete", "Are you sure you want to delete this movie with id: " + ui.spinBox_movie_id->value(), QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::No) {
		return;
	}
	else {
		if (query.exec()) {
			QMessageBox::information(this, "Success", "Movie deleted");
		}
		else {
			QMessageBox::critical(this, "Error", "Movie not found");
		}
		query.clear();
	}
	QSqlQueryModel* model = new QSqlQueryModel();
	QSqlQuery query2(loginDb);
	query2.prepare("SELECT movie_id, title, length, director FROM Movies");
	if (!query2.exec()) {
		QMessageBox::critical(this, "Error", "Query error");
	}
	model->setQuery(query2);
	ui.movie_table->setModel(model);
	ui.movie_table->show();
}
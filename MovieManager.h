#pragma once

#include <QMainWindow>
#include "ui_MovieManager.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QFileDialog>
#include <QImage>
#include <QMessageBox>

class MovieManager : public QMainWindow
{
	Q_OBJECT

public:
	MovieManager(QWidget *parent = nullptr);
	~MovieManager();
	void GetDb(QSqlDatabase db);
public slots:
	void on_pushButton_load_img_clicked();
	void on_pushButton_return_clicked();
	void on_pushButton_load_data_clicked();
	void on_pushButton_add_movie_clicked();
	void on_pushButton_delete_movie_clicked();
	void on_pushButton_update_movie_clicked();
signals:
	void return_To_AdminPage();
private:
	QSqlDatabase loginDb;
	Ui::MovieManagerClass ui;
	QImage image;
};

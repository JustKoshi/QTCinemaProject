#pragma once

#include <QMainWindow>
#include "ui_AdminPage.h"
#include <QtSql>
#include <QtGui>
#include <QtCore>
#include <QtWidgets>


class AdminPage : public QMainWindow
{
	Q_OBJECT

public:
	AdminPage(QWidget *parent = nullptr);
	~AdminPage();

private:
	Ui::AdminPageClass ui;
};

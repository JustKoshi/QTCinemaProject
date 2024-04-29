#pragma once

#include <QMainWindow>
#include "ui_AdminPage.h"

class AdminPage : public QMainWindow
{
	Q_OBJECT

public:
	AdminPage(QWidget *parent = nullptr);
	~AdminPage();

private:
	Ui::AdminPageClass ui;
};

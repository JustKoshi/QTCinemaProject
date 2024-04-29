#pragma once

#include <QMainWindow>
#include "ui_CheckOutScreen.h"

class CheckOutScreen : public QMainWindow
{
	Q_OBJECT

public:
	CheckOutScreen(QWidget *parent = nullptr);
	~CheckOutScreen();

private:
	Ui::CheckOutScreenClass ui;
};

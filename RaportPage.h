#pragma once

#include <QMainWindow>
#include "ui_RaportPage.h"

class RaportPage : public QMainWindow
{
	Q_OBJECT

public:
	RaportPage(QWidget *parent = nullptr);
	~RaportPage();

private:
	Ui::RaportPageClass ui;
};

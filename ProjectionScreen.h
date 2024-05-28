#pragma once

#include <QWidget>
#include "ui_ProjectionScreen.h"

class ProjectionScreen : public QWidget
{
	Q_OBJECT

public:
	ProjectionScreen(QWidget *parent = nullptr);
	~ProjectionScreen();

private:
	Ui::ProjectionScreenClass ui;
};

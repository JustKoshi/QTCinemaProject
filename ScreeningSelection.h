#pragma once

#include <QMainWindow>
#include "ui_ScreeningSelection.h"
#include <QVBoxLayout>
#include <QPixmap>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QHBoxLayout>
#include "ProjectionScreen.h"
#include <memory>

class ScreeningSelection : public QMainWindow
{
	Q_OBJECT

public:
	ScreeningSelection(QWidget *parent = nullptr);
	~ScreeningSelection();
	void getDb(QSqlDatabase db);
private:
	Ui::ScreeningSelectionClass ui;
	QSqlDatabase loginDb;
	QVBoxLayout* mainlayout;
	QVBoxLayout* scrolllayout;
	QScrollArea *scrollArea;
	QWidget *scrollAreaWidgetContents;
	void createPage(const QString& title, const QPixmap& image);
private slots:
	void onButtonClicked(QString id);
};

#include "ReportPage.h"



ReportPage::ReportPage(QWidget *parent, QSqlDatabase db)
	: QMainWindow(parent), db(db)
{
	ui.setupUi(this);
	ui.comboBox->addItem("Total");
	ui.comboBox->addItem("This Year");
	ui.comboBox->addItem("This Month");
	ui.comboBox->addItem("This Week");
	ui.comboBox->addItem("Today");
	ui.comboBox_chart->addItem("Piechart");
	ui.comboBox_chart->addItem("Barchart");
}

ReportPage::~ReportPage()
{}

void ReportPage::on_pushButton_return_clicked() {
	this->close();
	emit return_To_AdminPage();
}

void ReportPage::on_pushButton_seat_sales_clicked() {
	
	if (ui.groupBox_chart->layout() != nullptr) {
		QLayoutItem* child;
		while ((child = ui.groupBox_chart->layout()->takeAt(0)) != 0) {
			delete child->widget();
			delete child;
		}
	}
	ui.groupBox_chart->setTitle("Seat Sales");
	ui.groupBox_chart->setGeometry(10, 10, 800, 600);
	ui.groupBox_chart->setStyleSheet("background-color: white;");
	ui.groupBox_chart->setAutoFillBackground(true);
	ui.groupBox_chart->show();
	QChart *chart = new QChart();
	chart->setTitle("Seat Sales");
	chart->setAnimationOptions(QChart::SeriesAnimations);
	QSqlQuery query(db);
	if (ui.comboBox->currentText() == "This Year") {
		if (!query.exec("SELECT title, COUNT(*) \
        FROM Movies \
        JOIN Screenings ON Movies.movie_id = Screenings.movie_id \
        JOIN Seat_sales ON Screenings.screening_id = Seat_sales.screening_id \
        WHERE strftime('%Y', Screenings.date_start) = strftime('%Y', date('now')) \
        GROUP BY title")) {
			qDebug() << query.lastError();
		}
	}
	else if (ui.comboBox->currentText() == "This Month") {
		if (!query.exec("SELECT title, COUNT(*) \
        FROM Movies \
        JOIN Screenings ON Movies.movie_id = Screenings.movie_id \
        JOIN Seat_sales ON Screenings.screening_id = Seat_sales.screening_id \
        WHERE strftime('%Y-%m', Screenings.date_start) = strftime('%Y-%m', date('now')) \
        GROUP BY title")) {
			qDebug() << query.lastError();
		}
	}
	else if (ui.comboBox->currentText() == "This Week") {
		if (!query.exec("SELECT title, COUNT(*) \
        FROM Movies \
        JOIN Screenings ON Movies.movie_id = Screenings.movie_id \
        JOIN Seat_sales ON Screenings.screening_id = Seat_sales.screening_id \
        WHERE strftime('%Y-%W', Screenings.date_start) = strftime('%Y-%W', date('now')) \
        GROUP BY title")) {
			qDebug() << query.lastError();
		}
	}
	else if (ui.comboBox->currentText() == "Today") {
		if (!query.exec("SELECT title, COUNT(*) \
        FROM Movies \
        JOIN Screenings ON Movies.movie_id = Screenings.movie_id \
        JOIN Seat_sales ON Screenings.screening_id = Seat_sales.screening_id \
        WHERE date(Screenings.date_start) = date('now') \
        GROUP BY title")) {
			qDebug() << query.lastError();
		}
	}
	else {
		if (!query.exec("SELECT title, COUNT(*) \
		FROM Movies \
		JOIN Screenings ON Movies.movie_id = Screenings.movie_id \
		JOIN Seat_sales ON Screenings.screening_id = Seat_sales.screening_id \
		GROUP BY title")) {
			qDebug() << query.lastError();
		}
	}
	QPieSeries* series = new QPieSeries();
	QBarSeries* barSeries = new QBarSeries();
	if (ui.comboBox_chart->currentText() == "Piechart") {
		while (query.next()) {
			QPieSlice* slice = series->append(query.value(0).toString(), query.value(1).toInt());
			slice->setColor(QColor(rand() % 256, rand() % 256, rand() % 256));
			slice->setLabel(QString("%1: %2").arg(slice->label()).arg(slice->value()));

		}
		chart->addSeries(series);
	}
	else {
		while (query.next()) {
			QBarSet* set = new QBarSet(query.value(0).toString());
			set->setColor(QColor(rand() % 256, rand() % 256, rand() % 256));
			*set << query.value(1).toInt();
			set->setLabel(QString("%1: %2").arg(set->label()).arg(set->at(0)));
			QBarSet* spacer = new QBarSet("");
			barSeries->append(spacer);
			barSeries->append(set);



		}
		QStringList categories;
		categories << "";
		chart->addSeries(barSeries);
		QBarCategoryAxis* axis = new QBarCategoryAxis();
		axis->setGridLineVisible(false);
		axis->append(categories);
		chart->createDefaultAxes(); 
		chart->setAxisX(axis, barSeries); 

		foreach(QLegendMarker * marker, chart->legend()->markers()) {
			if (marker->label().isEmpty()) {
				marker->setVisible(false);
			}
		}
	}

	QChartView *chartView = new QChartView(chart);
	chartView->setRenderHint(QPainter::Antialiasing);
	chartView->setParent(ui.groupBox_chart);
	chartView->setGeometry(10, 10, 780, 580);
	ui.groupBox_chart->layout()->addWidget(chartView);
	chartView->show();
}

void ReportPage::on_pushButton_snack_sales_clicked() {

	if (ui.groupBox_chart->layout() != nullptr) {
		QLayoutItem *child;
		while ((child = ui.groupBox_chart->layout()->takeAt(0)) != 0) {
			delete child->widget();
			delete child;
		}
	}

	ui.groupBox_chart->setTitle("Snack Sales");
	ui.groupBox_chart->setGeometry(10, 10, 800, 600);
	ui.groupBox_chart->setStyleSheet("background-color: white;");
	ui.groupBox_chart->setAutoFillBackground(true);
	ui.groupBox_chart->show();
	QChart *chart = new QChart();
	chart->setTitle("Snack Sales");
	chart->setAnimationOptions(QChart::SeriesAnimations);
	QSqlQuery query(db);
	if (ui.comboBox->currentText() == "This Year") {
		if (!query.exec("SELECT snack_name, COUNT(*) \
        FROM Snacks \
        JOIN Snack_sales ON Snacks.snack_id = Snack_sales.snack_id \
        JOIN Sales ON Snack_sales.sale_id = Sales.sale_id \
        WHERE strftime('%Y', Sales.sale_date) = strftime('%Y', date('now')) \
        GROUP BY snack_name")) {
			qDebug() << query.lastError();
		}
	}
	else if (ui.comboBox->currentText() == "This Month") {
		if (!query.exec("SELECT snack_name, COUNT(*) \
        FROM Snacks \
        JOIN Snack_sales ON Snacks.snack_id = Snack_sales.snack_id \
        JOIN Sales ON Snack_sales.sale_id = Sales.sale_id \
        WHERE strftime('%Y-%m', Sales.sale_date) = strftime('%Y-%m', date('now')) \
        GROUP BY snack_name")) {
			qDebug() << query.lastError();
		}
	}
	else if (ui.comboBox->currentText() == "This Week") {
		if (!query.exec("SELECT snack_name, COUNT(*) \
        FROM Snacks \
        JOIN Snack_sales ON Snacks.snack_id = Snack_sales.snack_id \
        JOIN Sales ON Snack_sales.sale_id = Sales.sale_id \
        WHERE strftime('%Y-%W', Sales.sale_date) = strftime('%Y-%W', date('now')) \
        GROUP BY snack_name")) {
			qDebug() << query.lastError();
		}
	}
	else if (ui.comboBox->currentText() == "Today") {
		if (!query.exec("SELECT snack_name, COUNT(*) \
        FROM Snacks \
        JOIN Snack_sales ON Snacks.snack_id = Snack_sales.snack_id \
        JOIN Sales ON Snack_sales.sale_id = Sales.sale_id \
        WHERE date(Sales.sale_date) = date('now') \
        GROUP BY snack_name")) {
			qDebug() << query.lastError();
		}
	}
	else {
		if (!query.exec("SELECT snack_name, COUNT(*) \
        FROM Snacks \
        JOIN Snack_sales ON Snacks.snack_id = Snack_sales.snack_id \
        JOIN Sales ON Snack_sales.sale_id = Sales.sale_id \
        GROUP BY snack_name")) {
			qDebug() << query.lastError();
		}
	}

	QPieSeries* series = new QPieSeries();
	QBarSeries* barSeries = new QBarSeries();
	if (ui.comboBox_chart->currentText() == "Piechart") {
		while (query.next()) {
			QPieSlice* slice = series->append(query.value(0).toString(), query.value(1).toInt());
			slice->setColor(QColor(rand() % 256, rand() % 256, rand() % 256));
			slice->setLabel(QString("%1: %2").arg(slice->label()).arg(slice->value()));

		}
		chart->addSeries(series);
	}
	else {
		while (query.next()) {
			QBarSet* set = new QBarSet(query.value(0).toString());
			set->setColor(QColor(rand() % 256, rand() % 256, rand() % 256));
			*set << query.value(1).toInt();
			set->setLabel(QString("%1: %2").arg(set->label()).arg(set->at(0)));
			QBarSet* spacer = new QBarSet("");
			barSeries->append(spacer);
			barSeries->append(set);



		}
		QStringList categories;
		categories << "";
		chart->addSeries(barSeries);
		QBarCategoryAxis* axis = new QBarCategoryAxis();
		axis->setGridLineVisible(false);
		axis->append(categories);
		chart->createDefaultAxes();
		chart->setAxisX(axis, barSeries);

		foreach(QLegendMarker * marker, chart->legend()->markers()) {
			if (marker->label().isEmpty()) {
				marker->setVisible(false);
			}
		}
	}
	QChartView *chartView = new QChartView(chart);
	chartView->setRenderHint(QPainter::Antialiasing);
	chartView->setParent(ui.groupBox_chart);
	chartView->setGeometry(10, 10, 780, 580);
	ui.groupBox_chart->layout()->addWidget(chartView);
	chartView->show();
}
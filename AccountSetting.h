#pragma once
#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>

void changePassword(QSqlDatabase db, int id);
void changeLogin(QSqlDatabase db, int id);
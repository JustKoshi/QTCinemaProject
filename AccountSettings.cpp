#include "AccountSetting.h"

/**
 * @file AccountSetting.cpp
 * @brief This file contains the implementation of the functions to change the password and login of the user.
 */

/**
 * @brief This function changes the password of the user.
 * * @param db The database object.
 * * @param id The id of the user.
 * 
 * * This function creates a dialog box with fields for the user to enter the login, old password, new password and confirm new password. It then checks if the old password is correct and if the new password and confirm new password match. If all the conditions are met, the password is changed in the database.
 * */
void changePassword(QSqlDatabase db, int id) {
    QScopedPointer<QDialog> dialog(new QDialog);
    QFormLayout layout(dialog.get());
    dialog->setMinimumSize(200, 100);

    QLineEdit loginEdit(dialog.get());
    loginEdit.setPlaceholderText("Enter login");
    layout.addRow("Login", &loginEdit);

    QLineEdit oldPassword(dialog.get());
    oldPassword.setPlaceholderText("Enter old password");
    oldPassword.setEchoMode(QLineEdit::Password);
    layout.addRow("Old Password", &oldPassword);

    QLineEdit newPassword(dialog.get());
    newPassword.setPlaceholderText("Enter new password");
    newPassword.setEchoMode(QLineEdit::Password);
    layout.addRow("New Password", &newPassword);

    QLineEdit confirmNewPassword(dialog.get());
    confirmNewPassword.setPlaceholderText("Confirm new password");
    confirmNewPassword.setEchoMode(QLineEdit::Password);
    layout.addRow("Confirm New Password", &confirmNewPassword);

    QPushButton confirmButton("Confirm", dialog.get());
    layout.addRow(&confirmButton);

    QDialog* dialogPtr = dialog.get();
    QObject::connect(&confirmButton, &QPushButton::clicked, [dialogPtr, &loginEdit, &oldPassword, &newPassword, &confirmNewPassword, db, id]() {
        if (loginEdit.text().isEmpty() || oldPassword.text().isEmpty() || newPassword.text().isEmpty() || confirmNewPassword.text().isEmpty()) {
			QMessageBox::critical(dialogPtr, "Error", "All fields must be filled.");
			return;
		}
        if (newPassword.text() == oldPassword.text()) {
			QMessageBox::critical(dialogPtr, "Error", "New password cannot be the same as the old password.");
			return;
		}
        QSqlQuery query(db);
        query.prepare("SELECT COUNT(*) FROM LoginData WHERE employee_id = :employee_id AND login = :login AND password = :password");
        query.bindValue(":employee_id", id);
        query.bindValue(":login", loginEdit.text());
        query.bindValue(":password", oldPassword.text());
        if (!query.exec()) {
            QMessageBox::critical(dialogPtr, "Error", "Query error");
        }
        if (query.next() && query.value(0).toInt() > 0) {
            if (newPassword.text() == confirmNewPassword.text()) {
                QSqlQuery updateQuery(db);
                updateQuery.prepare("UPDATE LoginData SET password = :password WHERE employee_id = :employee_id AND login = :login");
                updateQuery.bindValue(":password", newPassword.text());
                updateQuery.bindValue(":employee_id", id);
                updateQuery.bindValue(":login", loginEdit.text());
                if (!updateQuery.exec()) {
                    QMessageBox::critical(dialogPtr, "Error", "Query error");
                }
                QMessageBox::information(dialogPtr, "Success", "Password changed successfully.");
                dialogPtr->accept();
            }
            else {
                QMessageBox::critical(dialogPtr, "Error", "New passwords do not match.");
            }
        }
        else {
            QMessageBox::critical(dialogPtr, "Error", "Old password or login is incorrect.");
        }
        });
    dialog->exec();
}

/**
 * @brief This function changes the login of the user.
 * * @param db The database object.
 * * @param id The id of the user.
 * 
 * * This function creates a dialog box with fields for the user to enter the old login, old password and new login. It then checks if the old login and password are correct. If the conditions are met, the login is changed in the database.
 * */

void changeLogin(QSqlDatabase db, int id) {
  
    QScopedPointer<QDialog> dialog(new QDialog);
    QFormLayout layout(dialog.get());
    dialog->setMinimumSize(200, 100);

    QLineEdit oldLogin(dialog.get());
    oldLogin.setPlaceholderText("Enter old login");
    layout.addRow("Old Login", &oldLogin);

    QLineEdit oldPassword(dialog.get());
    oldPassword.setPlaceholderText("Enter password");
    oldPassword.setEchoMode(QLineEdit::Password);
    layout.addRow("Password", &oldPassword);

    QLineEdit newLogin(dialog.get());
    newLogin.setPlaceholderText("Enter new login");
    layout.addRow("New Login", &newLogin);

    QPushButton confirmButton("Confirm", dialog.get());
    layout.addRow(&confirmButton);

    QDialog* dialogPtr = dialog.get();
    QObject::connect(&confirmButton, &QPushButton::clicked, [dialogPtr, &oldLogin, &oldPassword, &newLogin, db, id]() {
        if (oldLogin.text().isEmpty() || oldPassword.text().isEmpty() || newLogin.text().isEmpty()) {

            QMessageBox::critical(dialogPtr, "Error", "All fields must be filled.");
			return;


        }
        if (oldLogin.text() == newLogin.text()) {
			QMessageBox::critical(dialogPtr, "Error", "New login cannot be the same as the old login.");
        }
        QSqlQuery query(db);
        query.prepare("SELECT COUNT(*) FROM LoginData WHERE employee_id = :employee_id AND login = :login AND password = :password");
        query.bindValue(":employee_id", id);
        query.bindValue(":login", oldLogin.text());
        query.bindValue(":password", oldPassword.text());
        if (!query.exec()) {
			QMessageBox::critical(dialogPtr, "Error", "Query error");

        }
        if (query.next() && query.value(0).toInt() > 0) {
			QSqlQuery updateQuery(db);
			updateQuery.prepare("UPDATE LoginData SET login = :login WHERE employee_id = :employee_id");
			updateQuery.bindValue(":login", newLogin.text());
			updateQuery.bindValue(":employee_id", id);
			if (!updateQuery.exec()) {
				QMessageBox::critical(dialogPtr, "Error", "Query error");
			}
			QMessageBox::information(dialogPtr, "Success", "Login changed successfully.");
			dialogPtr->accept();
		}
		else {
			QMessageBox::critical(dialogPtr, "Error", "Old login or password is incorrect.");
		}
	});
        dialog->exec();

}
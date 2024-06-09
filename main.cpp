#include "LoginScreen.h"
#include <QtWidgets/QApplication>
#include "ProjectionScreen.h"

/**
 * @file main.cpp
 * @brief This file contains the main function to run the program.
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginScreen w;
    ProjectionScreen p;
    w.show();
    return a.exec();
}

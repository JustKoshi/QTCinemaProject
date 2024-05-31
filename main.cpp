#include "LoginScreen.h"
#include <QtWidgets/QApplication>
#include "ProjectionScreen.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginScreen w;
    ProjectionScreen p;
    w.show();
    return a.exec();
}

#include "mainwindow.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("mainDb.db");
    if(!db.open()){
        qDebug()<<"Error opening Database in main.cpp!";
    }
    else{
        MainWindow *w = new MainWindow();
        w->show();
    }

    return a.exec();
}

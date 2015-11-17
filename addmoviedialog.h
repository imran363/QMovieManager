#ifndef ADDMOVIEDIALOG_H
#define ADDMOVIEDIALOG_H

#include <QDialog>
#include "filedownloader.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>


namespace Ui {
class AddMovieDialog;
}

class AddMovieDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddMovieDialog(QWidget *parent = 0);
    ~AddMovieDialog();
    void setDataOffline(QString jsonFilePath, QString posterPath);
    void setData();
    void saveToDatabase();

private slots:
    void on_browseButton_clicked();
    void on_searchButton_clicked();
    void setDataOnline();
    void setPosterOnline();

    void on_saveButton_clicked();

    void on_backButton_clicked();

private:
    Ui::AddMovieDialog *ui;
    FileDownloader *downloader, *posterDownloader;
    QString file;
    QJsonDocument jdoc;
    QJsonObject json;
    QString info;
    QString poster;

};

#endif // ADDMOVIEDIALOG_H

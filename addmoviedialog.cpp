#include "addmoviedialog.h"
#include "ui_addmoviedialog.h"
#include "filedownloader.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QDir>

AddMovieDialog::AddMovieDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddMovieDialog)
{
    ui->setupUi(this);
}

AddMovieDialog::~AddMovieDialog()
{
    delete ui;
}

void AddMovieDialog::on_browseButton_clicked()
{
    file = QFileDialog::getOpenFileName(this, "Open Movie","",("Movies (*.mkv *.mp4 *.avi *.wmv)"));
    ui->pathLineEdit->setText(file);
    QString year="";
    QString name="";
    QFileInfo fileInfo(file);


    QRegExp rxYear("(19|20)(\\d{2})");
    int pos = rxYear.indexIn(file);
    if(pos > -1)
        year = rxYear.cap();
    else
        year = "2025";

    name = fileInfo.baseName().replace(year, "");
    name = name.remove("(");
    name = name.remove(")");
    name = name.trimmed();

    QString path = fileInfo.absolutePath();
    info = path+"/info.json";
    poster = path+"/poster.jpg";
    QFile file;
    file.setFileName(info);
    if(file.exists()){
        //ui->searchButton->setDisabled(true);
        setDataOffline(info, poster);
    }

    ui->spinBox->setValue(year.toInt());
    ui->movieNameLineEdit->setText(name);
}

void AddMovieDialog::on_searchButton_clicked()
{
    QString name = ui->movieNameLineEdit->text();
    int y = ui->spinBox->value();
    QString year="";
    if(y<2025){
        year = QString::number(y);
    }

    QUrl url("http://www.omdbapi.com/?t="+name+"&y="+year+"&plot=short&r=json");
    downloader = new FileDownloader(url, this);
    connect(downloader, SIGNAL(downloaded()), this, SLOT(setDataOnline()));
}

void AddMovieDialog::setDataOnline(){
    QByteArray bytes= downloader->downloadedData();
    jdoc = QJsonDocument::fromJson(bytes);
    json = jdoc.object();
    setData();
    posterDownloader = new FileDownloader(json["Poster"].toString(), this);
    connect(posterDownloader, SIGNAL(downloaded()), this, SLOT(setPosterOnline()));
}

void AddMovieDialog::setDataOffline(QString jsonFilePath, QString posterPath){
    QFile file;
    QByteArray bytes;

    file.setFileName(jsonFilePath);
    if(file.open(QIODevice::ReadOnly)){

        bytes = file.readAll();
        file.close();
        ui->plotText->setText(bytes);
        jdoc = QJsonDocument::fromJson(bytes);
        json = jdoc.object();
        setData();
    }
    file.setFileName(posterPath);
    if(file.open(QIODevice::ReadOnly)){

        bytes = file.readAll();
        file.close();
        QPixmap pix;
        pix.loadFromData(bytes);
        ui->posterLabel->setPixmap(pix);
        jdoc = QJsonDocument::fromJson(bytes);
        json = jdoc.object();
        setData();
    }
    else{
        ui->errorLabel->setText("Error opening JSON");
    }
}

void AddMovieDialog::setData(){
    if(!json.isEmpty()){
        if(json["Response"].toString() == "True"){
            ui->nameLabel->setText(json["Title"].toString());
            ui->yearLabel->setText(json["Year"].toString());
            ui->releasedLabel->setText(json["Released"].toString());
            ui->runtimeLabel->setText(json["Runtime"].toString());
            ui->genreLabel->setText(json["Genre"].toString());
            ui->actorsLabel->setText(json["Actors"].toString());
            ui->languageLabel->setText(json["Language"].toString());
            ui->countryLabel->setText(json["Coutnry"].toString());
            ui->awardsLabel->setText(json["Awards"].toString());
            ui->imdbRatingLabel->setText(json["imdbRating"].toString());
            ui->imdbIdLabel->setText(json["imdbID"].toString());
            ui->plotText->setText(json["Plot"].toString());
            ui->errorLabel->setText("");
            ui->saveButton->setEnabled(true);

        }

        else{
            ui->nameLabel->setText("Movie Not Found!");
            ui->yearLabel->setText("");
            ui->releasedLabel->setText("");
            ui->runtimeLabel->setText("");
            ui->genreLabel->setText("");
            ui->actorsLabel->setText("");
            ui->languageLabel->setText("");
            ui->countryLabel->setText("");
            ui->awardsLabel->setText("");
            ui->imdbRatingLabel->setText("");
            ui->imdbIdLabel->setText("");
            ui->plotText->setText("");
            ui->errorLabel->setText("Movie Not Found or Internal Data Error!");
        }
    }
    else{
        ui->errorLabel->setText("Invalid Info File!");
    }
}

void AddMovieDialog::setPosterOnline(){
    QPixmap pix;
    pix.loadFromData(posterDownloader->downloadedData());
    ui->posterLabel->setPixmap(pix);
}

void AddMovieDialog::on_saveButton_clicked()
{
    saveToDatabase();
    QFileInfo fileInfo(file);
    QString path = fileInfo.absolutePath();
    info = path+"/info.json";
    poster = path+"/poster.jpg";
    qDebug()<<info<<" ===> "<<poster;

    if(!jdoc.isEmpty()){
        QFile file (info);
        file.open(QIODevice::WriteOnly);
        file.write(jdoc.toJson());
        //QDataStream out (&file);
        //out << jdoc.toJson();
        file.close();
        ui->posterLabel->pixmap()->save(poster);
    }
}

void AddMovieDialog::saveToDatabase()
{
    QFileInfo fileInfo(file);
    QString id = ui->imdbIdLabel->text();
    QString name = ui->nameLabel->text();
    QString type = ui->typeComboBox->currentText();
    QString language = ui->languageComboBox->currentText();
    QString genre = ui->genreLabel->text();
    QString rating = ui->imdbRatingLabel->text();
    QString year = ui->yearLabel->text();
    QString quality = ui->qualityComboBox->currentText();

    QString appPath = QCoreApplication::applicationDirPath();
    QDir dir(appPath);
    QString relativePath = dir.relativeFilePath(file);
    QString sql = "INSERT INTO movies VALUES ("
                  "\'"+id+"\',"
                  "\'"+name+"\',"
                  "\'"+type+"\',"
                  "\'"+language+"\',"
                  "\'"+genre+"\',"
                  "\'"+rating+"\',"
                  "\'"+year+"\',"
                  "\'"+quality+"\',"
                  "\'"+relativePath+"\')";


    QSqlQuery sqlQuery;
    sqlQuery.prepare(sql);


    if(!sqlQuery.exec()){
        qDebug()<<"Error Executing SQL Query on addmoviedialog.cpp: Line 209.";
    }


}

void AddMovieDialog::on_backButton_clicked()
{
    this->deleteLater();
}

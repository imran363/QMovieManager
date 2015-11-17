#include "moviedetaildialog.h"
#include "ui_moviedetaildialog.h"
#include <QSqlQuery>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>

movieDetailDialog::movieDetailDialog(QString id,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::movieDetailDialog)
{
    ui->setupUi(this);
    movieId = id;
    setData();
}

movieDetailDialog::~movieDetailDialog()
{
    delete ui;
}

void movieDetailDialog::setData()
{
    QJsonDocument jdoc;
    QJsonObject json;
    QString path, jsonPath, posterPath;
    QSqlQuery query;
    query.prepare("SELECT name, path FROM movies WHERE id = \'"+movieId+"\'");
    query.exec();
    while(query.next()){
        ui->movieHEaderLabel->setText(query.value(0).toString());
        path = query.value(1).toString();
    }

    QFile movieFile, jsonFile, posterFile;
    movieFile.setFileName(path);
    QFileInfo fileInfo(movieFile);
    jsonPath = fileInfo.absolutePath() + "/info.json";
    posterPath = fileInfo.absolutePath() + "/poster.jpg";

    jsonFile.setFileName(jsonPath);
    QByteArray bytes;

    if(jsonFile.open(QIODevice::ReadOnly)){
        bytes = jsonFile.readAll();
        jsonFile.close();
        jdoc = QJsonDocument::fromJson(bytes);
        json = jdoc.object();
     }
     posterFile.setFileName(posterPath);
     if(posterFile.open(QIODevice::ReadOnly)){
            bytes = posterFile.readAll();
            posterFile.close();
            QPixmap pix;
            pix.loadFromData(bytes);
            ui->posterLabel->setPixmap(pix);
     }


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

void movieDetailDialog::on_backButton_clicked()
{
    this->deleteLater();
}

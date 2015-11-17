#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addmoviedialog.h"
#include "moviedetaildialog.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlResult>
#include <QSqlError>
#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    getMovies();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addMovieButton_clicked()
{
    AddMovieDialog *add= new AddMovieDialog(this);
    add->show();
}

void MainWindow::getMovies()
{
    QString type = ui->typeComboBox->currentIndex() ? ui->typeComboBox->currentText() : "%";
    QString language = ui->languageComboBox->currentIndex() ? ui->languageComboBox->currentText() : "%";
    QString genre = ui->genreComboBox->currentIndex() ? ui->genreComboBox->currentText() : "%";
    QString rating = ui->ratingComboBox->currentIndex() ? ui->ratingComboBox->currentText() : "0";
    QString sortBy = ui->sortByComboBox->currentIndex() ? ui->sortByComboBox->currentText().toLower() : "name";
    QString sortOrder = ui->sortByComboBox->currentIndex() ? "DESC" : "ASC";
    rating = rating.remove("+");

    moviesId.clear();
    moviesName.clear();
    moviesPath.clear();
    ui->movie1Poster->clear();
    ui->movie2Poster->clear();
    ui->movie3Poster->clear();
    ui->movie4Poster->clear();
    ui->movie1PlayButton->setDisabled(true);
    ui->movie1DetailsButton->setDisabled(true);
    ui->movie2PlayButton->setDisabled(true);
    ui->movie2DetailsButton->setDisabled(true);
    ui->movie3PlayButton->setDisabled(true);
    ui->movie3DetailsButton->setDisabled(true);
    ui->movie4PlayButton->setDisabled(true);
    ui->movie4DetailsButton->setDisabled(true);

    QString startYear, endYear;
    switch (ui->yearComboBox->currentIndex()) {
    case 0:
        startYear = "1900";
        endYear = "2025";
        break;
    case 1:
        startYear = "2011";
        endYear = "2025";
        break;
    case 2:
        startYear = "2001";
        endYear = "2010";
        break;
    case 3:
        startYear = "1991";
        endYear = "2000";
        break;
    case 4:
        startYear = "1981";
        endYear = "1990";
        break;
    case 5:
        startYear = "1900";
        endYear = "1980";
        break;
    default:
        startYear = "1900";
        endYear = "2025";
        break;
    }


    QString sql = "SELECT id, name, path FROM movies WHERE "
                  "type LIKE \'"+type+"\' AND "
                  "language LIKE \'"+language+"\' AND "
                  "genre LIKE \'%"+genre+"%\' AND "
                  "rating >= "+rating+" AND "
                  "year >= "+startYear+" AND "
                  "year <= "+endYear+" ORDER "
                  "BY "+sortBy+" "+sortOrder;
//    qDebug()<<sql<<"\n";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sql);
    if(sqlQuery.exec()){
        while(sqlQuery.next()){
            moviesId.push_back(sqlQuery.value(0).toString());
            moviesName.push_back(sqlQuery.value(1).toString());
            moviesPath.push_back(sqlQuery.value(2).toString());
        }
        currentMovieIndex = 0;
    }

    if((moviesId.size() > 0) && (moviesId.size()%4 == 0)){
        totalPages = moviesId.size() / 4;
    }
    else{
        totalPages = moviesId.size() / 4 +1;
    }
    currentPage = 1;

    ui->totalPagesLabel->setText(QString::number(totalPages));
    ui->currentPageLineEdit->setText(QString::number(currentPage));

    setAllPosters(currentMovieIndex);
}

void MainWindow::setAllPosters(int index)
{
    ui->movie1Poster->clear();
    ui->movie2Poster->clear();
    ui->movie3Poster->clear();
    ui->movie4Poster->clear();
    ui->movie1PlayButton->setDisabled(true);
    ui->movie1DetailsButton->setDisabled(true);
    ui->movie2PlayButton->setDisabled(true);
    ui->movie2DetailsButton->setDisabled(true);
    ui->movie3PlayButton->setDisabled(true);
    ui->movie3DetailsButton->setDisabled(true);
    ui->movie4PlayButton->setDisabled(true);
    ui->movie4DetailsButton->setDisabled(true);

    for(unsigned int i=index, j=1; i<moviesId.size() && j<5; ++i, ++j){
        setPoster(j, moviesPath[i]);
    }

}

void MainWindow::setPoster(int posterNumber, QString path){
    QFile file;
    QByteArray bytes;
    QPixmap pix;

    file.setFileName(path);
    QFileInfo fileInfo(file);
    QString posterPath = fileInfo.absolutePath()+"/poster.jpg";
    file.setFileName(posterPath);
    if(file.open(QIODevice::ReadOnly)){
        bytes = file.readAll();
        file.close();
        pix.loadFromData(bytes);
    }
    else{
        ui->movie1Poster->setText("Error opening JSON");
    }
    pix.scaled(ui->movie1Poster->size());

    switch (posterNumber) {
    case 1:
        ui->movie1Poster->setScaledContents(true);
        ui->movie1Poster->setPixmap(pix);
        ui->movie1PlayButton->setEnabled(true);
        ui->movie1DetailsButton->setEnabled(true);
        break;
    case 2:
        ui->movie2Poster->setScaledContents(true);
        ui->movie2Poster->setPixmap(pix);
        ui->movie2PlayButton->setEnabled(true);
        ui->movie2DetailsButton->setEnabled(true);
        break;
    case 3:
        ui->movie3Poster->setScaledContents(true);
        ui->movie3Poster->setPixmap(pix);
        ui->movie3PlayButton->setEnabled(true);
        ui->movie3DetailsButton->setEnabled(true);
        break;
    case 4:
        ui->movie4Poster->setScaledContents(true);
        ui->movie4Poster->setPixmap(pix);
        ui->movie4PlayButton->setEnabled(true);
        ui->movie4DetailsButton->setEnabled(true);
        break;
    default:
        ui->movie1Poster->setText("Some Error!");
        break;
    }
}

void MainWindow::on_typeComboBox_currentIndexChanged(int index)
{
    getMovies();
}

void MainWindow::on_languageComboBox_currentIndexChanged(int index)
{
    getMovies();
}

void MainWindow::on_genreComboBox_currentIndexChanged(int index)
{
    getMovies();
}

void MainWindow::on_ratingComboBox_currentIndexChanged(int index)
{
    getMovies();
}

void MainWindow::on_yearComboBox_currentIndexChanged(int index)
{
    getMovies();
}

void MainWindow::on_sortByComboBox_currentIndexChanged(int index)
{
    getMovies();
}

void MainWindow::on_nextPushButton_clicked()
{
    if(currentPage < totalPages){
        currentMovieIndex+= 4;
        ++currentPage;
        setAllPosters(currentMovieIndex);
        ui->currentPageLineEdit->setText(QString::number(currentPage));
    }
}

void MainWindow::on_previousPushButton_clicked()
{
    if(totalPages > 1 && currentPage != 1){
        if((currentMovieIndex%4) > 0){
            currentMovieIndex -= 4;
            --currentPage;
            ui->currentPageLineEdit->setText(QString::number(currentPage));
        }
        else{
            currentMovieIndex = 0;
            currentPage =1;
            ui->currentPageLineEdit->setText(QString::number(currentPage));
        }
        setAllPosters(currentMovieIndex);
    }
}

void MainWindow::on_movie1PlayButton_clicked()
{
    QString path = moviesPath[currentMovieIndex];
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));

}
void MainWindow::on_movie2PlayButton_clicked()
{
    QString path = moviesPath[currentMovieIndex+1];
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}



void MainWindow::on_movie3PlayButton_clicked()
{
    QString path = moviesPath[currentMovieIndex+2];
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));


}

void MainWindow::on_movie4PlayButton_clicked()
{
    QString path = moviesPath[currentMovieIndex+3];
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));

}

void MainWindow::on_movie1DetailsButton_clicked()
{
    QString id = moviesId[currentMovieIndex];
    movieDetailDialog *details = new movieDetailDialog(id,this);
    details->show();
}

void MainWindow::on_movie2DetailsButton_clicked()
{
    QString id = moviesId[currentMovieIndex+1];
    movieDetailDialog *details = new movieDetailDialog(id,this);
    details->show();
}

void MainWindow::on_movie3DetailsButton_clicked()
{
    QString id = moviesId[currentMovieIndex+2];
    movieDetailDialog *details = new movieDetailDialog(id,this);
    details->show();
}

void MainWindow::on_movie4DetailsButton_clicked()
{
    QString id = moviesId[currentMovieIndex+3];
    movieDetailDialog *details = new movieDetailDialog(id,this);
    details->show();
}

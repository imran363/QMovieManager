#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setAllPosters(int index);
    void setPoster(int posterNumber, QString path);
    void getMovies();

private slots:
    void on_addMovieButton_clicked();


    void on_typeComboBox_currentIndexChanged(int index);

    void on_languageComboBox_currentIndexChanged(int index);

    void on_genreComboBox_currentIndexChanged(int index);

    void on_ratingComboBox_currentIndexChanged(int index);

    void on_yearComboBox_currentIndexChanged(int index);

    void on_sortByComboBox_currentIndexChanged(int index);

    void on_nextPushButton_clicked();

    void on_previousPushButton_clicked();

    void on_movie2PlayButton_clicked();

    void on_movie1PlayButton_clicked();

    void on_movie3PlayButton_clicked();

    void on_movie4PlayButton_clicked();

    void on_movie1DetailsButton_clicked();

    void on_movie2DetailsButton_clicked();

    void on_movie3DetailsButton_clicked();

    void on_movie4DetailsButton_clicked();

private:
    Ui::MainWindow *ui;
    std::vector<QString> moviesId;
    std::vector<QString> moviesName;
    std::vector<QString> moviesPath;
    int currentMovieIndex = 0;
    int totalPages = 1;
    int currentPage = 1;
};

#endif // MAINWINDOW_H

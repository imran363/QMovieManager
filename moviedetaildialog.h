#ifndef MOVIEDETAILDIALOG_H
#define MOVIEDETAILDIALOG_H

#include <QDialog>

namespace Ui {
class movieDetailDialog;
}

class movieDetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit movieDetailDialog(QString id, QWidget *parent = 0);
    ~movieDetailDialog();
    void setData();

private slots:
    void on_backButton_clicked();

private:
    Ui::movieDetailDialog *ui;
    QString movieId;
};

#endif // MOVIEDETAILDIALOG_H

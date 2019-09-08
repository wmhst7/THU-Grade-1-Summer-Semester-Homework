#ifndef DIALOGHE_H
#define DIALOGHE_H

#include <QDialog>

namespace Ui {
class DialogHe;
}

class DialogHe : public QDialog
{
    Q_OBJECT

public:
    explicit DialogHe(QWidget *parent = nullptr);
    ~DialogHe();

    bool Accept = true;

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::DialogHe *ui;
};

#endif // DIALOGHE_H

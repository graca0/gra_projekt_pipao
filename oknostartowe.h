#ifndef OKNOSTARTOWE_H
#define OKNOSTARTOWE_H

#include <QDialog>

namespace Ui {
class OknoStartowe;
}

class OknoStartowe : public QDialog
{
    Q_OBJECT

public:
    explicit OknoStartowe(QWidget *parent = nullptr);
    ~OknoStartowe();
    int iloscMonet();
    bool czyGramy();
private slots:
    void on_btn_start_clicked();

    void on_pushButton_clicked();

private:
    Ui::OknoStartowe *ui;
    bool gra = true;
};

#endif // OKNOSTARTOWE_H

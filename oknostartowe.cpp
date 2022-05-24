#include "oknostartowe.h"
#include "ui_oknostartowe.h"
#include "mainwindow.h"
OknoStartowe::OknoStartowe(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoStartowe)
{
    ui->setupUi(this);
}

OknoStartowe::~OknoStartowe()
{
    delete ui;
}
int OknoStartowe::iloscMonet(){
    return ui->spin_coins->value();

}
bool OknoStartowe::czyGramy(){
    return this->gra;

}
void OknoStartowe::on_btn_start_clicked()
{
this->close();
}


void OknoStartowe::on_pushButton_clicked()
{
    gra = false;
    this->close();
}


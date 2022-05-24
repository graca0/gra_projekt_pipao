#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Plansza.h"
#include "Entity.h"
#include "oknostartowe.h"
#include <QTimer>
#include <QMessageBox>
#include <QKeyEvent>
#include <QThread>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QMediaPlayer>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startButton_clicked();
    void sledz();
    void odblokujPrzyciski();

private:
    void zmienGrafiki();
    void grafikaRandomMonster();
    void grafikaSledzacy();

    void ustawGrafike();
    void ustawWszystko();
    void wyswietlMape();
    void koniecGry();
    void ustawMonety();
    void sprawdzMonety();
    bool sprawdz();
    void keyPressEvent(QKeyEvent *event);
    void przypiszSciany();

    void zapiszSciane(bool,bool,bool,bool,int,int);
    void rysujSciane(int,int,int);
    void rysujPostaci();
    void czyszczenie();
    void startGry();

    void samaSciana(int,int);
    void goraDolSciana(int,int);
    void dolSciana(int,int);
    void goraSciana(int,int);
    void lewoSciana(int,int);
    void prawoSciana(int,int);
    void goraPrawoSciana(int,int);
    void goraLewoSciana(int,int);
    void dolPrawoSciana(int,int);
    void dolLewoSciana(int,int);
    void lewoPrawoSciana(int,int);
    void czterySciany(int,int);
    void goraLewoPrawo(int,int);
    void dolLewoPrawo(int,int);
    void goraDolPrawo(int,int);
    void goraDolLewo(int,int);

    QImage* image=nullptr;
    QTimer* timer_=nullptr;
    QTimer* buttonTimer_=nullptr;
    Plansza* plansza_ = nullptr;
    Gracz* gracz_;
    RandomMonster* randomMonster_=nullptr;
    RandomMonster*randomMonster2_=nullptr;
    SledzacyMonster *sledzacyMonster_;
    bool wygrana = false;
    QGraphicsItem *item;
    QGraphicsScene* scene = new QGraphicsScene();
    QImage image2;
    QImage image3;
    QImage image4;
    QImage ja;
    QImage kasa;
    QImage podloga;

    QGraphicsPixmapItem*item1;
    QGraphicsPixmapItem* item2;
    QGraphicsPixmapItem* gracz;
    QGraphicsPixmapItem* item3;
    QGraphicsPixmapItem* item4;
    QGraphicsPixmapItem** coins;

    OknoStartowe* okno;
    bool odblokowane=true;
    int sciany[20][20];

    int zmienna=0;
    int punkty=0;
    int predkosc=300;
    int predkoscGracza=50;
    int iloscMonet=0;
    const int szerokosc=20;
    const int skala=50;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

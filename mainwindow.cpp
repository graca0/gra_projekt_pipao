#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Plansza.h"
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QObject>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    startGry();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startGry(){
    okno=new OknoStartowe();
    do{
        okno->exec();
        iloscMonet = okno->iloscMonet();
        if(!okno->czyGramy()){
            exit(EXIT_SUCCESS);
        }
    }while((okno->iloscMonet()<1 || okno->iloscMonet()>19));
    ustawWszystko();
    wyswietlMape();
}
void MainWindow::ustawMonety(){
    std::pair<int, int> puste;

    for(int i=0;i<iloscMonet;i++){
        puste = plansza_->losowePusteMiejsce();
        plansza_->ustawMonete(puste.first,puste.second,true);
    }
}

void MainWindow::sprawdzMonety(){

    for(int i=0;i<szerokosc;i++){
        for(int j=0;j<szerokosc;j++){
            if(plansza_->czyMoneta(i,j)&&gracz_->getX()==i&&gracz_->getY()==j){
                punkty++;
                plansza_->ustawMonete(i,j,false);

                if(punkty==iloscMonet){
                     wygrana=true;
                    koniecGry();
                }
            }
        }
    }
}


void MainWindow::ustawWszystko(){
    timer_=new QTimer();
    buttonTimer_=new QTimer();

    plansza_ = new Plansza(szerokosc,iloscMonet);

    std::pair<int, int> start = plansza_->losowePusteMiejsce();
    gracz_ = new Gracz(start.first, start.second, plansza_-> zwrocPlansze(),szerokosc);

    start = plansza_->losowePusteMiejsce();
    sledzacyMonster_ = new SledzacyMonster(start.first, start.second, plansza_-> zwrocPlansze(), szerokosc);

    start = plansza_->losowePusteMiejsce();
    randomMonster_=new RandomMonster(start.first, start.second, plansza_-> zwrocPlansze(),szerokosc);

    start = plansza_->losowePusteMiejsce();
    start = plansza_->losowePusteMiejsce();
    start = plansza_->losowePusteMiejsce();
    randomMonster2_=new RandomMonster(start.first, start.second, plansza_-> zwrocPlansze(),szerokosc);


    ustawMonety();
    przypiszSciany();

    ustawGrafike();

    QObject::connect(timer_,SIGNAL(timeout()),this,SLOT(sledz()));
    QObject::connect(buttonTimer_,SIGNAL(timeout()),this,SLOT(odblokujPrzyciski()));
    timer_->start(predkosc);
    image=new QImage(szerokosc*skala,szerokosc*skala,QImage::Format_ARGB32);

    int x=0,y=0;
    zmienna=0;

    for(int i=0;i<szerokosc;i++){
        for(int j=0;j<szerokosc;j++){
            if(plansza_->czySciana(i,j)){
                rysujSciane(y,x,sciany[i][j]);
            }
            else{

            }
            x+=skala;
        }
        x=0;
        y+=skala;
    }
    QImage obrazTla;
    obrazTla.load("C:/Users/Graca/Downloads/projektKoniec/obrazki/podloga.png");
    obrazTla = obrazTla.scaled(skala,skala);

    QPixmap pix(skala*szerokosc,skala*szerokosc);
    QPainter p(&pix);
    for(int i=0;i<skala*szerokosc;i+=skala)
         for(int j=0;j<skala*szerokosc;j+=skala)
    p.drawImage(i, j,obrazTla);
    QGraphicsPixmapItem *tlo = new QGraphicsPixmapItem(pix);
    scene->addItem(tlo);


    item = new QGraphicsPixmapItem(QPixmap::fromImage(*image));
    coins = new QGraphicsPixmapItem*[iloscMonet];
    for(int i=0;i<iloscMonet;i++){
        coins[i]= new QGraphicsPixmapItem(QPixmap::fromImage(kasa));
        scene->addItem(coins[i]);
    }
    scene->addItem(item);
    scene->addItem(item2);
    scene->addItem(item3);
    scene->addItem(item4);
    scene->addItem(gracz);

}

void MainWindow::odblokujPrzyciski(){
    odblokowane=true;
    buttonTimer_->stop();
}


void MainWindow::keyPressEvent(QKeyEvent *event){
    if(odblokowane){
        if(event->key()==Qt::Key_W){
            gracz_->ruch(4);
            ja.load("C:/Users/Graca/Downloads/projektKoniec/obrazki/lisTyl.png");
        }
        if(event->key()==Qt::Key_A){
            gracz_->ruch(8);
            ja.load("C:/Users/Graca/Downloads/projektKoniec/obrazki/lisLewo.png");
        }
        if(event->key()==Qt::Key_D){
            gracz_->ruch(2);
            ja.load("C:/Users/Graca/Downloads/projektKoniec/obrazki/lisPrawo.png");
        }
        if(event->key()==Qt::Key_S){
            gracz_->ruch(6);
            ja.load("C:/Users/Graca/Downloads/projektKoniec/obrazki/lisFront.png");
        }
        if(sprawdz())
            koniecGry();
        sprawdzMonety();

        wyswietlMape();
        buttonTimer_->start(predkoscGracza);
        odblokowane=false;
        ja=ja.scaled(skala,skala);
        gracz->setPixmap(QPixmap::fromImage(ja));
    }
}
void MainWindow::sledz(){
    sledzacyMonster_->sledzenie(gracz_);
    randomMonster_->ruch();
    randomMonster2_->ruch();
    zmienGrafiki();
    wyswietlMape();
    if(sprawdz())
        koniecGry();
}

void MainWindow::koniecGry(){

    QMessageBox msgBox;
    czyszczenie();
    if( wygrana)
    msgBox.setText("Wygrales");
    else
   msgBox.setText("Koniec Gry przegrales");
   wygrana = false;
    msgBox.exec();
    startGry();
}

bool MainWindow::sprawdz(){
    return gracz_->getX()==sledzacyMonster_->getX()&&gracz_->getY()==sledzacyMonster_->getY()
            ||gracz_->getX()==randomMonster_->getX()&&gracz_->getY()==randomMonster_->getY()
            ||gracz_->getX()==randomMonster2_->getX()&&gracz_->getY()==randomMonster2_->getY();
}

void MainWindow::on_startButton_clicked()
{
    ustawWszystko();
    wyswietlMape();
}

void MainWindow::wyswietlMape(){
    rysujPostaci();
    ui->graphicsView->setScene(scene);
}

void MainWindow::rysujPostaci(){
    int liczba2=0;
    int licznikScian=0;

    for(int i=0;i<szerokosc;i++){
        for(int j=0;j<szerokosc;j++){
            if(plansza_->czyMoneta(i,j)){
                coins[liczba2]->setPos(i*skala,j*skala);
                if(liczba2<iloscMonet)
                    liczba2++;
            }

            else{
                if(liczba2<iloscMonet)
                    coins[liczba2]->setPos(0,0);

                if(gracz_->getX()==i&&gracz_->getY()==j)
                    gracz->setPos(i*skala,j*skala);
                else if(sledzacyMonster_->getX()==i&&sledzacyMonster_->getY()==j)
                    item2->setPos(i*skala,j*skala);
                else if(randomMonster_->getX()==i&&randomMonster_->getY()==j)
                    item3->setPos(i*skala,j*skala);
                else if(randomMonster2_->getX()==i&&randomMonster2_->getY()==j)
                    item4->setPos(i*skala,j*skala);


            }
        }
    }
}

void MainWindow::ustawGrafike(){
    int liczba=0;

    podloga.load("C:/Users/Graca/Downloads/projektKoniec/obrazki/podloga.png");
    ja.load("C:/Users/Graca/Downloads/projektKoniec/obrazki/lisPrawo.png");
    kasa.load("C:/Users/Graca/Downloads/projektKoniec/obrazki/apple.png");

    podloga=podloga.scaled(skala,skala);
    ja=ja.scaled(skala,skala);
    kasa=kasa.scaled(skala,skala);
    image2=image2.scaled(skala,skala);
    image3=image3.scaled(skala,skala);
    image4=image4.scaled(skala,skala);



    item1=new QGraphicsPixmapItem(QPixmap::fromImage(podloga));
    item2=new QGraphicsPixmapItem(QPixmap::fromImage(image2));
    gracz=new QGraphicsPixmapItem(QPixmap::fromImage(ja));
    item3=new QGraphicsPixmapItem(QPixmap::fromImage(image3));
    item4=new QGraphicsPixmapItem(QPixmap::fromImage(image4));
    zmienGrafiki();
}

void MainWindow::samaSciana(int x,int y){
    for(int i=x;i<x+skala;i++){
        for(int j=y;j<y+skala;j++){
            if(j>=y+15&&j<=(y+skala)-15&&i>=x+15&&i<=(x+skala)-15)
                image->setPixelColor(i,j,Qt::black);
            else
                image->setPixelColor(i,j,Qt::gray);
        }
    }


}

void MainWindow::goraDolSciana(int x, int y){
    for(int i=x;i<x+skala;i++){
        for(int j=y;j<y+skala;j++){
            if(i>=x+15&&i<=(x+skala)-15)
                image->setPixelColor(i,j,Qt::black);
            else
                image->setPixelColor(i,j,Qt::gray);

        }
    }
}

void MainWindow::dolSciana(int x, int y){
    for(int i=x;i<x+skala;i++){
        for(int j=y;j<y+skala;j++){
            if(i>=x+15&&i<=(x+skala)-15&&j>=y+15)
                image->setPixelColor(i,j,Qt::black);
            else
                image->setPixelColor(i,j,Qt::gray);

        }
    }
}
void MainWindow::goraSciana(int x, int y){
    for(int i=x;i<x+skala;i++){
        for(int j=y;j<y+skala;j++){
            if(i>=x+15&&i<=(x+skala)-15&&j<=(y+skala)-15)
                image->setPixelColor(i,j,Qt::black);
            else
                image->setPixelColor(i,j,Qt::gray);

        }
    }
}

void MainWindow::lewoSciana(int x, int y){
    for(int i=x;i<x+skala;i++){
        for(int j=y;j<y+skala;j++){
            if(j>=y+15&&j<=(y+skala)-15&&i<=(x+skala)-15)
                image->setPixelColor(i,j,Qt::black);
            else
                image->setPixelColor(i,j,Qt::gray);
        }
    }
}
void MainWindow::prawoSciana(int x, int y){
    for(int i=x;i<x+skala;i++){
        for(int j=y;j<y+skala;j++){
            if(j>=y+15&&j<=(y+skala)-15&&i>=x+15)
                image->setPixelColor(i,j,Qt::black);
            else
                image->setPixelColor(i,j,Qt::gray);
        }
    }
}

void MainWindow::goraPrawoSciana(int x, int y){
    for(int i=x;i<x+skala;i++){
        for(int j=y;j<y+skala;j++){
            if((i>=x+15&&i<=(x+skala)-15&&j<=(y+skala)-15)||(j>=y+15&&j<=(y+skala)-15&&i>=x+15))
                image->setPixelColor(i,j,Qt::black);
            else
                image->setPixelColor(i,j,Qt::gray);
        }
    }
}
void MainWindow::goraLewoSciana(int x, int y){
    for(int i=x;i<x+skala;i++){
        for(int j=y;j<y+skala;j++){
            if((i>=x+15&&i<=(x+skala)-15&&j<=(y+skala)-15)||(j>=y+15&&j<=(y+skala)-15&&i<=(x+skala)-15))
                image->setPixelColor(i,j,Qt::black);
            else
                image->setPixelColor(i,j,Qt::gray);
        }
    }
}

void MainWindow::dolPrawoSciana(int x, int y){
    for(int i=x;i<x+skala;i++){
        for(int j=y;j<y+skala;j++){
            if((i>=x+15&&i<=(x+skala)-15&&j>=y+15)||(j>=y+15&&j<=(y+skala)-15&&i>=x+15))
                image->setPixelColor(i,j,Qt::black);
            else
                image->setPixelColor(i,j,Qt::gray);
        }
    }
}
void MainWindow::dolLewoSciana(int x, int y){
    for(int i=x;i<x+skala;i++){
        for(int j=y;j<y+skala;j++){
            if((i>=x+15&&i<=(x+skala)-15&&j>=y+15)||(j>=y+15&&j<=(y+skala)-15&&i<=(x+skala)-15))
                image->setPixelColor(i,j,Qt::black);
            else
                image->setPixelColor(i,j,Qt::gray);
        }
    }
}

void MainWindow::lewoPrawoSciana(int x, int y){
    for(int i=x;i<x+skala;i++){
        for(int j=y;j<y+skala;j++){
            if(j>=y+15&&j<=y+skala-15)
                image->setPixelColor(i,j,Qt::black);
            else
                image->setPixelColor(i,j,Qt::gray);
        }
    }
}
void MainWindow::czterySciany(int x, int y){
    for(int i=x;i<x+skala;i++){
        for(int j=y;j<y+skala;j++){
            if((i>=x+15&&i<=x+skala-15)||(j>=y+15&&j<=y+skala-15))
                image->setPixelColor(i,j,Qt::black);
            else
                image->setPixelColor(i,j,Qt::gray);
        }
    }
}

void MainWindow::goraLewoPrawo(int x,int y){
    for(int i=x;i<x+skala;i++){
        for(int j=y;j<y+skala;j++){
            if((j>=y+15&&j<=y+skala-15)||(i>=x+15&&i<=(x+skala)-15&&j<=(y+skala)-15))
                image->setPixelColor(i,j,Qt::black);
            else
                image->setPixelColor(i,j,Qt::gray);
        }
    }
}
void MainWindow::dolLewoPrawo(int x,int y){
    for(int i=x;i<x+skala;i++){
        for(int j=y;j<y+skala;j++){
            if(j>=y+15&&j<=y+skala-15||(i>=x+15&&i<=(x+skala)-15&&j>=y+15))
                image->setPixelColor(i,j,Qt::black);
            else
                image->setPixelColor(i,j,Qt::gray);
        }
    }
}

void MainWindow::goraDolPrawo(int x,int y){
    for(int i=x;i<x+skala;i++){
        for(int j=y;j<y+skala;j++){
            if(i>=x+15&&i<=(x+skala)-15||(j>=y+15&&j<=(y+skala)-15&&i>=x+15))
                image->setPixelColor(i,j,Qt::black);
            else
                image->setPixelColor(i,j,Qt::gray);
        }
    }
}
void MainWindow::goraDolLewo(int x,int y){
    for(int i=x;i<x+skala;i++){
        for(int j=y;j<y+skala;j++){
            if(i>=x+15&&i<=(x+skala)-15||(j>=y+15&&j<=(y+skala)-15&&i<=(x+skala)-15))
                image->setPixelColor(i,j,Qt::black);
            else
                image->setPixelColor(i,j,Qt::gray);
        }
    }
}

void MainWindow::zmienGrafiki(){
    grafikaRandomMonster();
    grafikaSledzacy();
}

void MainWindow::grafikaSledzacy(){
    if(sledzacyMonster_->getStan()==1)
        image2.load("C:/Users/Graca/Downloads/projektKoniec/obrazki/tigerTyl.png");
    if(sledzacyMonster_->getStan()==2)
        image2.load("C:/Users/Graca/Downloads/projektKoniec/obrazki/tigerPrawo.png");
    if(sledzacyMonster_->getStan()==3)
        image2.load("C:/Users/Graca/Downloads/projektKoniec/obrazki/tigerFront.png");
    if(sledzacyMonster_->getStan()==4)
        image2.load("C:/Users/Graca/Downloads/projektKoniec/obrazki/tigerLewo.png");
    image2=image2.scaled(skala,skala);
    item2->setPixmap(QPixmap::fromImage(image2));
}

void MainWindow::grafikaRandomMonster(){
    if(randomMonster_->getStan()==1)
        image3.load("C:/Users/Graca/Downloads/projektKoniec/obrazki/szopTyl.png");
    if(randomMonster_->getStan()==2)
        image3.load("C:/Users/Graca/Downloads/projektKoniec/obrazki/szopPrawo.png");
    if(randomMonster_->getStan()==3)
        image3.load("C:/Users/Graca/Downloads/projektKoniec/obrazki/szopFront.png");
    if(randomMonster_->getStan()==4)
        image3.load("C:/Users/Graca/Downloads/projektKoniec/obrazki/szopLewo.png");

    if(randomMonster2_->getStan()==1)
        image4.load("C:/Users/Graca/Downloads/projektKoniec/obrazki/pigTyl.png");
    if(randomMonster2_->getStan()==2)
        image4.load("C:/Users/Graca/Downloads/projektKoniec/obrazki/pigPrawo.png");
    if(randomMonster2_->getStan()==3)
        image4.load("C:/Users/Graca/Downloads/projektKoniec/obrazki/pigFront.png");
    if(randomMonster2_->getStan()==4)
        image4.load("C:/Users/Graca/Downloads/projektKoniec/obrazki/pigLewo.png");

    image3=image3.scaled(skala,skala);
    image4=image4.scaled(skala,skala);
    item3->setPixmap(QPixmap::fromImage(image3));
    item4->setPixmap(QPixmap::fromImage(image4));
}

void MainWindow::zapiszSciane(bool gora,bool prawo,bool dol, bool lewo,int x,int y){
    if(!gora&&!prawo&&!dol&&!lewo)
        sciany[x][y]=1;
    if(gora&&!prawo&&!dol&&!lewo)
        sciany[x][y]=2;
    if(!gora&&prawo&&!dol&&!lewo)
        sciany[x][y]=3;
    if(!gora&&!prawo&&dol&&!lewo)
        sciany[x][y]=4;
    if(!gora&&!prawo&&!dol&&lewo)
        sciany[x][y]=5;
    if(gora&&prawo&&!dol&&!lewo)
        sciany[x][y]=6;
    if(gora&&!prawo&&dol&&!lewo)
        sciany[x][y]=7;
    if(gora&&!prawo&&!dol&&lewo)
        sciany[x][y]=8;
    if(!gora&&prawo&&dol&&!lewo)
        sciany[x][y]=9;
    if(!gora&&prawo&&!dol&&lewo)
        sciany[x][y]=10;
    if(!gora&&!prawo&&dol&&lewo)
        sciany[x][y]=11;
    if(gora&&prawo&&!dol&&lewo)
        sciany[x][y]=12;
    if(gora&&prawo&&dol&&!lewo)
        sciany[x][y]=13;
    if(gora&&!prawo&&dol&&lewo)
        sciany[x][y]=14;
    if(!gora&&prawo&&dol&&lewo)
        sciany[x][y]=15;
    if(gora&&prawo&&dol&&lewo)
        sciany[x][y]=16;
}


void MainWindow::rysujSciane(int x,int y,int n){
    switch(n){
    case 1:
        samaSciana(x,y);
        break;
    case 2:
        lewoSciana(x,y);
        break;
    case 3:
        dolSciana(x,y);
        break;
    case 4:
        prawoSciana(x,y);
        break;
    case 5:
        goraSciana(x,y);
        break;
    case 6:
        dolLewoSciana(x,y);
        break;
    case 7:
        lewoPrawoSciana(x,y);
        break;
    case 8:
        goraLewoSciana(x,y);
        break;
    case 9:
        dolPrawoSciana(x,y);
        break;
    case 10:
        goraDolSciana(x,y);
        break;
    case 11:
        goraPrawoSciana(x,y);

        break;
    case 12:
        goraDolLewo(x,y);
        break;
    case 13:
        dolLewoPrawo(x,y);

        break;
    case 14:
        goraLewoPrawo(x,y);
        break;
    case 15:
        goraDolPrawo(x,y);
        break;
    case 16:
        czterySciany(x,y);
        break;
    }
}


void MainWindow::przypiszSciany(){
    bool gora=false;
    bool dol=false;
    bool lewo=false;
    bool prawo=false;
    for(int i=0;i<szerokosc;i++){
        for(int j=0;j<szerokosc;j++){
            if(plansza_->czySciana(i,j))
            {
                if(j>0)
                    lewo=plansza_->czySciana(i,j-1);
                if(j<szerokosc-1)
                    prawo=plansza_->czySciana(i,j+1);
                if(i>0)
                    gora=plansza_->czySciana(i-1,j);
                if(i<szerokosc-1)
                    dol=plansza_->czySciana(i+1,j);
                zapiszSciane(gora,prawo,dol,lewo,i,j);
            }
            else{
                sciany[i][j]=0;
            }
            gora=false;
            lewo=false;
            prawo=false;
            dol=false;
        }
    }
}

void MainWindow::czyszczenie(){
    //okno->~OknoStartowe();

    delete timer_;
    delete buttonTimer_;
    delete plansza_;
    delete gracz_;
    delete sledzacyMonster_;
    delete randomMonster_;
    delete randomMonster2_;

    for(int i=0;i<iloscMonet;i++)
        delete[] coins[i];
    delete [] coins;

    delete  item2;
    delete  gracz;
    delete item3;
    delete  item4;
    punkty=0;
    scene->clear();
}

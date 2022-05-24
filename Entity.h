#pragma once
#include <cmath>
#include <algorithm>
#include "Plansza.h"
#include <queue>
class Entity
{
private:
    int polozenieX{}, polozenieY{};
    void setX(int x) { polozenieX = x; }
    void setY(int y) { polozenieY = y; }
    KomponentPlanszy** pl;
    int stan=0;
public:

    Entity(int x, int y, KomponentPlanszy** p) {
        polozenieX = x;
        polozenieY = y;
        pl = p;
    };
    int getStan(){return stan;}
    int getX() { return polozenieX; }
    int getY() { return polozenieY; }
    KomponentPlanszy** getPlansza() { return pl; }
    void ruszSie(bool os, int gdzie) {
        if (os)
            zmianaX(gdzie);
        else
            zmianaY(gdzie);
    }
    void zmianaX(int x) {
        if (x > 0)
        {
            setX(this->polozenieX += 1);
            stan=2;
        }
        else if (x < 0)
        {
            setX(this->polozenieX -= 1);
            stan=4;
        }
    }
    void zmianaY(int y) {
        if (y > 0){
            setY(polozenieY += 1);
            stan=3;
        }
        else if (y < 0){
            setY(polozenieY -= 1);
            stan=1;
        }
    }


};

class Gracz :public Entity {
public:
    Gracz(int x, int y, KomponentPlanszy** p, int s) :Entity(x, y, p) {}
    void ruch(int x) {
        int row = this->getX();
        int col = this->getY();

        switch (x)
        {
        case 2:
            row += 1;
            break;
        case 8:
            row -= 1;
            break;
        case 4:
            col -= 1;
            break;
        case 6:
            col += 1;
            break;
        default:
            break;
        }
        if (this->getPlansza()[row / 5][col / 5].getPole(row % 5, col % 5).getSciana() == false) {
            this->zmianaY(col - this->getY());
            this->zmianaX(row - this->getX());

        }
    }
};
class RandomMonster : public Entity {
    int szerokoscPlanszy;

public:
    RandomMonster(int x, int y, KomponentPlanszy** p,int s) :Entity(x, y,p),szerokoscPlanszy(s) {

    }
    int getKierunek() { return rand() % 4; }

    bool miesciSie(int wiersz, int kolumna)
    {
        return (wiersz >= 0) && (wiersz < szerokoscPlanszy) &&
            (kolumna >= 0) && (kolumna < szerokoscPlanszy);
    }
    bool ruch() {

        int licznik = 0;
        int row = 0;
        int col = 0;
            while(licznik<5){
                licznik++;
                row = this->getX();
                col = this->getY();
            switch (getKierunek())
            {
            case 0:
                row -= 1;
                //cout << "G \n";
                break;
            case 1:
                col -= 1;
                //cout << "L \n";
                break;
            case 2:
                col += 1;
                //cout << "P \n";
                break;
            case 3:
                row += 1;
                //cout << "D \n";
                break;
            default:
                break;
            }
            if (miesciSie(row, col) && this->getPlansza()[row / 5][col / 5].getPole(row % 5, col % 5).getSciana() == false ) {
                this->zmianaY(col - this->getY());
                this->zmianaX(row - this->getX());
                cout << endl;
                return true;
            }
        }
        return false;
    }
};
struct punkt {
    int x, y;

};
struct kolejkaBTS {
    punkt pt;
    int odleglosc;
    string ruch;

};
class SledzacyMonster :public Entity {
private:
    Gracz* gracz_;

    int szerokoscPlanszy;
    bool** tablicaprawdy;

    bool miesciSie(int wiersz, int kolumna)
    {
        return (wiersz >= 0) && (wiersz < szerokoscPlanszy) &&
            (kolumna >= 0) && (kolumna < szerokoscPlanszy);
    }

    char szukanie() {
        if (this->getPlansza()[this->getX() / 5][this->getY() / 5].getPole(this->getX() % 5, this->getY() % 5).getSciana() == true ||
            this->getPlansza()[gracz_->getX() / 5][gracz_->getY() / 5].getPole(gracz_->getX() % 5, gracz_->getY() % 5).getSciana() == true)
            return ' ';
        zerowanieTablicy();
        tablicaprawdy[this->getX()][this->getY()] = true;
        std::queue<kolejkaBTS> q;
        kolejkaBTS s = { {this->getX(),this->getY()},0,"" };
        q.push(s);
        string gdzie = " ";
        int rowNum[] = { -1, 0, 0, 1 };
        int colNum[] = { 0, -1, 1, 0 };
        while (!q.empty()) {

            kolejkaBTS curr = q.front();
            punkt pt = curr.pt;

            if (pt.x ==gracz_->getX() && pt.y == gracz_->getY()) {
                return curr.ruch[0];
            }

        //	cout << "x=" << q.front().pt.x << " ";
        //	cout << "y=" << q.front().pt.y << endl;
        //	cout << "odleglosc=" << q.front().odleglosc << endl;
        //	cout << "ruch=" << q.front().ruch << endl;
            q.pop();

            for (int i = 0; i < 4; i++) {
                int row = pt.x + rowNum[i];
                int col = pt.y + colNum[i];

                if (miesciSie(row, col) && this->getPlansza()[row / 5][col / 5].getPole(row % 5, col % 5).getSciana() == false && !tablicaprawdy[row][col]) {
                    tablicaprawdy[row][col] = true;
                    gdzie = ' ';
                    switch (i)
                    {
                    case 0:
                        gdzie = 'G';
                        break;
                    case 1:
                        gdzie = 'L';
                        break;
                    case 2:
                        gdzie = 'P';
                        break;
                    case 3:
                        gdzie = 'D';
                        break;

                    default:
                        break;
                    }
                    kolejkaBTS dodaniekom{ {row,col},curr.odleglosc + 1,curr.ruch + gdzie };
                    q.push(dodaniekom);
                }
            }
        }
        return ' ';





    }
    void zerowanieTablicy() {

        for (int i = 0; i < this->szerokoscPlanszy; i++) {
            for (int j = 0; j < this->szerokoscPlanszy; j++) {
                tablicaprawdy[i][j] = false;
            }

        }
    }

    public:
        bool koniec() {
            cout << gracz_->getX();
            cout << gracz_->getY();
                return false;
        }
    SledzacyMonster(int x, int y, KomponentPlanszy** p,int s) :Entity(x, y,p) {
    //gracz_ = nullptr;

        szerokoscPlanszy = s;

        tablicaprawdy = new bool* [s];
        for (int i = 0; i < s; i++) {
            tablicaprawdy[i] = new bool[s];
        }
        zerowanieTablicy();
        for (int i = 0; i < s; i++) {
            for (int j = 0; j < s; j++) {
                if (tablicaprawdy[i][j])
                    cout << "a";
                else
                    cout << "b";
            }
            cout << endl;
        }
    };

    void ustawCel(Gracz* gracz) {
        gracz_ = gracz;
    }

    void sledzenie(Gracz* gracz) {
        ustawCel(gracz);

        switch (szukanie())
        {
        case 'D':
            this->zmianaX(1);
            break;

        case 'G':
            this->zmianaX(-1);
            break;
        case 'L':
            this->zmianaY(-1);
            break;
        case 'P':
            this->zmianaY(1);
            break;

        default:
            cout << "blad";
            break;
        }

    }

};

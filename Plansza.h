#pragma once
#include <iostream>
#include <stdlib.h>  
#include <time.h> 
#include "KomponentPlanszy.h"

using namespace std;

class Plansza
{
private:
	int szerokosc = 0;
    int iloscMonet=0;

	KomponentPlanszy** plansza;

	void stworzPlansze() {
		plansza = new KomponentPlanszy * [szerokosc / 5];
		for (int i = 0; i < szerokosc / 5; i++)
			plansza[i] = new KomponentPlanszy[szerokosc / 5];
	}

	void generujPlansze() {
		srand(time(NULL));
		for (int i = 0; i < szerokosc / 5; i++) {
			for (int j = 0; j < szerokosc / 5; j++) {
				plansza[i][j].stworzSciany(j == 0, j == (szerokosc / 5) - 1, i == 0, i == (szerokosc / 5) - 1);
				plansza[i][j].generujKwadrat();
			}
		}
	}

public:
    Plansza(int szerokosc,int monety) {
        this->iloscMonet=monety;
		this->szerokosc = szerokosc;
		stworzPlansze();
		generujPlansze();
	}
    bool czySciana(int x, int y) {
        return plansza[x / 5][y / 5].getPole(x % 5, y % 5).getSciana();
	}
    bool jakiTyp(int x,int y){
        return plansza[x / 5][y / 5].getPole(x % 5, y % 5).getTyp();
    }

    bool czyMoneta(int x, int y) {
        return plansza[x / 5][y / 5].getPole(x % 5, y % 5).getMoneta();
    }

    void ustawMonete(int x,int y,bool m){
        plansza[x / 5][y / 5].getPole(x % 5, y % 5).setMoneta(m);
    }

	std::pair<int, int> losowePusteMiejsce() {
		if (plansza == nullptr)
			return std::pair<int, int>(-1, -1);
		
		int x = 0, y = 0;
		do {
			x = rand() % szerokosc;
			y = rand() % szerokosc;
		} while (plansza[x / 5][y / 5].getPole(x % 5, y % 5).getSciana() == true);
		return std::pair<int, int>(x, y);

	}

    KomponentPlanszy** zwrocPlansze() { return plansza; }
	void wypiszPlansze() {
		for (int i = 0; i < szerokosc; i++) {
			for (int j = 0; j < szerokosc; j++) {
				if (plansza[i / 5][j / 5].getPole(i % 5, j % 5).getSciana())
					cout << (char)219;
				else
					cout << " ";
			}
			cout << endl;
		}
	}
};


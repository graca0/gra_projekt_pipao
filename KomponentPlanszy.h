#pragma once
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "Pole.h"
class KomponentPlanszy
{
private:
	const int szerokosc = 5;
	Pole** kwadrat;
	bool lew = false, pra = false, gor = false, dl = false;

	void stworzKwadrat() {
		kwadrat = new Pole * [szerokosc];
		for (int i = 0; i < szerokosc; i++)
		{
			kwadrat[i] = new Pole[szerokosc];
		}
	}

	void gora() {
		for (int i = 0; i < szerokosc; i++) {
			kwadrat[0][i].setSciana(true);
		}
	}

	void dol() {
		for (int i = 0; i < szerokosc; i++) {
			kwadrat[szerokosc - 1][i].setSciana(true);
		}
	}

	void lewo() {
		for (int i = 0; i < szerokosc; i++) {
			kwadrat[i][0].setSciana(true);
		}
	}

	void prawo() {
		for (int i = 0; i < szerokosc; i++) {
			kwadrat[i][szerokosc - 1].setSciana(true);
		}
	}

	bool zablokowany(int x, int y) {
		return kwadrat[x - 1][y].getSciana() && kwadrat[x + 1][y].getSciana() && kwadrat[x][y - 1].getSciana() && kwadrat[x][y + 1].getSciana();
	}

	void stworzPrzeszkody() {

		for (int i = 1; i < 4; i++) {
			int los = 0;
			for (int j = 1; j < 4; j++) {
				los = rand() % 2;
				if (!(los % 2))
					kwadrat[i][j].setSciana(true);
			}
		}
	}

	bool sprawdzGora(int x, int y) {
		for (int i = x; i >= 0; i--) {
			if (kwadrat[i][y].getSciana())
				return false;
		}
		return true;
	}

	bool sprawdzDol(int x, int y) {
		for (int i = x; i < szerokosc; i++) {
			if (kwadrat[i][y].getSciana())
				return false;
		}
		return true;
	}

	bool sprawdzPrawo(int x, int y) {
		for (int i = y; i >= 0; i--) {
			if (kwadrat[x][i].getSciana())
				return false;
		}
		return true;
	}

	bool sprawdzLewo(int x, int y) {
		for (int i = y; i < szerokosc; i++) {
			if (kwadrat[x][i].getSciana())
				return false;
		}
		return true;
	}

	bool sprawdzWarunki(int x, int y) {
		return !sprawdzDol(x, y) && !sprawdzPrawo(x, y) && !sprawdzGora(x, y) && !sprawdzLewo(x, y);
	}


	void uwolnijPole(int x, int y) {
		if (sprawdzWarunki(x, y)) {

			if (x > 0 && !gor && sprawdzWarunki(x, y))
				kwadrat[x - 1][y].setSciana(false);
			if (x < szerokosc - 1 && !dl && sprawdzWarunki(x, y))
				kwadrat[x + 1][y].setSciana(false);
			if (y > 0 && !lew && sprawdzWarunki(x, y))
				kwadrat[x][y - 1].setSciana(false);
			if (y < szerokosc - 1 && !pra && sprawdzWarunki(x, y))
				kwadrat[x][y + 1].setSciana(false);
		}
	}

	void uwolnijPola() {

		for (int i = 1; i < 4; i++) {
			for (int j = 1; j < 4; j++) {
				uwolnijPole(i, j);
			}
		}
	}

public:
	KomponentPlanszy() {
		stworzKwadrat();
	}

    Pole& getPole(int x, int y) {
        return kwadrat[x][y];
	}

	void generujKwadrat() {
		stworzPrzeszkody();
		uwolnijPola();
	}

    void ustawOtoczenie(int x, int y,int i, Pole*p){
        kwadrat[x][y].setOtoczenie(i,p);
    }

	void stworzSciany(bool l, bool p, bool g, bool d) {
		lew = l;
		pra = p;
		gor = g;
		dl = d;

		if (lew)
			lewo();
		if (pra)
			prawo();
		if (gor)
			gora();
		if (dl)
			dol();
	}

};


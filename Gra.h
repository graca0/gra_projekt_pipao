#pragma once
#include "Entity.h"


class Gra {
private:
	Plansza* plansza_ = nullptr;
	Gracz* gracz_;
	SledzacyMonster *sledzacyMonster_;
    int szerokosc_;
public:
	Gra(int szerokosc) {
		szerokosc_ = szerokosc;
		plansza_ = new Plansza(szerokosc);
		std::pair<int, int> start = plansza_->losowePusteMiejsce();
		gracz_ = new Gracz(start.first, start.second, plansza_->zworcPlansze());
		start = plansza_->losowePusteMiejsce();
		sledzacyMonster_ = new SledzacyMonster(start.first, start.second, plansza_->zworcPlansze(), szerokosc);
	}
    void wyswietlGre() {


	}
	bool koniecGry() {
		return sledzacyMonster_->koniec();
	}
	void ruch() {
		int a;
		std::cout << "gdzie:";
		std::cin >> a;
		gracz_->ruch(a);
		sledzacyMonster_->sledzenie(gracz_);
        wyswietlGre();

	}

};

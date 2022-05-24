#pragma once
class Pole
{
private:
	bool sciana = false;
    bool moneta=false;
    Pole* otoczenie[4];
public:
	Pole() {

	}
	Pole(bool s) {
		setSciana(s);
	}
    int getTyp(){

    }
    bool getMoneta(){return moneta;}
	bool getSciana() { return sciana; }
	bool getOtoczenie(int i) {
		if (otoczenie[i] != nullptr)
			return otoczenie[i]->getSciana();
		else
			return 0;
	}

	void zmienOtoczenie(int i) {
		otoczenie[i]->setSciana(false);
	}

    void setOtoczenie(int i, Pole* pole) {
		otoczenie[i] = pole;
	}
	void setSciana(bool s) {
		sciana = s;
	}
    void setMoneta(bool m){
        moneta=m;
    }
};


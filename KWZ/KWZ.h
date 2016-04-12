#pragma once
#include "stdafx.h"
struct Task
{
	int czas_trwania;
	int ES, EF, LS, LF;
	std::vector<int> poprzednicy;
};

class CPM
{
	int _Liczba_zadan;
	int _Liczba_polaczen;
	int _Sciezka;
	int _Czas_procesu;
	Task *_Zadania;
	int *_Liczba_poprzednikow;
	std::vector<std::pair<int, int> > _Polaczenia;
	std::vector<int> _Kolejnosc;
	std::vector<int> _KolejnoscTopologiczna;

public:

	CPM() : _Liczba_zadan(0), _Liczba_polaczen(0), _Sciezka(0), _Czas_procesu(0), _Zadania(nullptr), _Liczba_poprzednikow(nullptr) {}
	void wczytaj_dane();
	void zlicz_poprzednikow();
	void sortuj_topoligicznie();
	void es_ef();
	void ls_lf();
	void sciezka_krytyczna();
	void pokaz_wynik();

};
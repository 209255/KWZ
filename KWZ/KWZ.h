#pragma once
#include "stdafx.h"
#include <algorithm>
struct Task
{
	int czas_trwania;
	int najmniejszy_czas;
	int koszt_skrocenia;
	int ES, EF, LS, LF;
	std::vector<int> poprzednicy;
	friend std::ostream &operator << (std::ostream & StrmWyj, Task const & p)
	{
		StrmWyj << p.czas_trwania << "\t";
		StrmWyj << p.najmniejszy_czas << "\t";
		StrmWyj << p.koszt_skrocenia << "\t";
		return StrmWyj;
	}
	
};

class CPM
{
	int _dostepny_budzet;
	int _budzet;
	int _Liczba_zadan;
	int _Liczba_polaczen;
	int _Sciezka;
	int _Czas_procesu;
	int _totalCost;
	Task *_Zadania;
	int *_Liczba_poprzednikow;
	std::vector<std::pair<int, int> > _Polaczenia;
	std::vector<int> _Kolejnosc;
	std::vector<int> _KolejnoscTopologiczna;
	std::vector<int> _shortenList;
	std::vector<int> _criticalPath;

public:

	CPM() : _Liczba_zadan(0), _Liczba_polaczen(0), _Sciezka(0), _Czas_procesu(0), _Zadania(nullptr), _Liczba_poprzednikow(nullptr) {}
	void wczytaj_dane();
	void zlicz_poprzednikow();
	void sortuj_topoligicznie();
	void es_ef();
	void ls_lf();
	void sciezka_krytyczna();
	void pokaz_wynik();
	void alg();
	void resulat();
	int maxEf();
};
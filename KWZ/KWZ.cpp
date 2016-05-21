// KWZ.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "KWZ.h"


void CPM::wczytaj_dane()
{
	std::fstream plik;
	plik.open("data10.txt");
	plik >> _Liczba_zadan;
	plik >> _Liczba_polaczen;
	plik >> _dostepny_budzet;
	_budzet = _dostepny_budzet;
	_Zadania = new Task[_Liczba_zadan];
	_Liczba_poprzednikow = new int[_Liczba_zadan];

	for (int i = 0; i < _Liczba_zadan; ++i)
	{
		plik >> _Zadania[i].czas_trwania;
		plik >> _Zadania[i].najmniejszy_czas;
		plik >> _Zadania[i].koszt_skrocenia;
		_Liczba_poprzednikow[i] = 0;
	}

	for (int i = 0; i < _Liczba_polaczen; ++i)
	{
		std::pair<int, int> temp;
		plik >> temp.first;
		plik >> temp.second;
		temp.first--;
		temp.second--;
		_Polaczenia.push_back(temp);
	}
}

void CPM::zlicz_poprzednikow()
{
	for (int i = 0; i < _Liczba_polaczen; ++i)
	{
		_Zadania[_Polaczenia[i].second].poprzednicy.push_back(_Polaczenia[i].first);
		_Liczba_poprzednikow[_Polaczenia[i].second]++;
	}

	for (int i = 0; i < _Liczba_zadan; ++i)
	{
		if (_Liczba_poprzednikow[i] == 0)
		{
			_Kolejnosc.push_back(i);
		}
	}
}

void CPM::sortuj_topoligicznie()
{
	while (_Kolejnosc.size()>0)
	{
		int currTask = _Kolejnosc[0];
		_Kolejnosc.erase(_Kolejnosc.begin());
		_KolejnoscTopologiczna.push_back(currTask);
		for (int i = 0; i < _Polaczenia.size(); ++i)
		{
			if (_Polaczenia[i].first == currTask)
			{
				_Liczba_poprzednikow[_Polaczenia[i].second]--;
				if (_Liczba_poprzednikow[_Polaczenia[i].second] == 0)
					_Kolejnosc.push_back(_Polaczenia[i].second);
			}
		}
	}

	std::cout << std::endl << "Kolejnosc Topologiczna" << std::endl;

	for (auto it :_KolejnoscTopologiczna)
	{
		std::cout << it+1 << " ";
	}
}

void CPM::es_ef()
{
	for (int i = 0; i < _Liczba_zadan; ++i)
	{
		int currentTask = _KolejnoscTopologiczna[i], newES = 0;
		for (int j = 0; j < _Zadania[currentTask].poprzednicy.size(); ++j)
		{
			if (newES < _Zadania[_Zadania[currentTask].poprzednicy[j]].EF)
				newES = _Zadania[_Zadania[currentTask].poprzednicy[j]].EF;
		}
		_Zadania[currentTask].ES = newES;
		_Zadania[currentTask].EF = newES + _Zadania[currentTask].czas_trwania;
	}
}

void CPM::ls_lf()
{
	
	for (int i = 0; i < _Liczba_zadan; ++i)
	{
		if (_Zadania[i].EF > _Czas_procesu)
			_Czas_procesu = _Zadania[i].EF;
	}

	for (int i = 0; i < _Liczba_zadan; ++i)
	{
		_Zadania[i].LF = _Czas_procesu;
		_Zadania[i].LS = _Zadania[i].LF - _Zadania[i].czas_trwania;
	}

	for (int k = 0; k < _Liczba_zadan; k++)
	{
		for (int j = 0; j < _Liczba_polaczen; ++j)
		{
			int z1 = _Polaczenia[j].second; int z2 = _Polaczenia[j].first;

			if (_Zadania[z2].LF > _Zadania[z1].LS)
			{
				_Zadania[z2].LF = _Zadania[z1].LS;
				_Zadania[z2].LS = _Zadania[z2].LF - _Zadania[z2].czas_trwania;
			}
		}
	}
}

void CPM::sciezka_krytyczna()
{
	std::cout << std::endl << "critical path" << std::endl;
	while (_Sciezka < _Czas_procesu)
	{
		for (int i = 0; i<_Liczba_zadan; i++)
		{
			if (_Zadania[i].ES == _Zadania[i].LS && _Zadania[i].LS == _Sciezka)
			{
				_criticalPath.push_back(i);
				std::cout << i + 1 << " " << _Zadania[i].ES << " " << _Zadania[i].EF << std::endl;
				_Sciezka = _Zadania[i].EF;
			}
		}
	}
}

void CPM::pokaz_wynik()
{
	std::cout << std::endl << "process time " << std::endl << _Czas_procesu;
	std::cout << std::endl << std::endl << "ES EF LS LS" << std::endl;
	for (int i = 0; i < _Liczba_zadan; ++i)
	{
		std::cout << _Zadania[i].ES << " " << _Zadania[i].EF << " " << _Zadania[i].LS << " " << _Zadania[i].LF << std::endl;
	}
	sciezka_krytyczna();
}

void CPM::alg()
{
	for (auto i = 0; i < _Liczba_zadan; ++i)
		_shortenList.push_back( 0);
	for (int i = _criticalPath.size() - 1; i > -1; --i)
	{
		auto newDuration = _Zadania[_criticalPath[i]].najmniejszy_czas;
		auto oldDuration = _Zadania[_criticalPath[i]].czas_trwania;
		auto cost = _Zadania[_criticalPath[i]].koszt_skrocenia;
		while ((oldDuration - newDuration) * cost > _budzet)
			++newDuration;
		_Zadania[_criticalPath[i]].czas_trwania = newDuration;
		_budzet -= (oldDuration - newDuration) * cost;
		_shortenList[_criticalPath[i]] = oldDuration - newDuration;
	}
}
void CPM::resulat()
{
	std::cout << "reduction scheduler time:" << std::endl << maxEf() << std::endl;
	std::cout << "reduction cost: " << std::endl <<_dostepny_budzet - _budzet << std::endl;
	std::cout << "reduction:" << std::endl;
	for (int i = 0; i < _Liczba_zadan; ++i)
		std::cout << _shortenList[i] << " ";
}
int CPM::maxEf()
{
	int maxEf = 0;
	for (int i = 0; i <_Liczba_zadan; ++i) {
		if (_Zadania[i].EF > maxEf)
			maxEf = _Zadania[i].EF;
	}
	return maxEf;
}



int main()
{
	CPM p;
	p.wczytaj_dane();
	p.zlicz_poprzednikow();
	p.sortuj_topoligicznie();
	p.es_ef();
	p.ls_lf();
	p.pokaz_wynik();
	p.alg();
	p.resulat();
	system("pause");
	
}

// ConsoleApplication1.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <cstdlib>
#include <string>
#include <iostream>
#include <ctime> //do losowania
#include <Windows.h> //zawiera cls i sleep
#include <fstream> // pliki txt
#include <conio.h> //zawiera getch()
/*
getch() to funkcja pobierąca klawisz z klawiatury
75-lewo 77-prawo 72-gora 80-dol http://www.lookuptables.com/ebcdic_scancodes.php
enter - odczytujemy jako powrot karetki znak 13 http://www.asciitable.com/
*/
using namespace std;
int px, py; // rozmiar planszy
int li_r = 0;
int koniec = 0;

struct pole {
	int wartosc;
	bool odkryte;
};

pole plansza[10][10];

HANDLE hOut;


//---------------------------------------------//
void kolor(int i);
void generacja();
int menu_glowne(int i);
void wygrana(int k);
void Gra();
void odkryj(int x, int y);
//---------------------------------------------//
int main()
{

	menu_glowne(0);
	return 0;
}
//---------------------------------------------//
void kolor(int i)
{
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (i == 1)
	{ // czerwony
		SetConsoleTextAttribute(hOut, FOREGROUND_RED);
	}
	else if (i == 2)
	{ // zielony
		SetConsoleTextAttribute(hOut, FOREGROUND_GREEN);
	}
	else if (i == 3)
	{ // niebieski
		SetConsoleTextAttribute(hOut, FOREGROUND_BLUE);
	}
	else
	{ // domyslny kolor tekstu w konsoli(biały)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
}
//---------------------------------------------//
int menu_glowne(int i)
{
	char guzik;
	system("cls");

	cout << "MENU:" << endl << "Wybierz opcje(strzalki i enter)" << endl;
	if (i == 0)
		cout << "NOWA GRA  <---" << endl;
	else
		cout << "NOWA GRA" << endl;

	if (i == 1)
		cout << "WYJSCIE  <---" << endl;
	else
		cout << "WYJSCIE" << endl;


	do
		guzik = _getch(); // to _ z przodu bo kompliator poprosił (to prawda)
	while (guzik != 80 && guzik != 72 && guzik != 13); //dlaczego taki warunek http://cpp0x.pl/kursy/Kurs-C++/Obsluga-klawiatury-za-pomoca-conio-h/320

	if (guzik == 80) //w dół
	{
		if (i >= 1)
			i = 0;
		else
			i++;
		menu_glowne(i);
	}
	else if (guzik == 72)
	{
		if (i <= 0)
			i = 1;
		else
			i--;
		menu_glowne(i);
	}
	else if (guzik == 13)
	{
		if (i != 1) //nie wybano wyjscia
		{
			generacja();
			Gra();
			if (koniec == 1)
				return 0;
		}
		if (i == 1) 
		{
			return 0;
		}
	}
}
//---------------------------------------------//
void generacja()
{
	for (int x = 0; x<10; x++)
		for (int y = 0; y<10; y++)
		{
			plansza[x][y].wartosc = 0;
			plansza[x][y].odkryte = false;
		}
	int x, y;
	int ilosc = 10;
	while (ilosc>0)
	{
		x = rand() % 10;
		y = rand() % 10;

		if (plansza[x][y].wartosc != 9)
		{
			if (plansza[x][y].wartosc != 9)
			{
				plansza[x][y].wartosc = 9; //ustawiamy mine

				for (int k = -1; k<2; k++)
					for (int l = -1; l<2; l++)
					{
						if ((x + l)<0 || (y + k)<0) continue; //krawedz
						if ((x + l)>9 || (y + k)>9) continue; //wkrawedz
						if (plansza[x + l][y + k].wartosc == 9) continue; // mina
						plansza[x + l][y + k].wartosc += 1; //zwieksz o 1
					}
			}
			ilosc--;
		}
	}
}
//---------------------------------------------//
void Gra()
{
	char guzik;
	system("cls"); //wyczysc ekran

	for (int i = 0; i<10; i++)
	{
		for (int j = 0; j<10; j++)
		{
			if (j == px && i == py) //pozycja kursora
			{
				kolor(1);
				cout << "~";
				kolor(0);
			}
			else
			{
				if (plansza[j][i].odkryte == true) // pole odkryte
				{
					if (plansza[j][i].wartosc == 0)   //wartosc = 0
						cout << " ";                //wyswietl spacje
					else
						cout << plansza[j][i].wartosc; //wyswietl wartosc 1-8
				}
				if (plansza[j][i].odkryte == false) //pole nie odkryte
					cout << "#";
			}
		}

		cout << endl;
	}
	do
	{
		guzik = _getch(); // pobiera znak tak dlugo az zostana wcisniete strzalki gora/dol/lewo/prawo lub enter/spacja
	} while (guzik != 80 && guzik != 72 && guzik != 75 && guzik != 77 && guzik != 13 && guzik != 32 && guzik != 27);

	if (guzik == 80) //w dół
	{
		if (py == 9)
			py = 0; //zasady tablicy
		else
			py++;
		Gra();
	}
	else if (guzik == 72) // w gore
	{
		if (py == 0)
			py = 9; //zasady tablicy
		else
			py--;
		Gra();
	}
	else if (guzik == 75) // w lewo
	{
		if (px == 0)
			px = 9;
		else
			px--;
		Gra();
	}
	else if (guzik == 77) // w prawo		
	{
		if (px == 9)
			px = 0;
		else
			px++;
		Gra();
	}
	else if (guzik == 27) //ESC
	{
		menu_glowne(0);
	}
	else if (guzik == 32 || guzik == 13)//spacja i enter
		odkryj(px, py);

	if (koniec == 1)
		return;

	int miny = 0;
	for (int i = 0; i<10; i++)
	{
		for (int j = 0; j<10; j++)
		{
			if (plansza[j][i].odkryte == false)
				miny++;
		}
	}
	if (miny != 10) Gra();
	else wygrana(1);
}

void odkryj(int x, int y)
{
	if (x<0 || x>9) return; // poza tablicą wyjście
	if (y<0 || y>9) return; // poza tablicą wyjście
	if (plansza[x][y].odkryte == true) return;  // już odkryte wyjście

	if (plansza[x][y].wartosc >= 9) 
		wygrana(0);
		

	if (plansza[x][y].wartosc != 9 && plansza[x][y].odkryte == false)
		plansza[x][y].odkryte = true;   // odkryj!

	if (plansza[x][y].wartosc != 0) return; // wartość > 0 wyjście

											//wywołanie funkcji dla każdego sąsiada
	odkryj(x - 1, y - 1);
	odkryj(x - 1, y);
	odkryj(x - 1, y + 1);
	odkryj(x + 1, y - 1);
	odkryj(x + 1, y);
	odkryj(x + 1, y + 1);
	odkryj(x, y - 1);
	odkryj(x, y);
	odkryj(x, y + 1);
}

void wygrana(int k)
{
	if (k == 1)
	cout << "Gra zakonczona. Wygrales!";
	if (k == 0)
		cout << "Gra zakonczona. Przegrales!";
	koniec++;
	Sleep(1500);
}

/*
Zródła:
Tablice znaków: http://www.lookuptables.com/ebcdic_scancodes.php
getch(): http://cpp0x.pl/kursy/Kurs-C++/Obsluga-klawiatury-za-pomoca-conio-h/320
Zmiana koloru tekstu: http://cpp0x.pl/kursy/Kurs-WinAPI-C++/Roznosci/Kolory-w-konsoli/374
*/
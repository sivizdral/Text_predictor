
#define _CRT_SECURE_NO_WARNINGS
#include <IOSTREAM>
using namespace std;


#include "TrieStablo.h"
#include <filesystem>
#include <string>
#include <fstream>

namespace fs = std::filesystem;


void trazi(Stablo &s, const char *kljuc)
{
      cout << "Trazim kljuc: " << kljuc << endl;
      char *rez = s.dohvatiInfo(kljuc);
      if( rez )
            cout << "<" << kljuc << ">" << " : " << rez << endl << endl;
      else
            cout << "Ne postoji kljuc: " << kljuc << endl << endl;
}

void main()
{
    TrieStablo* stablo = new TrieStablo;
    int izbor = 1, dat_broj, izbor2 = 2;
    string dat, putanja, rec;
    while (izbor) {
        cout << "Odaberite jednu od opcija za rad:" << endl << "1. Stvaranje recnika";
        cout << endl << "2. Unosenje reci za pretragu" << endl << "0. Prekid rada" << endl;

        cin >> izbor;
        switch (izbor) {
        case 1:
            cout << "Ako zelite da se recnik kreira od jedne datoteke, unesite 1, inace unesite 2" << endl;
            cin >> dat_broj;
            switch (dat_broj) {
            case 1:
                cout << "Unesite naziv datoteke iz koje se citaju podaci: " << endl;
                cin >> dat;
                stablo->citajIzDatoteke(dat);
                stablo->ispisiStatistiku(cout);
                cout << endl;
                break;
            case 2:
                cout << "Unesite putanju do skupa datoteka: " << endl;
                cin >> putanja;
                stablo->citajIzSkupaDatoteka(putanja);
                stablo->ispisiStatistiku(cout);
                cout << endl;
                break;
            }
            break;
        case 2:
            while (izbor2) {
                cout << "1. Unesite novi string za pretragu" << endl;
                cout << "0. Zavrsite sa pretragom" << endl;
                cin >> izbor2;
                if (!izbor2) break;
                cout << "Unesite string koji zelite: ";
                cin >> rec;
                cout << endl;
                stablo->predikcija(rec.c_str());
                cout << endl;
            }
            break;
        case 0:
            break;
        }
    }
    delete stablo;
}
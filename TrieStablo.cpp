// TrieStablo.cpp: implementation of the TrieStablo class.
//
//////////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_WARNINGS
#include "TrieStablo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TrieStablo::TrieCvor::TrieCvor(TrieStablo::TrieCvor *roditelj)
: m_roditelj(roditelj),
  m_info(0),
  m_brojPodstabala(0)
{
      for(int i = 0; i < 255; i++)
            m_niz_pokazivaca[i] = 0;
      verovatnoca = 0;
}

TrieStablo::TrieCvor::~TrieCvor()
{
      if( m_info )
            delete []m_info;

      for(int i = 0; i < 255; i++)
            if( m_niz_pokazivaca[i] )
                  delete m_niz_pokazivaca[i];
}


TrieStablo::TrieCvor *TrieStablo::TrieCvor::pronadjiPodstablo(unsigned char deoKljuca)
{
      return m_niz_pokazivaca[deoKljuca];
}

void TrieStablo::TrieCvor::dodajPodstablo(char deoKljuca, TrieStablo::TrieCvor *cvor)
{
      if( ! m_niz_pokazivaca[deoKljuca] )
      {
            m_niz_pokazivaca[deoKljuca] = cvor;
            m_brojPodstabala++;
      }
}

void TrieStablo::TrieCvor::ukloniPodstablo(char deoKljuca)
{
      if( m_niz_pokazivaca[deoKljuca] )
      {
            m_niz_pokazivaca[deoKljuca] = 0;
            m_brojPodstabala--;
      }
}

void TrieStablo::TrieCvor::postaviInfo(const char *tekst)
{
      if( m_info )
      {
            delete []m_info;
            m_info = 0;
      }

      if( tekst )
      {
            m_info = new char[ strlen(tekst) + 1 ];
            strcpy(m_info, tekst);
      }
}

char *TrieStablo::TrieCvor::dohvatiInfo()
{
      return m_info;
}


void TrieStablo::TrieCvor::poseti(ostream &it)
{
      if( m_info )
            it << m_info << endl;

      for(int i = 0; i < 255; i++)
            if( m_niz_pokazivaca[i] )
                  m_niz_pokazivaca[i]->poseti(it);
}



//---------------------------------------------------------------


TrieStablo::TrieCvor *TrieStablo::pronadjiCvor(const char *kljuc)
{
      if( ! m_koren )
            return 0;

      TrieCvor *tek = m_koren;

      for( int i = 0; i < strlen(kljuc) && tek; i++ )
            tek = tek->pronadjiPodstablo( kljuc[i] );

      return tek;
}



TrieStablo::Elem* TrieStablo::pronadjiMaks(TrieCvor* koren)
{
    Elem* reci = new Elem[3];
    TrieCvor* sled = nullptr;
    queue<TrieCvor*> red;
    red.push(koren);
    while (!red.empty()) {
        sled = red.front();
        red.pop();
        int v = sled->dohvatiVerovatnocu();
        if (v > reci[0].maks) {
            reci[2] = reci[1];
            reci[1] = reci[0];
            reci[0].maks = v;
            reci[0].pok = sled;
        }
        else if (v > reci[1].maks) {
            reci[2] = reci[1];
            reci[1].maks = v;
            reci[1].pok = sled;
        }
        else if (v > reci[2].maks) {
            reci[2].maks = v;
            reci[2].pok = sled;
        }

        for (int i = 0; i < 255; i++) {
            if (sled->pronadjiPodstablo(i) != 0) red.push(sled->pronadjiPodstablo(i));
        }
        continue;
    }
    return reci;
}

TrieStablo::Elem* TrieStablo::ubaciRec(Elem* reci, TrieCvor* cvor)
{
    for (int i = 0; i < 3; i++) if (reci[i].pok == cvor) return reci;
    int v = cvor->dohvatiVerovatnocu();
    if (v > reci[0].maks) {
        reci[2] = reci[1];
        reci[1] = reci[0];
        reci[0].maks = v;
        reci[0].pok = cvor;
    }
    else if (v > reci[1].maks) {
        reci[2] = reci[1];
        reci[1].maks = v;
        reci[1].pok = cvor;
    }
    else if (v > reci[2].maks) {
        reci[2].maks = v;
        reci[2].pok = cvor;
    }
    return reci;
}

TrieStablo::Elem* TrieStablo::iskombinujReci(Elem* reci1, Elem* reci2)
{
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (reci1[i].pok == reci2[j].pok) {
                if (j == 0) {
                    reci2[0] = reci2[1];
                    reci2[1] = reci2[2];
                    reci2[2].pok = nullptr;
                    reci2[2].maks = 0;
                }
                else if (j == 1) {
                    reci2[1] = reci2[2];
                    reci2[2].pok = nullptr;
                    reci2[2].maks = 0;
                }
                else {
                    reci2[2].pok = nullptr;
                    reci2[2].maks = 0;
                }
            }
        }
    }
    Elem* reci = new Elem[3];
    int i1 = 0, i2 = 0;
    for (int i = 0; i < 3; i++) {
        if (reci1[i1].maks > reci2[i2].maks) {
            reci[i] = reci1[i1];
            i1++;
        }
        else {
            reci[i] = reci2[i2];
            i2++;
        }
    }
    return reci;
}


TrieStablo::TrieStablo()
: m_koren(0)
{
}

TrieStablo::~TrieStablo()
{
      if( m_koren )
            delete m_koren;
}


bool TrieStablo::umetni(const char* kljuc, const char* podaci)
{
    if (!m_koren)
    {
        m_koren = new TrieCvor(0);
        m_brCvorova++;
    }

    TrieCvor* tek = m_koren;
    TrieCvor* sledeci = 0;

    for (int i = 0; i < strlen(kljuc); i++)
    {

        sledeci = tek->pronadjiPodstablo(kljuc[i]);

        if (!sledeci)
        {
            sledeci = new TrieCvor(tek);
            tek->dodajPodstablo(kljuc[i], sledeci);
            m_brCvorova++;
        }

        tek = sledeci;
    }

    bool rezultat = true;  // ako je kljuc prvi put umetnut, onda true
    if (sledeci->dohvatiInfo())
        rezultat = false;
    else
        m_brKljuceva++;

    sledeci->povecajVerovatnocu();

    sledeci->postaviInfo(podaci);

    return rezultat;
}

bool TrieStablo::obrisi(const char *kljuc)
{
      if( ! m_koren )
            return false;

      TrieCvor *tek = pronadjiCvor(kljuc);
      if( ! tek )
            return false;

      tek->postaviInfo(0);
      int i = strlen(kljuc)-1;
      
      while( i >= 0 && tek && tek->brojPodstabala() == 0 && !tek->dohvatiInfo() )
      {
      TrieCvor *roditelj = tek->roditelj();

            delete tek;
            m_brCvorova--;

            if( roditelj )
                  roditelj->ukloniPodstablo( kljuc[i--] );
            else
                  m_koren = 0;
            tek = roditelj;
      }

      m_brKljuceva--;
      return true;
}

void TrieStablo::predikcija(const char* kljuc)
{
    Elem* reci = new Elem[3];
    Elem* reci_pomocni = new Elem[3];
    char susedi[26][5] = { {'q','w','s','z',0},  \
   {'v', 'h', 'n', 0, 0}, { 'x','f','v',0,0 }, { 'e','r','f','x','s' }, {'w','s','d','r'},\
   {'r', 't', 'g', 'c', 'd'}, { 't','y','h','v','f' }, { 'y','u','j','b','g' }, {'u','j','k','o',0},\
   {'u', 'i', 'k', 'n', 'h'}, { 'i','o','l','m','j' }, { 'p','o','k',0,0 }, {'n','k',0,0,0},\
   {'b', 'j', 'm', 0, 0}, { 'i','k','l','p',0 }, { 'o','l',0,0,0 }, {'w','a',0,0,0},\
   {'e', 'd', 'f', 't', 0}, { 'a','w','e','d','z' }, { 'r','f','g','y',0 }, {'y','j','h','i',0},\
   {'c', 'g', 'b', 0, 0}, { 'q','a','s','e',0 }, { 'z','d','c',0,0 }, { 't','g','h','u',0 }, { 's','x',0,0,0 } };
    TrieCvor* cvor = pronadjiCvor(kljuc);
    if (cvor && cvor->dohvatiInfo()) {
        cout << "1. " << cvor->dohvatiInfo() << endl;
        cvor->povecajVerovatnocu();
        return;
        //reci = pronadjiMaks(cvor);
    }
    else if (cvor) {
        reci = pronadjiMaks(cvor);
    }
    int broj = 1;
    /*if (!reci[0].pok) broj = 3;
    else if (!reci[1].pok) broj = 2;
    else if (!reci[2].pok) broj = 1;*/
    if (broj || !cvor) {
        for (int i = 0; i < strlen(kljuc); i++) {
            char* novi = new char[strlen(kljuc) + 1];
            for (int k = 0; k <= strlen(kljuc); k++) novi[k] = kljuc[k];
            for (int j = 0; j < 5; j++) {
                if (susedi[kljuc[i] - 'a'][j]) {
                    novi[i] = susedi[kljuc[i] - 'a'][j];
                    cvor = pronadjiCvor(novi);
                    if (cvor && cvor->dohvatiInfo()) {
                        reci = ubaciRec(reci, cvor);
                        reci_pomocni = pronadjiMaks(cvor);
                        reci = iskombinujReci(reci, reci_pomocni);
                    }
                    else if (cvor) {
                        reci_pomocni = pronadjiMaks(cvor);
                        reci = iskombinujReci(reci, reci_pomocni);
                    }
                    if (!cvor) {
                        for (int k = 0; k < strlen(kljuc); k++) {
                            char* novi2 = new char[strlen(kljuc) + 1];
                            for (int p = 0; p <= strlen(kljuc); p++) novi2[p] = novi[p];
                            if (k != i) {
                                for (int p = 0; p < 5; p++) {
                                    if (susedi[kljuc[k] - 'a'][p]) {
                                        novi2[k] = susedi[kljuc[k] - 'a'][p];
                                        cvor = pronadjiCvor(novi2);
                                        if (cvor && cvor->dohvatiInfo()) {
                                            reci = ubaciRec(reci, cvor);
                                            reci_pomocni = pronadjiMaks(cvor);
                                            reci = iskombinujReci(reci, reci_pomocni);
                                        }
                                        else if (cvor) {
                                            reci_pomocni = pronadjiMaks(cvor);
                                            reci = iskombinujReci(reci, reci_pomocni);
                                        }
                                        if (!cvor) {
                                            for (int q = 0; q < strlen(kljuc); q++) {
                                                char* novi3 = new char[strlen(kljuc) + 1];
                                                for (int t = 0; t <= strlen(kljuc); t++) novi3[t] = novi2[t];
                                                if (q != k && q != i) {
                                                    for (int t = 0; t < 5; t++) {
                                                        if (susedi[kljuc[q] - 'a'][t]) {
                                                            novi3[q] = susedi[kljuc[q] - 'a'][t];
                                                            cvor = pronadjiCvor(novi3);
                                                            if (cvor && cvor->dohvatiInfo()) {
                                                                reci = ubaciRec(reci, cvor);
                                                                reci_pomocni = pronadjiMaks(cvor);
                                                                reci = iskombinujReci(reci, reci_pomocni);
                                                            }
                                                            else if (cvor) {
                                                                reci_pomocni = pronadjiMaks(cvor);
                                                                reci = iskombinujReci(reci, reci_pomocni);
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    cout << "1. " << reci[0].pok->dohvatiInfo() << " " << reci[0].pok->dohvatiVerovatnocu() << endl;
    cout << "2. " << reci[1].pok->dohvatiInfo() << " " << reci[1].pok->dohvatiVerovatnocu() << endl;
    cout << "3. " << reci[2].pok->dohvatiInfo() << " " << reci[2].pok->dohvatiVerovatnocu() << endl;
}

void TrieStablo::obidji(ostream &it)
{
      if( m_koren )
            m_koren->poseti(it);
      else
            it << "Stablo je prazno." << endl;
}


char *TrieStablo::dohvatiInfo(const char *kljuc)
{
/*
      if( ! m_koren )
            return 0;

      TrieCvor *cvor = m_koren;
      int i = 0, maxi = strlen(kljuc);

      while( cvor && i < maxi )
            cvor = cvor->pronadjiPodstablo( kljuc[i++] );
*/
      TrieCvor *cvor = pronadjiCvor(kljuc);

      if( cvor )
            return cvor->dohvatiInfo();

      return 0;
}

void TrieStablo::citajIzDatoteke(string ime)
{
    ifstream dat;
    string rec;
    dat.open(ime);
    while (dat >> rec) {
        bool prava_rec = true;
        for (int i = 0; i < rec.size(); i++) {
            if (!((rec[i] >= 'A' && rec[i] <= 'Z') || (rec[i] >= 'a' && rec[i] <= 'z'))) {
                prava_rec = false;
                break;
            }
            if (rec[i] >= 'A' && rec[i] <= 'Z') rec[i] = rec[i] - 'A' + 'a';
        }
        const char* crec = rec.c_str();
        if (prava_rec) umetni(crec, crec);
    }
}

void TrieStablo::citajIzSkupaDatoteka(string putanja)
{
    int br = 0;
    for (const auto& ulaz : fs::directory_iterator(putanja)) {
        ifstream dat;
        string ime = ulaz.path().string();
        dat.open(ime);
        string rec;
        while (dat >> rec) {
            bool prava_rec = true;
            for (int i = 0; i < rec.size(); i++) {
                if (!((rec[i] >= 'A' && rec[i] <= 'Z') || (rec[i] >= 'a' && rec[i] <= 'z'))) {
                    prava_rec = false;
                    break;
                }
                if (rec[i] >= 'A' && rec[i] <= 'Z') rec[i] = rec[i] - 'A' + 'a';
            }
            const char* crec = rec.c_str();
            if (prava_rec) umetni(crec, crec);
        }
    }
}

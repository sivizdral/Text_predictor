// TrieStablo.h: interface for the TrieStablo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRIESTABLO_H__EA14736F_A30C_4DA9_A044_C865B9328099__INCLUDED_)
#define AFX_TRIESTABLO_H__EA14736F_A30C_4DA9_A044_C865B9328099__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Stablo.h"
#include <queue>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

class TrieStablo : public Stablo  
{
protected:

      class TrieCvor
      {
	      TrieCvor	*m_niz_pokazivaca[255];
            TrieCvor    *m_roditelj;
	      char		*m_info;
            int         m_brojPodstabala;
            int verovatnoca;

            TrieCvor(const TrieCvor &) { }
            TrieCvor &operator=(const TrieCvor &) { }

      public:
	      TrieCvor(TrieCvor *roditelj);

	      virtual ~TrieCvor();

            TrieCvor *roditelj() 
            {
                  return m_roditelj;
            }

	      TrieCvor *pronadjiPodstablo(unsigned char deoKljuca);
	      void dodajPodstablo(char deoKljuca, TrieCvor *cvor);
            void ukloniPodstablo(char deoKljuca);

            int brojPodstabala() const { return m_brojPodstabala; }

            char *dohvatiInfo();
            void postaviInfo(const char *tekst);

            void poseti(ostream &it);
            void povecajVerovatnocu() { verovatnoca++; }
            int dohvatiVerovatnocu() const { return verovatnoca; }
      };


      TrieCvor    *m_koren;

      struct Elem {
          int maks;
          TrieCvor* pok;
          Elem(int m = 0, TrieCvor* p = nullptr) : maks(m), pok(p) {}
      };

      TrieStablo(const TrieStablo &) { }
      TrieStablo &operator=(const TrieStablo &) { }

      TrieCvor    *pronadjiCvor(const char *kljuc);   // pronalazi cvor koji sadrzi dati kljuc

      Elem* pronadjiMaks(TrieCvor* koren);

      Elem* ubaciRec(Elem* reci, TrieCvor* cvor);

      Elem* iskombinujReci(Elem* reci1, Elem* reci2);

public:
	TrieStablo();
	virtual ~TrieStablo();

	virtual bool umetni(const char *kljuc, const char* podaci);
	virtual bool obrisi(const char *kljuc);

    virtual void pretrazi(const char* kljuc) {
        TrieCvor* cvor = pronadjiCvor(kljuc);
        if (cvor) cout << "Rec je nadjena! " << cvor->dohvatiVerovatnocu() << endl;
        else cout << "Rec nije nadjena!" << endl;
    }

    void predikcija(const char* kljuc);

      virtual void obidji(ostream &it);

      virtual char *dohvatiInfo(const char *kljuc);

      void citajIzDatoteke(string ime);

      void citajIzSkupaDatoteka(string putanja);
};

#endif // !defined(AFX_TRIESTABLO_H__EA14736F_A30C_4DA9_A044_C865B9328099__INCLUDED_)

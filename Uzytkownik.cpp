#include "Uzytkownik.h"

using namespace std;

//Haslp
Haslo::Haslo(const char* haslo)
{
    setHaslo(haslo);
}

void Haslo::setHaslo(const char* haslo)
{
    assert(haslo != nullptr);

    size_t dlugosc = strlen(haslo);
    assert(dlugosc >= 8 && dlugosc <= 12);

    bool maDuzaLitere = false;
    bool maMalaLitere = false;
    bool maCyfre = false;

    for (size_t i = 0; i < dlugosc; ++i)
    {
        if (isupper(haslo[i])) maDuzaLitere = true;
        if (islower(haslo[i])) maMalaLitere = true;
        if (isdigit(haslo[i])) maCyfre = true;
    }

    assert(maDuzaLitere);
    assert(maMalaLitere);
    assert(maCyfre);

    strncpy(this->m_haselko, haslo, dlugosc);
    this->m_haselko[dlugosc] = '\0';
}

const char* Haslo::getHaslo() const
{
    return m_haselko;
}


//Uzytkownik
void Uzytkownik::setNazweUzytkownika(const char* nazwa)
{
    assert(nazwa != nullptr);

    size_t dlugosc_nazwy = strlen(nazwa);
    assert(dlugosc_nazwy > 0 && dlugosc_nazwy <= 20);
    assert(!isdigit(nazwa[0]));

    for (size_t i = 0; i < dlugosc_nazwy; ++i)
    {
        assert(!isspace(nazwa[i]));
    }

    strncpy(this->m_nazwa_uzytkownika, nazwa, dlugosc_nazwy);
    this->m_nazwa_uzytkownika[dlugosc_nazwy] = '\0';
}

const char* Uzytkownik::getNazwaUzytkownika() const
{
    return m_nazwa_uzytkownika;
}

void Uzytkownik::setCzasUzytkowania(double czas)
{
    assert(czas > 0);
    this->m_czas_uzytkowania = czas;
}

void Uzytkownik::setHaselko(Haslo& haslo)
{
    this->m_haslo.setHaslo(haslo.getHaslo());
}

void Uzytkownik::zaloguj(const char* podane_haslo)
{
    if (podane_haslo != nullptr && strcmp(this->m_haslo.getHaslo(), podane_haslo) == 0)
    {

        if (this->m_uprawnienia == Uprawnienia::zwykle)
        {
            srand(time(NULL));
            double min = 1.0;
            double max = 10.0;

            double random = min + (double)rand() / RAND_MAX * (max - min);
            this->m_czas_uzytkowania += random;
        }
    }
}

void Uzytkownik::setAll(const char* nazwa, Uprawnienia upr, double czas, Uzytkownik* wspol, Haslo& haslo)
{

    setNazweUzytkownika(nazwa);
    this->m_uprawnienia = upr;
    setCzasUzytkowania(czas);
    this->m_wspolpracownik = wspol;

    setHaselko(haslo);
}

Uzytkownik::Uzytkownik(const char* nazwa, Uprawnienia upr, double czas, Uzytkownik* wspol, Haslo& haslo)
    : m_uprawnienia(upr), m_wspolpracownik(wspol), m_haslo(haslo.getHaslo())
{
    this->setNazweUzytkownika(nazwa);
    this->setCzasUzytkowania(czas);
}

Uzytkownik* Uzytkownik::instancja_wzorcowa = nullptr;

void Uzytkownik::setInstancjaWzorcowa(Uzytkownik* wzorzec)
{
    instancja_wzorcowa = wzorzec;
}

Uzytkownik::Uzytkownik()
    : m_uprawnienia(instancja_wzorcowa ? instancja_wzorcowa->m_uprawnienia : Uprawnienia::zwykle),
    m_wspolpracownik(nullptr),
    m_haslo(instancja_wzorcowa ? instancja_wzorcowa->m_haslo.getHaslo() : "Wzorzec123")
{
    if (instancja_wzorcowa) {
        this->setNazweUzytkownika(instancja_wzorcowa->getNazwaUzytkownika());
        this->setCzasUzytkowania(instancja_wzorcowa->m_czas_uzytkowania);
    }
    else {
        this->setNazweUzytkownika("Usereczek");
        this->setCzasUzytkowania(1.0);
    }
}

Uzytkownik::~Uzytkownik()
{
    if (instancja_wzorcowa == this) {
        instancja_wzorcowa = nullptr;
    }
}

Uprawnienia Uzytkownik::getUprawnienia() const
{
    return m_uprawnienia;
}

double Uzytkownik::getCzasUzytkowania() const
{
    return m_czas_uzytkowania;
}

Uzytkownik* Uzytkownik::getWspolpracownik() const
{
    return m_wspolpracownik;
}

Haslo& Uzytkownik::getHasloObiekt()
{
    return m_haslo;
}

//UzytkownikTymczasowy
void UzytkownikTymczasowy::setLimitLogowan(int limit)
{
    if (limit < 0)
    {
        throw invalid_argument("Limit logowan nie moze byc liczba ujemna!!!!!");
    }
    this->m_limit_logowan = limit;
}


void UzytkownikTymczasowy::zaloguj(const char* podane_haslo)
{
    if (this->m_limit_logowan == 0)
    {
        throw runtime_error("Wyczerpano limit logowan!!!!");
    }

    this->m_limit_logowan--;

    Uzytkownik::zaloguj(podane_haslo);
}

UzytkownikTymczasowy::UzytkownikTymczasowy(Uzytkownik& instancja_bazowa, int limit)
    : Uzytkownik(instancja_bazowa.getNazwaUzytkownika(), instancja_bazowa.getUprawnienia(), instancja_bazowa.getCzasUzytkowania(), instancja_bazowa.getWspolpracownik(), instancja_bazowa.getHasloObiekt()
    )
{
    this->setLimitLogowan(limit);
}
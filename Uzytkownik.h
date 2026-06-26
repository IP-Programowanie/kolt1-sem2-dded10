#pragma once
#include <cassert>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

using namespace std;

enum class Uprawnienia
{
    administracyjne = 0,
    zwykle = 1
};

class Haslo
{
private:
    char m_haselko[13];

public:
    Haslo() = delete;
    Haslo(const char* haslo);

    void setHaslo(const char* haslo);
    const char* getHaslo() const;
};

class Uzytkownik
{
private:
    char m_nazwa_uzytkownika[21];
    Uprawnienia m_uprawnienia;
    double m_czas_uzytkowania;
    Uzytkownik* m_wspolpracownik;
    Haslo m_haslo;

    static Uzytkownik* instancja_wzorcowa;

public:
    //Zasada dry dla nazwy
    void setNazweUzytkownika(const char* nazwa);
    const char* getNazwaUzytkownika() const;
    void setCzasUzytkowania(double czas);
    void setHaselko(Haslo& haslo);
    virtual void zaloguj(const char* podane_haslo);
    void setAll(const char* nazwa, Uprawnienia upr, double czas, Uzytkownik* wspol, Haslo& haslo);

    //dodaem by odczytac wartosci w dziedziczeniu
    Uprawnienia getUprawnienia() const;
    double getCzasUzytkowania() const;
    Uzytkownik* getWspolpracownik() const;
    Haslo& getHasloObiekt();

    //Uzytkownik() = delete; //by³o w poleceiu ale potem musze zasto¹piæ domyœlnym
    Uzytkownik();
    Uzytkownik(const Uzytkownik& innaInstancja) = delete;
    Uzytkownik(Uzytkownik&& innaInstancja) = delete;
    Uzytkownik& operator=(const Uzytkownik& innaInstancja) = delete;
    Uzytkownik& operator=(Uzytkownik&& innaInstancja) = delete;

    static void setInstancjaWzorcowa(Uzytkownik* wzorzec);

    Uzytkownik(const char* nazwa, Uprawnienia upr, double czas, Uzytkownik* wspol, Haslo& haslo);

    virtual ~Uzytkownik();
};

class UzytkownikTymczasowy : public Uzytkownik
{
private:
    int m_limit_logowan;

protected:
    using Uzytkownik::setCzasUzytkowania;

public:
    void setLimitLogowan(int limit);
    void zaloguj(const char* podane_haslo) override;

    UzytkownikTymczasowy() = delete;
    UzytkownikTymczasowy(const UzytkownikTymczasowy& innaInstancja) = delete;
    UzytkownikTymczasowy(UzytkownikTymczasowy&& innaInstancja) = delete;
    UzytkownikTymczasowy& operator=(const UzytkownikTymczasowy& innaInstancja) = delete;
    UzytkownikTymczasowy& operator=(UzytkownikTymczasowy&& innaInstancja) = delete;

    UzytkownikTymczasowy(Uzytkownik& instancja_bazowa, int limit);

    ~UzytkownikTymczasowy() override = default;
};
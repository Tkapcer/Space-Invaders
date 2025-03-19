#ifndef PROJEKT_GRACZ_H
#define PROJEKT_GRACZ_H
#include <SFML/Graphics.hpp>
#include <memory>
#include <cmath>
#include <random>
#include "Global.h"

class Obiekt {
protected:
    bool aktywny;
    float x;
    float y;

    sf::RenderWindow* okno;

    sf::Sprite sprite;
    sf::Texture tekstura;

public:
    Obiekt(sf::RenderWindow &okno, const std::string &sciezka);

    virtual bool GetAktywny();
    virtual void SetAktywny(bool status);
    virtual void Rysuj();
    virtual sf::FloatRect GetGlobalBounds();

};

class Pocisk : public Obiekt {
    sf::Vector2f predkosc;

public:
    Pocisk(sf::RenderWindow &okno, const std::string &sciezka, sf::Vector2f predkosc, float x, float y);
    void update();
};

class Gracz : public Obiekt {
    int zycia;
    int punkty;
    std::vector<Pocisk> pociski;

    sf::Text wyswietlanie_punktow;
    sf::Text wyswietlanie_zyc;
    sf::Font czcionka;

    sf::Clock zegar;
    sf::Time przerwa;

public:
    Gracz(sf::RenderWindow &okno, const std::string &sciezka);
    void RuchPrawo();
    void RuchLewo();
    void Strzal();
    void UpdatePociskow();
    std::vector<Pocisk>* GetPociski();
    void Update(std::vector<Pocisk> &pociski);
    void DodajPunkt();
    int GetPunkty();
    void Rysuj() override;
};

class Obcy : public Obiekt {
public:
    Obcy(sf::RenderWindow &okno, const std::string &sciezka, float x, float y);
    void Move(sf::Vector2f &predkosc);
};

class MenadzerObcych {
    std::vector<std::vector<Obcy>> Przeciwnicy;
    sf::RenderWindow* okno;
    sf::Vector2f predkosc;

    sf::Clock zegar_ruchu;
    sf::Time przerwa_ruchu = PRZERWA_RUCHU_OBCEGO;

    sf::Clock zegar_strzlu;
    sf::Time przerwa_strzalu = PRZERWA_STRZALU_OBCEGO;

    std::vector<Pocisk> pociski;

    float poziom;

public:
    MenadzerObcych(sf::RenderWindow &okno, sf::Vector2f wymiary_obcego, float poziom);
    void UpdateObcych(std::vector<Pocisk> &pociski, Gracz &gracz);
    void UpdatePociskow();
    std::vector<Pocisk>* GetPociski();
    bool KoniecPoziomu();
    void Rysuj();
};


#endif //PROJEKT_GRACZ_H

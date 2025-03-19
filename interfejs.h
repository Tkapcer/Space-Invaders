#ifndef PROJEKT_INTERFEJS_H
#define PROJEKT_INTERFEJS_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <filesystem>
#include <regex>
#include <fstream>
#include <map>
#include <sstream>
#include <ranges>
#include <thread>
#include <future>
#include "Global.h"
#include "Gracz.h"

class Menu;

class Pozycja {
protected:
    bool aktywny;
    sf::Sprite sprite;
    sf::Texture tekstura;
    int kolejnosc;
    sf::RenderWindow* okno;

    bool pierwsze_wywolanie;
public:
    Pozycja(std::string sciezka, sf::Vector2f wspolzedne, int kolejnosc, sf::RenderWindow &okno);
    void Rysuj(bool najechane);
    void SetAktywny(bool status);
    bool GetAktywny();
    sf::FloatRect GetGlobalBounds();
    virtual void Wykonaj(sf::Event &event, Menu &menu) = 0;
};

class Tytul : public Pozycja {
public:
    Tytul(std::string sciezka, sf::Vector2f wspolzedne, int kolejnosc, sf::RenderWindow &okno)
    : Pozycja(sciezka, wspolzedne, kolejnosc, okno) {};
    void Wykonaj(sf::Event &event, Menu &menu) override;
};

class Graj : public Pozycja {
    std::unique_ptr<Gracz> gracz;
    std::unique_ptr<MenadzerObcych> menadzer;

    float poziom = 1;
    int punkty;
public:
    Graj(std::string sciezka, sf::Vector2f wspolzedne, int kolejnosc, sf::RenderWindow &okno)
    : Pozycja(sciezka, wspolzedne, kolejnosc, okno) {};
    void Wykonaj(sf::Event &event, Menu &menu) override;
};

class TextBox {
    sf::Text text;
    sf::Font czciokna;
    std::string Nick;
    bool klawisz_przetwarzany;

public:
    TextBox(sf::Vector2f pozycja);

    void HandleEvent(const sf::Event& event, sf::RenderWindow& okno);
    void Rysuj(sf::RenderWindow& window);
    void Reset();
    std::string GetText();

    sf::RectangleShape box;
};

class Logowanie : public Pozycja {
    sf::Text naglowek;
    sf::Font czciokna;

    std::string tekst_naglowka;

    std::unique_ptr<TextBox> tekstbox;

public:
    Logowanie(std::string sciezka, sf::Vector2f wspolzedne, int kolejnosc, sf::RenderWindow &okno)
            : Pozycja(sciezka, wspolzedne, kolejnosc, okno) {};
    void Wykonaj(sf::Event &event, Menu &menu) override;
};

class Rekordy : public Pozycja {
    sf::Text tekstbox;
    sf::Font czcionka;
    std::string tekst;
public:
    Rekordy(std::string sciezka, sf::Vector2f wspolzedne, int kolejnosc, sf::RenderWindow &okno)
    : Pozycja(sciezka, wspolzedne, kolejnosc, okno) {}
    void Wykonaj(sf::Event &event, Menu &menu) override;
};

class Wyjscie : public Pozycja {
public:
    Wyjscie(std::string sciezka, sf::Vector2f wspolzedne, int kolejnosc, sf::RenderWindow &okno)
    : Pozycja(sciezka, wspolzedne, kolejnosc, okno) {}
    void Wykonaj(sf::Event &event, Menu &menu) override;
};

class Menu {
    std::vector<sf::Texture> tekstury;

    std::vector<std::shared_ptr<Pozycja>> pozycje;

    int index_wybranej_pozycji;

    sf::RenderWindow* okno;
    sf::Vector2f wspolrzedne;

    bool wyswieltaj;

    std::string nick;

    std::map<std::string, int> rekordy;

public:
    Menu(sf::RenderWindow &okno);
    void Update();
    void Mysz(sf::Vector2<int> pozycja_myszy, sf::Event &event);
    std::vector<std::shared_ptr<Pozycja>> GetPozycje();
    std::map<std::string, int> GetRekordy();
    std::string GetNick();
    void SetNick(std::string nick);
    void DodajRekord(std::pair<std::string, int> rekord);
};

#endif //PROJEKT_INTERFEJS_H

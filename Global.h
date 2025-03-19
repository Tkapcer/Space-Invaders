#ifndef PROJEKT_GLOBAL_H
#define PROJEKT_GLOBAL_H
#include <SFML/Graphics.hpp>

const std::string REKORDY = "../rekordy.txt";
const std::string TEKSTURY = "../tekstury/";
const std::string TYTUL = TEKSTURY + "menu/tytul.png";
const std::string GRAJ = TEKSTURY + "menu/graj.png";
const std::string LOGOWANIE = TEKSTURY + "menu/logowanie.png";
const std::string REKORDY_SIEZKA = TEKSTURY + "menu/rekordy.png";
const std::string WYJSCIE = TEKSTURY + "menu/wyjscie.png";
const std::vector<std::string> TEKSTURY_MENU = {TYTUL, GRAJ, LOGOWANIE, REKORDY_SIEZKA, WYJSCIE};
const std::string POCISK = "pocisk.png";
const std::string STATEK = "statek.png";
const std::string OBCY = "obcy.png";
const std::string CONSOLAS = TEKSTURY + "consola.ttf";

const sf::Vector2f WYMIARY_OBCEGO = {100, 100};
const sf::Vector2f PREDKOSC_OBCEGO_LEWO = {-0.5, 0};
const sf::Vector2f PREDKOSC_OBCEGO_PRAWO = {0.5, 0};
const sf::Vector2f PREDKOSC_OBCEGO_DOL = {0, 100};
const sf::Vector2f PREDKOSC_POCISKU = {0, -0.3};
const sf::Vector2f PREDKOSC_POCISKU_OBCEGO = {0, 0.2};

const sf::Time PRZERWA_RUCHU_OBCEGO = sf::milliseconds(10);
const sf::Time PRZERWA_STRZALU_GRACZA = sf::seconds(1);
const sf::Time PRZERWA_STRZALU_OBCEGO = sf::seconds(1);

const int ZYCIA_GRACZA = 3;

#endif //PROJEKT_GLOBAL_H

#include <iostream>
#include "Gracz.h"
//#include "Global.h"

Obiekt::Obiekt(sf::RenderWindow &okno, const std::string &sciezka) {
    aktywny = true;
    this->okno = &okno;
    if(!tekstura.loadFromFile(TEKSTURY + sciezka)) {
//        std::cerr << "Blad otwarcia pliku!\n";
        throw std::invalid_argument("Blad otwarcia plik!");
    } else {
        sprite.setTexture(tekstura);
    }
}

bool Obiekt::GetAktywny() {
    return aktywny;
}

void Obiekt::Rysuj() {
    if (aktywny) {
        sprite.setTexture(tekstura);
        okno->draw(sprite);
    }
}

sf::FloatRect Obiekt::GetGlobalBounds() {
    return sprite.getGlobalBounds();
}

void Obiekt::SetAktywny(bool status) {
    aktywny = status;
}

Pocisk::Pocisk(sf::RenderWindow &okno, const std::string &sciezka, sf::Vector2f predkosc, float x, float y) : Obiekt(okno, sciezka) {
    this->predkosc = predkosc;
    x -= tekstura.getSize().x / 2;
    this -> x = x;
    this -> y = y;
    sprite.setPosition(x, y);
}

Gracz::Gracz(sf::RenderWindow &okno, const std::string &sciezka) : Obiekt(okno, sciezka) {
    zycia = ZYCIA_GRACZA;
    przerwa = PRZERWA_STRZALU_GRACZA;
    punkty = 0;
    pociski.reserve(50);

    if(!czcionka.loadFromFile(CONSOLAS)) {
        throw std::invalid_argument("Blad otwarcia plik!");
    }

    wyswietlanie_punktow.setFont(czcionka);
    wyswietlanie_punktow.setCharacterSize(20);
    wyswietlanie_punktow.setFillColor(sf::Color::White);
    wyswietlanie_punktow.setPosition({10, 10});

    wyswietlanie_zyc.setFont(czcionka);
    wyswietlanie_zyc.setCharacterSize(20);
    wyswietlanie_zyc.setFillColor(sf::Color::White);
    wyswietlanie_zyc.setString("Zycia: " + std::to_string(zycia));
    wyswietlanie_zyc.setPosition({okno.getSize().x - wyswietlanie_zyc.getGlobalBounds().width - 10, 10});

    x = okno.getSize().x / 2 - sprite.getGlobalBounds().width / 2;
    y = okno.getSize().y - sprite.getGlobalBounds().height - 20;
    sprite.setPosition(x, y);
}

void Pocisk::update() {
    sprite.getPosition().y <= 0 || sprite.getPosition().y >= okno->getSize().y ? aktywny = false : aktywny = aktywny;
    sprite.setTexture(tekstura);
    if(aktywny) {
        sprite.move(predkosc);
        okno->draw(sprite);
    }
}

void Gracz::RuchPrawo() {
    if (x + sprite.getGlobalBounds().width < (*okno).getSize().x) {
        x += 0.2;
        sprite.move(0.2, 0);
    }
}

void Gracz::RuchLewo() {
    if (x >= 0.2) {
        x -= 0.2;
        sprite.move(-0.2, 0);
    }
}

void Gracz::Strzal() {
    if (zegar.getElapsedTime() >= przerwa) {
        pociski.push_back(Pocisk(*okno, POCISK, PREDKOSC_POCISKU, x + sprite.getGlobalBounds().width / 2, y));
        zegar.restart();
    }
}

void Gracz::UpdatePociskow() {
    for (auto &pocisk : pociski) {
        if (pocisk.GetAktywny()) {
            pocisk.update();
        }
    }

    auto it = std::ranges::remove_if(pociski, [](Pocisk &pocisk) {
        return !pocisk.GetAktywny();
    });

    pociski.erase(it.begin(), it.end());
}

std::vector<Pocisk>* Gracz::GetPociski() {
    return &pociski;
}

void Gracz::Update(std::vector<Pocisk> &pociski) {
    //Sprawdzanie kolizji
    for (auto &pocisk : pociski) {
        if (pocisk.GetGlobalBounds().intersects(sprite.getGlobalBounds()) && pocisk.GetAktywny()) {
            pocisk.SetAktywny(false);
            zycia--;
        }
    }
    if (zycia <= 0) {
        aktywny = false;
    }

    wyswietlanie_punktow.setString("Punkty: " + std::to_string(punkty));
    wyswietlanie_zyc.setString("Zycia: " + std::to_string(zycia));
}

void Gracz::DodajPunkt() {
    punkty++;
}

int Gracz::GetPunkty() {
    return punkty;
}

void Gracz::Rysuj() {
    Obiekt::Rysuj();
    okno->draw(wyswietlanie_punktow);
    okno->draw(wyswietlanie_zyc);
}

Obcy::Obcy(sf::RenderWindow &okno, const std::string &sciezka, float x, float y) : Obiekt(okno, sciezka){
    this->x = x;
    this->y = y;
    sprite.setPosition(x, y);
}

void Obcy::Move(sf::Vector2f &predkosc) {
    sprite.move(predkosc);
}

MenadzerObcych::MenadzerObcych(sf::RenderWindow &okno, sf::Vector2f wymiary_obcego, float poziom) {
    predkosc = PREDKOSC_OBCEGO_PRAWO * poziom;
    this->poziom = poziom;
    this->okno = &okno;
    int obcy_poziom = std::floor(((okno.getSize().x / 4) * 3) / wymiary_obcego.x);
    std::cout << obcy_poziom;
    const int obcy_pion = 3;
    const int margines = 10;
    Przeciwnicy.resize(obcy_poziom);

    for (int i = 0; i < obcy_poziom; ++i) {
        for (int j = 0; j < obcy_pion; ++j) {
            Przeciwnicy[i].emplace_back(Obcy(okno, OBCY, i * (wymiary_obcego.x + margines) + margines,
                                             j * (wymiary_obcego.y + margines) + margines + 50));
        }
    }
}

void MenadzerObcych::UpdateObcych(std::vector<Pocisk> &pociski, Gracz &gracz) {
    //Sprawdzanie kolizji
    for (auto &pocisk : pociski) {
        sf::FloatRect granice_pocisku = pocisk.GetGlobalBounds();
        for (auto &kol : Przeciwnicy) {
            kol.erase(std::remove_if(kol.begin(), kol.end(), [&](Obcy &obcy) {
                if (granice_pocisku.intersects(obcy.GetGlobalBounds())) {
                    obcy.SetAktywny(false);
                    pocisk.SetAktywny(false);
                    gracz.DodajPunkt();
                }
                return !obcy.GetAktywny();
            }), kol.end());
        }
    }

//    Ruszanie się obcych
    if (zegar_ruchu.getElapsedTime() >= przerwa_ruchu) {
        for (auto &kol : Przeciwnicy) {
            if (!kol.empty()){
                if (kol.back().GetGlobalBounds().left + WYMIARY_OBCEGO.x >= okno->getSize().x) {
                    predkosc = PREDKOSC_OBCEGO_LEWO * poziom;
                    for (auto &kolumn : Przeciwnicy) {
                        for (auto &obcy : kolumn) {
                            obcy.Move(const_cast<sf::Vector2f &>(PREDKOSC_OBCEGO_DOL));
                        }
                    }
                }
                if (kol.front().GetGlobalBounds().left <= 0) {
                    predkosc = PREDKOSC_OBCEGO_PRAWO * poziom;
                    for (auto &kolumn : Przeciwnicy) {
                        for (auto &obcy : kolumn) {
                            obcy.Move(const_cast<sf::Vector2f &>(PREDKOSC_OBCEGO_DOL));
                        }
                    }
                }
                for (auto &obcy : kol) {
                    obcy.Move(predkosc);
                    if (obcy.GetGlobalBounds().top + obcy.GetGlobalBounds().height >= gracz.GetGlobalBounds().top) {
                        gracz.SetAktywny(false);
                    }
                }
            }
        }
        zegar_ruchu.restart();
    }

//    Strzelanie obcych
    if (zegar_strzlu.getElapsedTime() >= przerwa_strzalu) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, Przeciwnicy.size() - 1);

        int losowa_kol = dis(gen);

        if (!Przeciwnicy[losowa_kol].empty()) {
            auto obcy_GB = Przeciwnicy[losowa_kol].back().GetGlobalBounds();
            this->pociski.emplace_back(Pocisk(*okno, POCISK, PREDKOSC_POCISKU_OBCEGO,
                                        obcy_GB.left + obcy_GB.width / 2,
                                        obcy_GB.top + obcy_GB.height / 2));
//            Przeciwnicy[losowa_kol].back().Strzal();
        }
        zegar_strzlu.restart();
    }

    for (auto &kol : Przeciwnicy) {
        std::erase_if(kol, [](Obcy &obcy) {
            return !obcy.GetAktywny();
        });
    }

    std::erase_if(Przeciwnicy, [](std::vector<Obcy> &kol) {
       return kol.empty();
    });
}

void MenadzerObcych::UpdatePociskow() {
    for (auto &pocisk : pociski) {
        if (pocisk.GetAktywny()) {
            pocisk.update();
        }
    }
}

std::vector<Pocisk> *MenadzerObcych::GetPociski() {
    return &pociski;
}

bool MenadzerObcych::KoniecPoziomu() {
    if (Przeciwnicy.empty()) {
        return true;
    } else {
        return false;
    }
}

void MenadzerObcych::Rysuj() {
    for (auto &kol : Przeciwnicy) {
        for (auto &obcy: kol) {
            if (obcy.GetAktywny()) {
                obcy.Rysuj();
            }
        }
    }
}

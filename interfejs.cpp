#include <iostream>
#include "interfejs.h"

Pozycja::Pozycja(std::string sciezka, sf::Vector2f wspolrzedne, int kolejnosc, sf::RenderWindow &okno) {
    aktywny = false;
    pierwsze_wywolanie = true;
    if(!tekstura.loadFromFile(sciezka)) {
        throw std::invalid_argument("Blad otwarcia plik!");
    }
    sprite.setTexture(tekstura);
    sprite.setPosition(wspolrzedne.x - sprite.getGlobalBounds().width / 2,
                       wspolrzedne.y + kolejnosc * (sprite.getGlobalBounds().height + 10));
    this->okno = &okno;
}

void Pozycja::Rysuj(bool najechane) {
    if (najechane) {
        sprite.setColor(sf::Color(173, 173, 173));
    } else {
        sprite.setColor(sf::Color::White);
    }
    sprite.setTexture(tekstura);
    okno->draw(sprite);
}

sf::FloatRect Pozycja::GetGlobalBounds() {
    return sprite.getGlobalBounds();
}

void Pozycja::SetAktywny(bool status) {
    aktywny = status;
}

bool Pozycja::GetAktywny() {
    return aktywny;
}

Menu::Menu(sf::RenderWindow &okno) {
    nick = "";
    wyswieltaj = true;
    this->okno = &okno;
    wspolrzedne.x = okno.getSize().x / 2;
    wspolrzedne.y = 100;

    index_wybranej_pozycji = -1;

    std::filesystem::permissions(REKORDY, std::filesystem::perms::owner_all
                                                 | std::filesystem::perms::group_all
                                                 | std::filesystem::perms::others_all);

    std::ifstream plik(REKORDY);
    if (plik.is_open()) {
        std::string linia;
        while (std::getline(plik, linia)) {
            if (!linia.empty()) {
                std::istringstream strumien(linia);
                std::string punkty;
                std::string nick;
                strumien >> punkty >> nick;
                rekordy[nick] = std::stoi(punkty);
            }
        }
    }

    pozycje.push_back(std::make_unique<Tytul>(TYTUL, wspolrzedne, 0, okno));
    pozycje.push_back(std::make_unique<Graj>(GRAJ, wspolrzedne, 1, okno));
    pozycje.push_back(std::make_unique<Logowanie>(LOGOWANIE, wspolrzedne, 2, okno));
    pozycje.push_back(std::make_unique<Rekordy>(REKORDY_SIEZKA, wspolrzedne, 3, okno));
    pozycje.push_back(std::make_unique<Wyjscie>(WYJSCIE, wspolrzedne, 4, okno));
}

void Menu::Update() {
    wyswieltaj = true;

    for (auto pozycja : pozycje) {
        if (pozycja->GetAktywny()) {
            wyswieltaj = false;
        }
    }
    if (wyswieltaj) {
        for (int i = 0; i <= pozycje.size() - 1; ++i) {
            if (i == index_wybranej_pozycji && i != 0) {
                pozycje[i]->Rysuj(true);
            } else {
                pozycje[i]->Rysuj(false);
            }
        }
    }
}

void Menu::Mysz(sf::Vector2<int> pozycja_myszy, sf::Event & event) {
    if (wyswieltaj){
        bool mysz_na_pozycji = false;
        for (int i = 0; i <= pozycje.size() - 1; ++i) {
            if (pozycje[i]->GetGlobalBounds().contains(static_cast<sf::Vector2f>(pozycja_myszy))) {
                index_wybranej_pozycji = i;
                mysz_na_pozycji = true;
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    pozycje[i]->SetAktywny(true);
                }
            }
        }
        if (!mysz_na_pozycji) {
            index_wybranej_pozycji = -1;
        }
    }
}

std::vector<std::shared_ptr<Pozycja>> Menu::GetPozycje() {
    return pozycje;
}

std::map<std::string, int> Menu::GetRekordy() {
    return rekordy;
}

std::string Menu::GetNick() {
    return nick;
}

void Menu::SetNick(std::string nick) {
    this->nick = nick;
}

void Menu::DodajRekord(std::pair<std::string, int> rekord) {
    rekordy[rekord.first] = rekord.second;
}

void Graj::Wykonaj(sf::Event &event, Menu &menu) {
    if (gracz == nullptr) {
        gracz = std::make_unique<Gracz>(*okno, STATEK);
        menadzer = std::make_unique<MenadzerObcych>(*okno, WYMIARY_OBCEGO, poziom);
    }
    if (menadzer == nullptr) {
        menadzer = std::make_unique<MenadzerObcych>(*okno, WYMIARY_OBCEGO, poziom);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        gracz->RuchPrawo();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        gracz->RuchLewo();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        gracz->Strzal();
    }

    gracz->UpdatePociskow();

    std::thread threadGracz([&]() { gracz->Update(*menadzer->GetPociski()); });
    std::thread threadMenadzer([&]() { menadzer->UpdateObcych(*gracz->GetPociski(), *gracz); });

    /*gracz->Update(*menadzer->GetPociski());
    menadzer->UpdateObcych(*gracz->GetPociski(), *gracz);*/

    threadMenadzer.join();
    threadGracz.join();

    gracz->Rysuj();
    menadzer->Rysuj();
    menadzer->UpdatePociskow();


    if (!gracz->GetAktywny()) {
        punkty = gracz->GetPunkty();
        menu.DodajRekord(std::pair<std::string, int>(menu.GetNick(), punkty));
        gracz = nullptr;
        menadzer = nullptr;
        poziom = 1;
        aktywny = false;
    } else if (menadzer->KoniecPoziomu()) {
        poziom += 0.5;
        menadzer = nullptr;
    }
    if (event.key.code == sf::Keyboard::Key::Escape) {
        aktywny = false;
        gracz = nullptr;
        menadzer = nullptr;
    }
}

void Tytul::Wykonaj(sf::Event &event, Menu &menu) {
    std::cout << "bajo jajo";
}

void Logowanie::Wykonaj(sf::Event &event, Menu &menu) {
    if (tekstbox == nullptr) {
        tekst_naglowka = "Podaj nick:";
        naglowek.setString(tekst_naglowka);
        if (!czciokna.loadFromFile(CONSOLAS)) {
            throw std::invalid_argument("Blad otwarcia plik!");
        }
        naglowek.setFont(czciokna);
        naglowek.setCharacterSize(40);
        naglowek.setFillColor(sf::Color::White);
        naglowek.setPosition(okno->getSize().x / 2 - naglowek.getLocalBounds().width / 2, okno->getSize().y / 4);

        sf::Vector2f pozycja = {static_cast<float>(okno->getSize().x / 2), okno->getSize().y / 4 + naglowek.getLocalBounds().height + 50};
        tekstbox = std::make_unique<TextBox>(pozycja);

    }
    if (event.key.code == sf::Keyboard::Enter) {
//        REGEX
        std::regex patern("^[a-zA-Z0-9]+$");
        std::string tetet = tekstbox->GetText();
        if (std::regex_match(tekstbox->GetText(), patern)) {
            menu.SetNick(tekstbox->GetText());
            aktywny = false;
        } else {
            tekstbox->Reset();
            tekstbox->Rysuj(*okno);
        }
        tekstbox = nullptr;

    } else if (tekstbox != nullptr) {
        tekstbox->HandleEvent(event, *okno);
        tekstbox->Rysuj(*okno);
    }

    if (event.key.code == sf::Keyboard::Key::Escape) {
        aktywny = false;
        pierwsze_wywolanie = true;
    }

    okno->draw(naglowek);
}

TextBox::TextBox( sf::Vector2f pozycja) {
    klawisz_przetwarzany = false;
    if (!czciokna.loadFromFile(CONSOLAS)) {
        throw std::invalid_argument("Blad otwarcia plik!");
    }

    box.setSize(sf::Vector2f(500, 50));
    box.setFillColor(sf::Color::White);
    box.setOutlineColor(sf::Color::Blue);
    box.setOutlineThickness(2);
    box.setPosition(pozycja.x - box.getSize().x / 2, pozycja.y);

    text.setFont(czciokna);
    text.setCharacterSize(static_cast<unsigned int>(box.getSize().y * 0.7f));
    text.setFillColor(sf::Color::Black);
    text.setPosition(box.getPosition());
}

void TextBox::HandleEvent(const sf::Event& event, sf::RenderWindow &okno) {
    if (event.type == sf::Event::TextEntered) {
        if (!klawisz_przetwarzany) {
            if (event.text.unicode == '\b' && !Nick.empty()) {
                Nick.pop_back();
            } else {
                Nick += static_cast<char>(event.text.unicode);
            }
            text.setString(Nick);
            klawisz_przetwarzany = true;
        }
    } else {
        klawisz_przetwarzany = false;
    }
}

void TextBox::Rysuj(sf::RenderWindow &window) {
    window.draw(box);
    window.draw(text);
}

std::string TextBox::GetText() {
    if (!Nick.empty() && Nick.back() == '\r') {
        Nick.pop_back();
    }
    return Nick;
}

void TextBox::Reset() {
    Nick = "";
}

void Rekordy::Wykonaj(sf::Event &event, Menu &menu) {
    if (pierwsze_wywolanie) {
        if (!czcionka.loadFromFile(CONSOLAS)) {
            throw std::invalid_argument("Blad otwarcia plik!");
        }
        tekstbox.setFont(czcionka);
        tekstbox.setFillColor(sf::Color::White);
        tekstbox.setCharacterSize(40);

        tekst = "Wynik:\tNick:\n";

        auto rekordy = menu.GetRekordy();
        for (auto it = rekordy.rbegin(); it != rekordy.rend(); ++it) {
            tekst += std::to_string(it->second) + ":\t" + it->first + "\n";
        }

        tekstbox.setString(tekst);

        tekstbox.setPosition(okno->getSize().x / 2 - tekstbox.getGlobalBounds().width / 2,
                             okno->getSize().y / 2 - tekstbox.getGlobalBounds().height / 2);


        pierwsze_wywolanie = false;
    }

    if (event.key.code == sf::Keyboard::Key::Escape) {
        aktywny = false;
        pierwsze_wywolanie = true;
    }

    okno->draw(tekstbox);
}

void Wyjscie::Wykonaj(sf::Event &event, Menu &menu) {
    std::ofstream plik(REKORDY);
    if (plik.is_open()) {
        for (auto &para : menu.GetRekordy()) {
            plik << std::to_string(para.second) + " " + para.first + "\n";
        }
    } else {
        throw std::invalid_argument("Blad otwarcia plik!");
    }

    std::filesystem::permissions(REKORDY, std::filesystem::perms::owner_read
                                                    | std::filesystem::perms::group_read
                                                    | std::filesystem::perms::others_read);

    okno->close();
}

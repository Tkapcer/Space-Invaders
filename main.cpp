#include <SFML/Graphics.hpp>
#include "interfejs.h"
#include "Gracz.h"

int main()
{
    sf::RenderWindow okno(sf::VideoMode(1920, 1080), "Space Invaders");

    Menu menu(okno);

    while (okno.isOpen()) {
       sf::Event event;
        while (okno.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                okno.close();
            }
        }

        okno.clear();

        for (int i = 0; i < TEKSTURY_MENU.size(); ++i) {
            if (menu.GetPozycje()[i]->GetAktywny()) {
                menu.GetPozycje()[i]->Wykonaj(event, menu);
            }
        }

        menu.Update();
        menu.Mysz(sf::Mouse::getPosition(okno), event);
        okno.display();
    }

    return 0;
}

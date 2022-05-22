#include <SFML/Graphics.hpp>
#include "Grid.h"
#include "WindowViewController.h"
#include "memory"

int main()
{

    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 800), "SFML works!");
    Grid grid;
    WindowViewController wvc(window);

    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
            wvc.handleInput(event);
        }

        window->clear();
        window->draw(grid);
        window->display();
    }

    return 0;
}
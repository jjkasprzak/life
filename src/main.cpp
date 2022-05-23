#include <SFML/Graphics.hpp>
#include "Grid.h"
#include "WindowViewController.h"
#include "memory"
#include <filesystem>

int main()
{
    auto lato = std::make_shared<sf::Font>();

    if (!lato->loadFromFile("fonts/Lato-Regular.ttf"))
    {
        throw std::exception("Failed to load a font");
    }

    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 800), "SFML works!");
    Grid grid;
    grid.setFont(lato);
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
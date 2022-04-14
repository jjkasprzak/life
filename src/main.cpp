#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow* windowp = new sf::RenderWindow(sf::VideoMode(200, 200), "SFML works!");
    sf::RenderWindow& window = *windowp;
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
    delete windowp;

    return 0;
}

#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class WindowViewController
{
public:
	WindowViewController(std::weak_ptr<sf::RenderWindow> window)
		:window_(window)
        ,drag_(false)
	{}
	void attach(std::weak_ptr<sf::RenderWindow> window)
	{
		window_ = window;
	}
	std::weak_ptr<sf::RenderWindow> getWindow() const
	{
		return window_;
	}
	
	void handleInput(const sf::Event& e) const
	{
		if (window_.expired())
			return;

		auto window = window_.lock();
        sf::View view = window->getView();
        const sf::Vector2f windowSize = sf::Vector2f((float)window->getSize().x, (float)window->getSize().y);
        const sf::Vector2f mouseViewPos = sf::Vector2f(e.mouseMove.x, e.mouseMove.y).cwiseDiv(windowSize).cwiseMul(view.getSize());

        if (e.type == sf::Event::MouseWheelScrolled)
        {
            view.zoom((e.mouseWheelScroll.delta < 0 ? 1.05f : 0.95f));
        }
        if (!drag_ && e.type == sf::Event::MouseButtonPressed)
        {
            drag_ = true;
            dragPos_ = mouseViewPos;
        }
        if (drag_ && e.type == sf::Event::MouseButtonReleased)
        {
            drag_ = false;
        }

        if (drag_ && e.type == sf::Event::MouseMoved)
        {
            view.move(dragPos_ - mouseViewPos);
            dragPos_ = mouseViewPos;
        }
        window->setView(view);
	}
private:
	std::weak_ptr<sf::RenderWindow> window_;

    mutable bool drag_;
    mutable sf::Vector2f dragPos_;
};
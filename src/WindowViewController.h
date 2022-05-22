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
        const sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
        const sf::Vector2f viewULPos = sf::Vector2f(mousePos.x, mousePos.y).cwiseDiv(windowSize).cwiseMul(view.getSize());

        if (e.type == sf::Event::MouseWheelScrolled)
        {
            float factor = e.mouseWheelScroll.delta < 0 ? 1.05f : 0.95f;
            const sf::Vector2f fromCenterPos = viewULPos - view.getSize().cwiseMul(sf::Vector2f(.5f, .5f));
            float fixFactor = (1-factor);
            view.move(fromCenterPos.cwiseMul({ fixFactor, fixFactor }));
            view.zoom(factor);
        }
        if (!drag_ && e.type == sf::Event::MouseButtonPressed)
        {
            drag_ = true;
            dragPos_ = viewULPos;
        }
        if (drag_ && e.type == sf::Event::MouseButtonReleased)
        {
            drag_ = false;
        }

        if (drag_ && e.type == sf::Event::MouseMoved)
        {
            view.move(dragPos_ - viewULPos);
            dragPos_ = viewULPos;
        }
        window->setView(view);
	}
private:
	std::weak_ptr<sf::RenderWindow> window_;

    mutable bool drag_;
    mutable sf::Vector2f dragPos_;
};
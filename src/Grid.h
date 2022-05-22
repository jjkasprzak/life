#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <array>
#include <stdio.h>

class Grid : public sf::Drawable
{
public:
	Grid(size_t desiredNumOfGridLines = 12, bool showLabels = true) noexcept
		:desiredNumOfGridLines_(desiredNumOfGridLines)
		,showLabels_(showLabels)
		,gridColor_(sf::Color::White)
		,valid_(false)
		,scaleFactor_(1)
		,scaleExponent_(0)
	{
		background_.setFillColor(sf::Color::Black);
	}

	void setDesiredNumOfGridLines(size_t nogl)
	{
		desiredNumOfGridLines_ = nogl;
	}

	void setLabelVisibility(bool showLabels)
	{
		showLabels_ = showLabels;
	}

	void setBackgroundColor(const sf::Color& color)
	{
		background_.setFillColor(color);
	}

	void setGridColor(const sf::Color& color)
	{
		gridColor_ = color;
	}

	bool getLabelVisibility() const
	{
		return showLabels_;
	}

	sf::Color getBackgroundColor() const
	{
		return background_.getFillColor();
	}

	size_t getNumberOfCells() const
	{
		return desiredNumOfGridLines_;
	}

	sf::Color setGridColor() const
	{
		return gridColor_;
	}

protected:
	virtual void draw(sf::RenderTarget& target, const sf::RenderStates& states) const override;
	
private:
	static const sf::FloatRect supportedArea;

	void updateGrid() const;
	void updateScale(const sf::Vector2f& diagonal) const;

	size_t desiredNumOfGridLines_;
	bool showLabels_;
	sf::Color gridColor_;

	mutable bool valid_;
	mutable unsigned int scaleFactor_;
	mutable int scaleExponent_;
	mutable sf::FloatRect bounds_;
	mutable sf::RectangleShape background_;
	mutable sf::VertexArray lines_, suppLines_;
	mutable std::vector<sf::Text> xLabels_, yLabels_;
};
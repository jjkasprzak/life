#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <array>
#include <stdio.h>

class Grid : public sf::Drawable
{
public:
	Grid(size_t numberOfCells = 64, bool showLabels = true) noexcept
		:numberOfCells_(numberOfCells)
		,showLabels_(showLabels)
		,gridColor_(sf::Color::White)
		,valid_(false)
		,scaleFactor_(0)
		,scaleExponent_(0)
	{
		background_.setFillColor(sf::Color::Black);
	}

	void setNumberOfCells(size_t numberOfCells)
	{
		if (numberOfCells_ != numberOfCells)
			valid_ = false;
		numberOfCells_ = numberOfCells;
	}

	void setLabelsVisibility(bool showScale)
	{
		showLabels_ = showScale;
	}

	void setBackgroundColor(const sf::Color& color)
	{
		background_.setFillColor(color);
	}

	void setGridColor(const sf::Color& color)
	{
		gridColor_ = color;
	}

	bool getLabelsVisibility() const
	{
		return showLabels_;
	}

	sf::Color getBackgroundColor() const
	{
		return background_.getFillColor();
	}

	size_t getNumberOfCells() const
	{
		return numberOfCells_;
	}

	sf::Color setGridColor() const
	{
		return gridColor_;
	}

protected:
	virtual void draw(sf::RenderTarget& target, const sf::RenderStates& states) const override;
	
private:
	void updateGrid() const;
	void updateScale(const sf::Vector2f& diagonal) const;

	size_t numberOfCells_;
	bool showLabels_;
	sf::Color gridColor_;

	mutable bool valid_;
	mutable unsigned int scaleFactor_;
	mutable int scaleExponent_;
	mutable sf::FloatRect bounds_;
	mutable sf::RectangleShape background_;
	mutable sf::VertexArray ticks_, suppTicks_;
	mutable std::vector<sf::Text> xLabels_, yLabels_;
};
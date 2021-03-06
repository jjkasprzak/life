#include "Grid.h"

const sf::FloatRect Grid::supportedArea(sf::Vector2f(-1e36f, -1e36f), sf::Vector2f(2e36f, 2e36f));

void Grid::draw(sf::RenderTarget& target, const sf::RenderStates& states) const
{
	sf::Vector2f center = target.getView().getCenter();
	sf::Vector2f diagonal = target.getView().getSize();
	sf::Vector2f diagHalf = diagonal.cwiseMul(sf::Vector2f(0.5f, 0.5f));
	sf::Vector2f ul = center - diagHalf;
	sf::Vector2f br = center + diagHalf;

	updateScale(diagonal);
	valid_ = valid_ && bounds_.contains(ul) && bounds_.contains(br);
	if (!valid_)
	{
		bounds_ = supportedArea.findIntersection(sf::FloatRect(ul - diagonal, diagonal.cwiseMul(sf::Vector2f(3.f, 3.f)))).value();
		updateGrid();
	}

	target.draw(background_, states);
	target.draw(lines_, states);
	target.draw(suppLines_, states);
	if (showLabels_)
	{
		float txtScale = std::max(diagonal.x, diagonal.y) * .007 / desiredNumOfGridLines_;
		float x = ul.x, y = ul.y;
		for (sf::Text& e : this->xLabels_)
		{
			e.setScale(sf::Vector2f(txtScale, txtScale));
			e.setPosition({ e.getPosition().x, y});
			target.draw(e, states);
		}
		for (sf::Text& e : this->yLabels_)
		{
			e.setScale(sf::Vector2f(txtScale, txtScale));
			e.setPosition({ x, e.getPosition().y});
			target.draw(e, states);
		}
	}
}

void Grid::updateGrid() const
{
	printf("GEN\n");
	float unit = scaleFactor_ * std::pow(10, scaleExponent_);
	float x = unit * (int)(bounds_.left / unit);
	float y = unit * (int)(bounds_.top / unit);

	float xlim = bounds_.left + bounds_.width;
	float ylim = bounds_.top + bounds_.height;
	lines_.clear();
	lines_.setPrimitiveType(sf::PrimitiveType::Lines);

	suppLines_.clear();
	suppLines_.setPrimitiveType(sf::PrimitiveType::Lines);
	int suppCount = scaleFactor_ == 2 ? 4 : 5;

	xLabels_.clear();
	yLabels_.clear();

	sf::Color suppTickColor = gridColor_;
	suppTickColor.a = 64;

	while (x < xlim)
	{
		lines_.append(sf::Vertex(sf::Vector2f(x, bounds_.top), gridColor_));
		lines_.append(sf::Vertex(sf::Vector2f(x, ylim), gridColor_));

		for (size_t i = 0; i < suppCount; i++)
		{
			float tmpx = x + unit * (i + 1) / suppCount;
			suppLines_.append(sf::Vertex(sf::Vector2f(tmpx, ylim), suppTickColor));
			suppLines_.append(sf::Vertex(sf::Vector2f(tmpx, bounds_.top), suppTickColor));
		}
		sf::Text label;


		label.setString(":^)");
		label.setPosition(sf::Vector2f(x, ylim));
		label.setFont(*font_);
		xLabels_.emplace_back(label);

		x += unit;
	}
	while (y < ylim)
	{
		lines_.append(sf::Vertex(sf::Vector2f(bounds_.left, y), gridColor_));
		lines_.append(sf::Vertex(sf::Vector2f(xlim, y), gridColor_));

		for (size_t i = 0; i < suppCount; i++)
		{
			float tmpy = y + unit * (i + 1) / suppCount;
			suppLines_.append(sf::Vertex(sf::Vector2f(xlim, tmpy), suppTickColor));
			suppLines_.append(sf::Vertex(sf::Vector2f(bounds_.left, tmpy), suppTickColor));
		}
		sf::Text label;

		label.setString(":^)");
		label.setPosition(sf::Vector2f(xlim, y));
		label.setFont(*font_);
		yLabels_.emplace_back(label);

		y += unit;
	}
	background_.setPosition(bounds_.getPosition());
	background_.setSize(bounds_.getSize());
	valid_ = true;
}

void Grid::updateScale(const sf::Vector2f& diagonal) const
{
	float idealUnit = std::max(diagonal.x, diagonal.y) / desiredNumOfGridLines_;
	float floatExp = std::log10f(idealUnit);
	int exponent = (int)floatExp;
	if (floatExp < 0)
		--exponent;
	float idealFactor = idealUnit / std::powf(10, exponent);

	unsigned int candidateFactor = 10;
	float minError = std::abs(candidateFactor - idealFactor);
	float err;
	for (auto factor : { 1,2,5 })
	{
		err = std::abs(factor - idealFactor);
		if (err < minError)
		{
			minError = err;
			candidateFactor = factor;
		}
	}
	if (candidateFactor == 10)
	{
		candidateFactor = 1;
		++exponent;
	}
	if (scaleFactor_ != candidateFactor || scaleExponent_ != exponent)
		valid_ = false;
	scaleFactor_ = candidateFactor;
	scaleExponent_ = exponent;
}

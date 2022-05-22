#include "Grid.h"

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
		bounds_ = sf::FloatRect(ul - diagonal, diagonal.cwiseMul(sf::Vector2f(3.f, 3.f)));
		updateGrid();
	}

	target.draw(background_, states);
	target.draw(ticks_, states);
	target.draw(suppTicks_, states);
	if (showLabels_)
	{
		for (const sf::Text& e : this->xLabels_)
			target.draw(e, states);
		for (const sf::Text& e : this->yLabels_)
			target.draw(e, states);
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
	ticks_.clear();
	ticks_.setPrimitiveType(sf::PrimitiveType::Lines);

	suppTicks_.clear();
	suppTicks_.setPrimitiveType(sf::PrimitiveType::Lines);
	int suppCount = scaleFactor_ == 2 ? 4 : 5;

	xLabels_.clear();
	yLabels_.clear();

	sf::Color suppTickColor = gridColor_;
	suppTickColor.a = 64;

	while (x < xlim)
	{
		ticks_.append(sf::Vertex(sf::Vector2f(x, bounds_.top), gridColor_));
		ticks_.append(sf::Vertex(sf::Vector2f(x, ylim), gridColor_));

		for (size_t i = 0; i < suppCount; i++)
		{
			float tmpx = x + unit * (i + 1) / suppCount;
			suppTicks_.append(sf::Vertex(sf::Vector2f(tmpx, ylim), suppTickColor));
			suppTicks_.append(sf::Vertex(sf::Vector2f(tmpx, bounds_.top), suppTickColor));
		}
		x += unit;
	}
	while (y < ylim)
	{
		ticks_.append(sf::Vertex(sf::Vector2f(bounds_.left, y), gridColor_));
		ticks_.append(sf::Vertex(sf::Vector2f(xlim, y), gridColor_));

		for (size_t i = 0; i < suppCount; i++)
		{
			float tmpy = y + unit * (i + 1) / suppCount;
			suppTicks_.append(sf::Vertex(sf::Vector2f(xlim, tmpy), suppTickColor));
			suppTicks_.append(sf::Vertex(sf::Vector2f(bounds_.left, tmpy), suppTickColor));
		}
		y += unit;
	}
	background_.setPosition(bounds_.getPosition());
	background_.setSize(bounds_.getSize());
	valid_ = true;
}

void Grid::updateScale(const sf::Vector2f& diagonal) const
{
	float area = diagonal.dot(diagonal);
	float idealUnit = std::sqrtf(area / numberOfCells_);
	int exponent = static_cast<int>(std::log10f(idealUnit));
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

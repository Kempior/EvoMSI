#include "GUI/Graph.hpp"

Graph::Graph(std::string name, sf::FloatRect rect): Widget(name),
rect(rect)
{
	graphView.setViewport(rect);
	graphView.setCenter(5.0f, -5.0f);
	graphView.zoom(1.075f);
	
	circle.setRadius(1.0f);
	circle.setFillColor(sf::Color::Red);
}

void Graph::handleEvent(const sf::Event& event)
{
	Widget::handleEvent(event);
}

void Graph::update(float dt)
{
	Widget::update(dt);
}

void Graph::draw(sf::RenderWindow& window)
{
	sf::View currentView = window.getView();
	
	window.setView(graphView);
	
	for(auto point : *points)
	{
		circle.setPosition(point.first, point.second);
	}
	
	window.setView(currentView);
	
	Widget::draw(window);
}

sf::Vector2f Graph::getPosition()
{
	return position;
}

sf::Vector2f Graph::getSize()
{
	return size;
}

void Graph::setPosition(sf::Vector2f position)
{
	this->position = position;
}

void Graph::recalculateSize()
{
	sf::Vector2f parentSize = parent->getSize();
	
	sf::Vector2f tl = sf::Vector2f(rect.left * parentSize.x, rect.top * parentSize.y);
	sf::Vector2f br = sf::Vector2f(rect.width * parentSize.x, rect.height * parentSize.y);
	
	size = br - tl;
}

void Graph::setPoints(const std::vector<std::pair<float, float>>* points)
{
	this->points = points;
}
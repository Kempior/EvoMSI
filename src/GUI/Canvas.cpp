#include "GUI/Canvas.hpp"

Canvas::Canvas(sf::RenderWindow& window): Widget("Canvas")
{
	size = static_cast<sf::Vector2f>(window.getSize());
}

void Canvas::handleEvent(const sf::Event& event)
{
	if(event.type == sf::Event::Resized)
	{
		size = sf::Vector2f(event.size.width, event.size.height);
		
		recalculateSize();
		recalculatePosition();
	}
	
	Widget::handleEvent(event);
}

void Canvas::update(float dt)
{
	updateStatus();
	
	Widget::update(dt);
}

void Canvas::draw(sf::RenderWindow& window)
{
	Widget::draw(window);
}

sf::Vector2f Canvas::getPosition()
{
	return sf::Vector2f();
}

sf::Vector2f Canvas::getSize()
{
	return size;
}

void Canvas::setPosition(sf::Vector2f position)
{
	return;
}

void Canvas::recalculateSize()
{
	Widget::recalculateSize();
}
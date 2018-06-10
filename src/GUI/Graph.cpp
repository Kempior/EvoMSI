#include "GUI/Graph.hpp"

Graph::Graph(std::string name, sf::FloatRect rect): Widget(name),
rect(rect)
{
	graphView.setViewport(rect);
	graphView.setCenter(5.0f, -5.0f);
	graphView.setSize(10.0f, 10.0f);
	graphView.zoom(1.075f);
	
	const float lineSize = 0.04f;
	
	horizontalLine.setSize(sf::Vector2f(20.0f, lineSize));
	verticalLine.setSize(sf::Vector2f(lineSize, -20.0f));
	
	horizontalLine.setFillColor(sf::Color::Black);
	verticalLine.setFillColor(sf::Color::Black);
	
	circle.setRadius(0.05f);
	circle.setFillColor(sf::Color::Red);
}

void Graph::handleEvent(const sf::Event& event)
{
	if(event.type == sf::Event::MouseMoved && otherGraph && points)
	{
		thisCircle = -1;
		otherGraph->otherCircle = -1;
		
		sf::Vector2f mousePos = mapToViewSpace(event.mouseMove.x, event.mouseMove.y);
		mousePos.y = -mousePos.y;
		
		for(unsigned int i = 0; i < points->size(); ++i)
		{
			if(pow(mousePos.x - (*points)[i].first, 2) + pow(mousePos.y - (*points)[i].second, 2) <= pow(0.05f, 2))
			{
				thisCircle = i;
				otherGraph->otherCircle = i;
				break;
			}
		}
	}
	
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
	
	window.draw(horizontalLine);
	window.draw(verticalLine);
	
	if(points)
	{
		int j = 0;
		for(unsigned int i = 0; i < points->size(); ++i)
		{
			if(j == thisCircle || j == otherCircle)
			{
				circle.setFillColor(sf::Color::Blue);
			}
			else
			{
				circle.setFillColor(sf::Color::Red);
			}
			
			circle.setPosition((*points)[i].first - circle.getRadius(), -(*points)[i].second - circle.getRadius());
			window.draw(circle);
			
			++j;
		}
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
	
	Widget::recalculateSize();
}

void Graph::setPoints(const std::vector<std::pair<float, float>>* points)
{
	this->points = points;
}

sf::Vector2f Graph::mapToViewSpace(float x, float y)
{
	const sf::FloatRect &viewport = graphView.getViewport();
	
	sf::IntRect viewRect(static_cast<int>(0.5f + windowSize.x * viewport.left),
						 static_cast<int>(0.5f + windowSize.y * viewport.top),
						 static_cast<int>(0.5f + windowSize.x * viewport.width),
						 static_cast<int>(0.5f + windowSize.y * viewport.height));
	
	sf::Vector2f normalized;
	normalized.x = -1.0f + 2.0f * (x - viewRect.left) / viewRect.width;
	normalized.y = 1.0f - 2.0f * (y - viewRect.top) / viewRect.height;
	
	return graphView.getInverseTransform().transformPoint(normalized);
}
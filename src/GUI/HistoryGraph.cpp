#include "GUI/HistoryGraph.hpp"

#include "GUI/Label.hpp"

HistoryGraph::HistoryGraph(std::string name, sf::FloatRect rect, std::string axisX, std::string axisY): Widget(name),
rect(rect)
{
	graphView.setViewport(rect);
	setView();
	
	const float lineSize = 0.05f;
	
	horizontalLine.setSize(sf::Vector2f(200.0f, lineSize));
	
	horizontalLine.setFillColor(sf::Color::Black);
	verticalLine.setFillColor(sf::Color::Black);
	
	for(int i = 0; i < 6; ++i)
	{
		values[i].setPrimitiveType(sf::LineStrip);
	}
	
	Label *label = new Label(axisX + "Label", axisX);
	label->setAnchor(sf::Vector2f(1.0f, 1.0f), sf::Vector2f(-5.0f, 0.0f), LOWER_RIGHT);
	addChildWidget(label);
	
	label = new Label(axisY + "Label", axisY);
	label->setAnchor(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 3.0f), UPPER_LEFT);
	addChildWidget(label);
}

void HistoryGraph::handleEvent(const sf::Event& event)
{
	Widget::handleEvent(event);
}

void HistoryGraph::update(float dt)
{
	Widget::update(dt);
}

void HistoryGraph::draw(sf::RenderWindow& window)
{
	sf::View currentView = window.getView();
	
	window.setView(graphView);
	
	window.draw(horizontalLine);
	window.draw(verticalLine);
	
	for(int i = 0; i < 6; ++i)
	{
		window.draw(values[i]);
	}
	
	window.setView(currentView);
	
	Widget::draw(window);
}

sf::Vector2f HistoryGraph::getPosition()
{
	return position;
}

sf::Vector2f HistoryGraph::getSize()
{
	return size;
}

void HistoryGraph::setPosition(sf::Vector2f position)
{
	this->position = position;
}

void HistoryGraph::recalculateSize()
{
	sf::Vector2f parentSize = parent->getSize();
	
	size = sf::Vector2f(rect.width * parentSize.x, rect.height * parentSize.y);
	
	Widget::recalculateSize();
}

void HistoryGraph::setPoints(const std::vector<std::pair<float, float>>* points)
{
	this->points = points;
}

void HistoryGraph::addValues()
{
	float min = std::numeric_limits<float>::max(), max = std::numeric_limits<float>::min(), avg;
	
	for(unsigned int i = 0; i < points->size(); ++i)
	{
		if((*points)[i].first < min)
		{
			min = (*points)[i].first;
		}
		
		if(max < (*points)[i].first)
		{
			max = (*points)[i].first;
		}
		
		avg += (*points)[i].first;
	}
	avg /= points->size();
	
	values[0].append(sf::Vertex(sf::Vector2f(iteration, -min), sf::Color(255, 165, 0)));
	values[1].append(sf::Vertex(sf::Vector2f(iteration, -avg), sf::Color::Red));
	values[2].append(sf::Vertex(sf::Vector2f(iteration, -max), sf::Color(255, 165, 0)));
	
	min = std::numeric_limits<float>::max();
	max = std::numeric_limits<float>::min();
	avg = 0;
	
	for(unsigned int i = 0; i < points->size(); ++i)
	{
		if((*points)[i].second < min)
		{
			min = (*points)[i].second;
		}
		
		if(max < (*points)[i].second)
		{
			max = (*points)[i].second;
		}
		
		avg += (*points)[i].second;
	}
	avg /= points->size();
	
	values[3].append(sf::Vertex(sf::Vector2f(iteration, -min), sf::Color(0,191,255)));
	values[4].append(sf::Vertex(sf::Vector2f(iteration, -avg), sf::Color::Blue));
	values[5].append(sf::Vertex(sf::Vector2f(iteration, -max), sf::Color(0,191,255)));

	setView();
	iteration++;
}

void HistoryGraph::reset()
{
	for(int i = 0; i < 6; ++i)
	{
		values[i].clear();
	}
	
	iteration = 0;
}

void HistoryGraph::setView()
{
	const float verticalSize = 0.001f;
	verticalLine.setSize(sf::Vector2f(iteration * verticalSize, -20.0f));
	
	graphView.setCenter(iteration / 2.0f, -5.0f);
	graphView.setSize(iteration * 1.02f, 12.0f);
}
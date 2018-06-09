#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "GUI/Widget.hpp"

#include <string>
#include <vector>

class Graph : public Widget
{
public:
	Graph(std::string name, sf::FloatRect rect);
	
	void handleEvent(const sf::Event& event) override;
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;
	
	sf::Vector2f getPosition() override;
	sf::Vector2f getSize() override;
	void setPosition(sf::Vector2f position) override;
	
	void recalculateSize() override;
	
	void setPoints(const std::vector<std::pair<float, float>>* points);
	
private:
	sf::Vector2f position;
	sf::Vector2f size;
	
	sf::FloatRect rect;
	sf::View graphView;
	
	const std::vector<std::pair<float, float>> *points = nullptr;
	
	sf::RectangleShape horizontalLine, verticalLine;
	sf::CircleShape circle;
};

#endif //GRAPH_HPP
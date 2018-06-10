#ifndef HISTORYGRAPH_HPP
#define HISTORYGRAPH_HPP

#include "GUI/Widget.hpp"

#include <string>
#include <vector>

class HistoryGraph : public Widget
{
public:
	HistoryGraph(std::string name, sf::FloatRect rect, std::string axisX, std::string axisY);
	
	void handleEvent(const sf::Event& event) override;
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;
	
	sf::Vector2f getPosition() override;
	sf::Vector2f getSize() override;
	void setPosition(sf::Vector2f position) override;
	
	void recalculateSize() override;
	
	void setPoints(const std::vector<std::pair<float, float>>* points);
	void addValues();
	void reset();
	
private:
	void setView();
	
private:
	sf::Vector2f position;
	sf::Vector2f size;
	
	sf::FloatRect rect;
	sf::View graphView;
	
	const std::vector<std::pair<float, float>> *points = nullptr;
	
	sf::RectangleShape horizontalLine, verticalLine;
	sf::VertexArray values[6];
	int iteration = 0;
};

#endif //HISTORYGRAPH_HPP
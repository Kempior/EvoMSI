#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "GUI/Widget.hpp"

class Canvas : public Widget
{
public:
	Canvas(sf::RenderWindow& window);

	void handleEvent(const sf::Event& event) override;
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;
	
	sf::Vector2f getPosition() override;
	sf::Vector2f getSize() override;
	void setPosition(sf::Vector2f position) override;
	
	void recalculateSize() override;
	
private:
	sf::Vector2f size;
};

#endif //CANVAS_HPP
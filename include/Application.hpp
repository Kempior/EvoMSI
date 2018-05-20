#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "SFML/Graphics.hpp"

#include <vector>
#include <random>

//class Canvas;

class Application
{
public:
	Application();
	~Application();
	
	void run();
	
private:
	void setupGUI();
	
private:
	sf::RenderWindow window;
	//Canvas* canvas;

	std::minstd_rand0 generator;
	std::uniform_int_distribution<int> dist;
};

#endif //APPLICATION_HPP
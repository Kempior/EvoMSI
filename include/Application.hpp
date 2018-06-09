#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "EvoLogic/Evo.h"

#include "SFML/Graphics.hpp"

#include <vector>
#include <random>

class Canvas;

class Application
{
public:
	Application();
	~Application();
	
	void run();
	
private:
	void setupGUI();
	void setupGraphs();
	
private:
	sf::RenderWindow window;
	Canvas* canvas;

	std::minstd_rand0 generator;
	std::uniform_int_distribution<int> dist;
	
	Evo *evo;
};

#endif //APPLICATION_HPP
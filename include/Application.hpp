#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "SFML/Graphics.hpp"

#include <vector>
#include <random>

#include "EvoLogic/Evo.h"

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
	
	Evo *evo;
};

#endif //APPLICATION_HPP
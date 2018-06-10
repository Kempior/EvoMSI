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
	void setupEvo();
	void resetEvo();
	void setupGUI();
	void setupInfo();
	void setupGraphs();
	void setupHistoryGraph();
	
private:
	sf::RenderWindow window;
	Canvas* canvas;

	std::minstd_rand0 generator;
	std::uniform_int_distribution<int> dist;
	
	Evo *evo = nullptr;
	bool isSimulating = false;
	int popSize = 200;
	float mutationChance = 0.5f, mutationMagnitude = 0.25f;
	float minX = 0.1f, minY = 0.1f, maxX = 10.0f, maxY = 10.0f;
};

#endif //APPLICATION_HPP
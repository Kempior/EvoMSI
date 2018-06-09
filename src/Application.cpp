#include "Application.hpp"
#include "EvoLogic/Evo.h"

#include "ServiceLocator/Locator.hpp"
#include "ServiceLocator/DefaultResourceFont.hpp"

#include "GUI/Widgets.hpp"

#include <ctime>

Application::Application():
generator(std::time(NULL)),
dist(-10, 10)
{
	window.create(sf::VideoMode(1280, 720), "Evo");
	canvas = new Canvas(window);
	
	generator();
	
	Locator::provideFont(new DefaultResourceFont());
	setupGUI();
	
	Evo evo(10, 0, 10, 0, 10);
	evo.fun1 = [](float x, float y) { return x; };
	evo.fun2 = [](float x, float y) { return x; };
	evo.NextGeneration();
}

Application::~Application()
{
	delete canvas;
	
	Locator::terminate();
}

void Application::run()
{
	sf::Clock clock;
	
	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			canvas->handleEvent(event);
			
			if(event.type == sf::Event::Closed)
			{
				window.close();
			}
			
			if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			{
				window.close();
			}
			
			if(event.type == sf::Event::Resized)
			{
				window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
			}
		}

		canvas->update(0.0f);
		
		window.clear();
		
		canvas->draw(window);
		
		window.display();
	}
}

void Application::setupGUI()
{
	setupGraphs();
}

void Application::setupGraphs()
{
	Graph *graph = new Graph("Graph1", sf::FloatRect(0.21875f, 0.0f, 0.39063f, 0.69444f));
	graph->setAnchor(sf::Vector2f(0.21875f, 0.0f), sf::Vector2f(), UPPER_LEFT);
	canvas->addChildWidget(graph);
	
	graph = new Graph("Graph2", sf::FloatRect(0.60938f, 0.0f, 0.39063f, 0.69444f));
	graph->setAnchor(sf::Vector2f(1.0f, 0.0f), sf::Vector2f(), UPPER_RIGHT);
	canvas->addChildWidget(graph);
}
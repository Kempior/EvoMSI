#include "Application.hpp"
#include "EvoLogic/Evo.h"

#include "ServiceLocator/Locator.hpp"
#include "ServiceLocator/DefaultResourceFont.hpp"

#include "GUI/Widgets.hpp"

#include <ctime>
#include <thread>
#include <chrono>

Application::Application():
generator(std::time(NULL)),
dist(-10, 10)
{
	window.create(sf::VideoMode(1280, 720), "Evo");
	canvas = new Canvas(window);
	
	generator();
	
	Locator::provideFont(new DefaultResourceFont());
	setupGUI();
	
	evo = new Evo(20, 0.1, 1, 0.1, 1);
	
	canvas->getWidget<Graph>("Graph1")->setPoints(evo->Points());
	canvas->getWidget<Graph>("Graph2")->setPoints(evo->Costs());
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
		
		window.clear(sf::Color::White);
		
		canvas->draw(window);
		
		window.display();

		// Bull
		if(clock.getElapsedTime().asSeconds() >= 1.0f)
		{
			clock.restart();
			evo->NextGeneration();
		}
		
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
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
#include "Application.hpp"
#include "EvoLogic/Evo.h"

//#include "ServiceLocator/Locator.hpp"
//#include "ServiceLocator/DefaultResourceFont.hpp"

//#include "GUI/Widgets.hpp"

#include <ctime>
#include <iostream>

Application::Application():
generator(std::time(NULL)),
dist(-10, 10)
{
	window.create(sf::VideoMode(1280, 720), "Evo");
	//canvas = new Canvas(window);
	
	generator();
	
	//Locator::provideFont(new DefaultResourceFont());
	setupGUI();
	
	evo = new Evo(20, 0.1, 1, 0.1, 1);
}

Application::~Application()
{
	//delete canvas;
	
	//Locator::terminate();
}

void Application::run()
{
	sf::Clock clock;
	
	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			//canvas->handleEvent(event);
			
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

		//canvas->update(0.0f);
		
		window.clear();
		
		//canvas->draw(window);
		
		window.display();
		
		// Bull
		evo->NextGeneration();
		auto points = evo->Points();
		auto costs = evo->Costs();
	}
}

void Application::setupGUI()
{
	
}
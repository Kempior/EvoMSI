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
}

Application::~Application()
{
	delete canvas;
	
	Locator::terminate();
}

void Application::run()
{
	sf::Clock clock, frameClock;
	
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
		if(isSimulating && clock.getElapsedTime().asSeconds() >= 1.0f)
		{
			clock.restart();
			evo->NextGeneration();
		}
		
		std::this_thread::sleep_for(std::chrono::milliseconds(16 - frameClock.restart().asMilliseconds()));
	}
}

void Application::setupEvo()
{
	evo = new Evo(popSize, minX, maxX, minY, maxY);
	evo->mutationChance = mutationChance;
	evo->mutationMagnitude = mutationMagnitude;
	
	canvas->getWidget<Graph>("Graph1")->setPoints(evo->Points());
	canvas->getWidget<Graph>("Graph2")->setPoints(evo->Costs());
}

void Application::resetEvo()
{
	delete evo;
	evo = nullptr;
	
	canvas->getWidget<Graph>("Graph1")->setPoints(nullptr);
	canvas->getWidget<Graph>("Graph2")->setPoints(nullptr);
}

void Application::setupGUI()
{
	setupInfo();
	setupGraphs();
}

void Application::setupInfo()
{
	//Info Box
	Box *infoBox = new Box("InfoBox", sf::FloatRect(0.0f, 0.0f, 0.21875f, 1.0f));
	infoBox->setAnchor(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(), UPPER_LEFT);
	canvas->addChildWidget(infoBox);
	
	//Start Button
	Button *button = new Button("StartButton", sf::Vector2f(110.0f, 30.0f));
	button->setAnchor(sf::Vector2f(0.03f, 0.02f), sf::Vector2f(), UPPER_LEFT);
	infoBox->addChildWidget(button);
	
	Label *label = new Label("StartLabel", "Start");
	label->setAnchor(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(), MIDDLE);
	button->addChildWidget(label);
	
	button->setFunc([this, button]()
	{
		setupEvo();
		isSimulating = true;
		canvas->getWidget<Button>("StartButton")->setIsActive(false);
		canvas->getWidget<Button>("ResetButton")->setIsActive(true);
		canvas->getWidget<Button>("PauseButton")->setIsActive(true);
	});
	
	//Reset Button
	button = new Button("ResetButton", sf::Vector2f(110.0f, 30.0f));
	button->setAnchor(sf::Vector2f(0.03f, 0.02f), sf::Vector2f(), UPPER_LEFT);
	button->setIsActive(false);
	infoBox->addChildWidget(button);
	
	label = new Label("ResetLabel", "Reset");
	label->setAnchor(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(), MIDDLE);
	button->addChildWidget(label);
	
	button->setFunc([this]()
	{
		resetEvo();
		isSimulating = false;
		canvas->getWidget<Button>("StartButton")->setIsActive(true);
		canvas->getWidget<Button>("ResetButton")->setIsActive(false);
		canvas->getWidget<Button>("PauseButton")->setIsActive(false);
	});
	
	//Pause Button
	button = new Button("PauseButton", sf::Vector2f(110.0f, 30.0f));
	button->setAnchor(sf::Vector2f(0.97f, 0.02f), sf::Vector2f(), UPPER_RIGHT);
	button->setIsActive(false);
	infoBox->addChildWidget(button);
	
	label = new Label("PauseLabel", "Pause");
	label->setAnchor(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(), MIDDLE);
	button->addChildWidget(label);
	
	button->setFunc([this]()
	{
		isSimulating = !isSimulating;
		canvas->getWidget<Label>("PauseLabel")->setText(isSimulating ? "Pause" : "Unpause");
	});
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
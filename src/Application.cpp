#include "Application.hpp"
#include "EvoLogic/Evo.h"

#include "ServiceLocator/Locator.hpp"
#include "ServiceLocator/DefaultResourceFont.hpp"

#include "GUI/Widgets.hpp"

#include <ctime>
#include <thread>
#include <chrono>
#include <regex>

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
	
	Graph *g1 = canvas->getWidget<Graph>("Graph1");
	Graph *g2 = canvas->getWidget<Graph>("Graph2");
	
	g1->setPoints(evo->Points());
	g2->setPoints(evo->Costs());
	
	g1->otherGraph = g2;
	g2->otherGraph = g1;
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
	
	//PopSize Text Field
	TextField *textField = new TextField("PopSizeTextField", sf::Vector2f(80.0f, 30.0f));
	textField->setAnchor(sf::Vector2f(0.03f, 0.02f), sf::Vector2f(0.0f, 40.0f), UPPER_LEFT);
	infoBox->addChildWidget(textField);
	textField->setTextSourceFunc([this]() -> std::string
	{
		return std::to_string(popSize);
	});
	textField->setDataUpdateFunc([this, textField]()
	{
		popSize = atoi(textField->getText().c_str());
	});
	textField->setValidateFunc([](std::string str) -> bool
	{
		return std::regex_match(str, std::regex("^([4-9]|[1-9]\\d{1,})$"));
	});
	
	label = new Label("PopSizeLabel", "Pop Size");
	label->setAnchor(sf::Vector2f(1.0f, 0.5f), sf::Vector2f(5.0f, 0.0f), LEFT);
	textField->addChildWidget(label);
	
	//MutationChance Text Field
	textField = new TextField("MutationChanceTextField", sf::Vector2f(80.0f, 30.0f));
	textField->setAnchor(sf::Vector2f(0.03f, 0.02f), sf::Vector2f(0.0f, 80.0f), UPPER_LEFT);
	infoBox->addChildWidget(textField);
	textField->setTextSourceFunc([this]() -> std::string
	{
		return std::to_string(mutationChance).substr(0, 7);
	});
	textField->setDataUpdateFunc([this, textField]()
	{
		mutationChance = atof(textField->getText().c_str());
	});
	textField->setValidateFunc([](std::string str) -> bool
	{
		return std::regex_match(str, std::regex("([[:digit:]]+)(\\.(([[:digit:]]+)?))?"));
	});
	
	label = new Label("MutationChanceLabel", "Mutation Chance");
	label->setAnchor(sf::Vector2f(1.0f, 0.5f), sf::Vector2f(5.0f, 0.0f), LEFT);
	textField->addChildWidget(label);
	
	//MutationMagnitude Text Field
	textField = new TextField("MutationMagnitudeTextField", sf::Vector2f(80.0f, 30.0f));
	textField->setAnchor(sf::Vector2f(0.03f, 0.02f), sf::Vector2f(0.0f, 120.0f), UPPER_LEFT);
	infoBox->addChildWidget(textField);
	textField->setTextSourceFunc([this]() -> std::string
	{
		return std::to_string(mutationMagnitude).substr(0, 7);
	});
	textField->setDataUpdateFunc([this, textField]()
	{
		mutationMagnitude = atof(textField->getText().c_str());
	});
	textField->setValidateFunc([](std::string str) -> bool
	{
		return std::regex_match(str, std::regex("([[:digit:]]+)(\\.(([[:digit:]]+)?))?"));
	});
	
	label = new Label("MutationMagnitudeLabel", "Mutation Magnitude");
	label->setAnchor(sf::Vector2f(1.0f, 0.5f), sf::Vector2f(5.0f, 0.0f), LEFT);
	textField->addChildWidget(label);
	
	//MinX Text Field
	textField = new TextField("MinXTextField", sf::Vector2f(80.0f, 30.0f));
	textField->setAnchor(sf::Vector2f(0.03f, 0.02f), sf::Vector2f(0.0f, 160.0f), UPPER_LEFT);
	infoBox->addChildWidget(textField);
	textField->setTextSourceFunc([this]() -> std::string
	{
		return std::to_string(minX).substr(0, 7);
	});
	textField->setDataUpdateFunc([this, textField]()
	{
		minX = atof(textField->getText().c_str());
	});
	textField->setValidateFunc([this, textField](std::string str) -> bool
	{
		return std::regex_match(str, std::regex("([[:digit:]]+)(\\.(([[:digit:]]+)?))?")) && atof(textField->getText().c_str()) < maxX;
	});
	
	label = new Label("MinXLabel", "Min X");
	label->setAnchor(sf::Vector2f(1.0f, 0.5f), sf::Vector2f(5.0f, 0.0f), LEFT);
	textField->addChildWidget(label);
	
	//MaxX Text Field
	textField = new TextField("MaxXTextField", sf::Vector2f(80.0f, 30.0f));
	textField->setAnchor(sf::Vector2f(0.03f, 0.02f), sf::Vector2f(0.0f, 200.0f), UPPER_LEFT);
	infoBox->addChildWidget(textField);
	textField->setTextSourceFunc([this]() -> std::string
	{
		return std::to_string(maxX).substr(0, 7);
	});
	textField->setDataUpdateFunc([this, textField]()
	{
		maxX = atof(textField->getText().c_str());
	});
	textField->setValidateFunc([this, textField](std::string str) -> bool
	{
		return std::regex_match(str, std::regex("([[:digit:]]+)(\\.(([[:digit:]]+)?))?")) && minX < atof(textField->getText().c_str());
	});
	
	label = new Label("MaxXLabel", "Max X");
	label->setAnchor(sf::Vector2f(1.0f, 0.5f), sf::Vector2f(5.0f, 0.0f), LEFT);
	textField->addChildWidget(label);
	
	//MinY Text Field
	textField = new TextField("MinYTextField", sf::Vector2f(80.0f, 30.0f));
	textField->setAnchor(sf::Vector2f(0.03f, 0.02f), sf::Vector2f(0.0f, 240.0f), UPPER_LEFT);
	infoBox->addChildWidget(textField);
	textField->setTextSourceFunc([this]() -> std::string
	{
		return std::to_string(minY).substr(0, 7);
	});
	textField->setDataUpdateFunc([this, textField]()
	{
		minY = atof(textField->getText().c_str());
	});
	textField->setValidateFunc([this, textField](std::string str) -> bool
	{
		return std::regex_match(str, std::regex("([[:digit:]]+)(\\.(([[:digit:]]+)?))?")) && atof(textField->getText().c_str()) < maxY;
	});
	
	label = new Label("MinYLabel", "Min Y");
	label->setAnchor(sf::Vector2f(1.0f, 0.5f), sf::Vector2f(5.0f, 0.0f), LEFT);
	textField->addChildWidget(label);
	
	//MaxY Text Field
	textField = new TextField("MaxYTextField", sf::Vector2f(80.0f, 30.0f));
	textField->setAnchor(sf::Vector2f(0.03f, 0.02f), sf::Vector2f(0.0f, 280.0f), UPPER_LEFT);
	infoBox->addChildWidget(textField);
	textField->setTextSourceFunc([this]() -> std::string
	{
		return std::to_string(maxY).substr(0, 7);
	});
	textField->setDataUpdateFunc([this, textField]()
	{
		maxY = atof(textField->getText().c_str());
	});
	textField->setValidateFunc([this, textField](std::string str) -> bool
	{
		return std::regex_match(str, std::regex("([[:digit:]]+)(\\.(([[:digit:]]+)?))?")) && minY < atof(textField->getText().c_str());
	});
	
	label = new Label("MaxYLabel", "Max Y");
	label->setAnchor(sf::Vector2f(1.0f, 0.5f), sf::Vector2f(5.0f, 0.0f), LEFT);
	textField->addChildWidget(label);
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
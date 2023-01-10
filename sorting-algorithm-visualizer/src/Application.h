#pragma once
#include <iostream> // for debugging
#include <chrono> // for sleeping
#include <thread> // for sleeping
#include <string>

#include "Renderer.h"
#include "NodeHandler.h"

class Application
{
public:
	Application(int windowWidth, int windowHeight);

	Renderer renderer;
	NodeHandler nodeHandler;
	SortingAlgorithm currentAlgorithm;
	bool close;

	bool handleEvents();
	void getUserInput();
	void sort();
	void displayStats();
	void restart();
	void run();
};
#pragma once
#include <SFML/Graphics.hpp>

#include "Node.h"

class Renderer
{
private:
	sf::RenderWindow window;
public:
	Renderer() {}

	void create(int width, int height);
	void render(Node* node);
	void clear();
	void update();
	bool pollEvent(sf::Event& event);
	void close();
	void displayText(int x, int y, std::string message);
	
	int getWindowWidth();
	int getWindowHeight();
};
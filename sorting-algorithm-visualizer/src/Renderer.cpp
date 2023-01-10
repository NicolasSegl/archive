#include "Renderer.h"

void Renderer::create(int width, int height) { window.create(sf::VideoMode(width, height), "Sorting Algorithm Visualizer"); }

void Renderer::render(Node* node) {	window.draw(node->rect); }

void Renderer::clear()
{ 
	sf::Color bgColour = sf::Color(30, 30, 30);
	window.clear(bgColour); 
}

void Renderer::update() { window.display(); }

int Renderer::getWindowWidth() { return window.getSize().x; }

int Renderer::getWindowHeight() { return window.getSize().y; }

bool Renderer::pollEvent(sf::Event& event) { return window.pollEvent(event); }

void Renderer::close() { window.close(); }

void Renderer::displayText(int x, int y, std::string message)
{
	sf::Font font;
	font.loadFromFile("times-new-roman.ttf");
	sf::Text text(message, font, 20);
	text.setFillColor(sf::Color::White);
	text.setPosition(sf::Vector2f(x, y));
	window.draw(text);
}
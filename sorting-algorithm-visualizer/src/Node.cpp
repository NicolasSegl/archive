#include "Node.h"

Node::Node(int x, int y, int width, int height, int value, int n)
{
	rectWidth = width;
	rectHeight = height;
	nodeValue = value;
	numOfNodes = n;

	setupRect(x, y, numOfNodes);
}

void Node::setupRect(int x, int y, int n)
{
	rect.setSize(sf::Vector2f(rectWidth, rectHeight));
	rect.setFillColor(sf::Color::White);
	if (numOfNodes < 512)
		rect.setOutlineThickness(1.f);
	rect.setOutlineColor(sf::Color::Black);

	rect.setPosition(sf::Vector2f(x, y));
}

Node Node::operator=(const Node& node)
{
	rectHeight = node.rectHeight;
	nodeValue = node.nodeValue;

	setupRect(rect.getPosition().x, node.rect.getPosition().y, numOfNodes);
	return *this;
}
#pragma once
#include <sfml/Graphics.hpp>
#include <iostream>

class Node
{
public:
	Node() {}
	Node(int x, int y, int width, int height, int value, int numOfNodes);
	int rectWidth, rectHeight, nodeValue, numOfNodes;
	sf::RectangleShape rect;

	void setupRect(int x, int y, int numOfNodes);
	Node operator=(const Node& node);
};
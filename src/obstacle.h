#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>


class Obstacle {

	float mWidth, mHeight, mLeft, mTop;
	int mHitedEdge;

public:
	std::vector<sf::Vertex> points;

	Obstacle();
	sf::Vector2f getClosestPoint(sf::Vector2f point);
	bool bbCollision(sf::Vector2f point);
	bool contains(sf::Vector2f point);
	void addPoint(sf::Vector2f coords);
};
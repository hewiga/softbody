#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

class MassPoint {

	sf::Vector2f mPosition;
	sf::Vector2f mVelocity;
	sf::Vector2f mForce;

public:
	MassPoint(sf::Vector2f position);
	sf::Vector2f getPosition();
	sf::Vector2f getVelocity();
	sf::Vector2f getForce();
	void zero();
	void setPosition(sf::Vector2f newPosition);
	void setVelocity(sf::Vector2f newVelocity);
	void updateForce(sf::Vector2f force, sf::Time dt);
	void updatePosition(sf::Time dt, sf::Vector2f move);
	void updatePositionX(sf::Time dt, float moveX);
	void updatePositionY(sf::Time dt, float moveY);
};
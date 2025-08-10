#include "massPoint.h"

MassPoint::MassPoint(sf::Vector2f position) {
	mPosition = position;
	mVelocity = sf::Vector2f(0, 0);
	mForce = sf::Vector2f(0, 0);
}

sf::Vector2f MassPoint::getPosition() {
	return mPosition;
}
sf::Vector2f MassPoint::getVelocity() {
	return mVelocity;
}
sf::Vector2f MassPoint::getForce() {
	return mForce;
}

void MassPoint::setPosition(sf::Vector2f newPosition) {
	mPosition = newPosition;
}
void MassPoint::setVelocity(sf::Vector2f newVelocity) {
	mVelocity = newVelocity;
}
void MassPoint::updateForce(sf::Vector2f force, sf::Time dt) {
	mForce += force * dt.asSeconds();
}
void MassPoint::zero() {
	mForce = sf::Vector2f(0, 0);
}

void MassPoint::updatePosition(sf::Time dt, sf::Vector2f move) {

	mPosition += move * dt.asSeconds();
}

void MassPoint::updatePositionX(sf::Time dt, float moveX) {
	
	mPosition.x += moveX * dt.asSeconds();
}
void MassPoint::updatePositionY(sf::Time dt, float moveY) {

	mPosition.y += moveY * dt.asSeconds();
}
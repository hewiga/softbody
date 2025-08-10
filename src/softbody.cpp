#include "softbody.h"

void SoftBody::addMassPoint(sf::Vector2f position) {
	
	mMassPoints.push_back(MassPoint(position));
}

void SoftBody::addMassPoint(int x, int y) {

	mMassPoints.push_back(MassPoint(sf::Vector2f(x, y)));
}

void SoftBody::addConnection(int point1, int point2) {

	mConnections.push_back(Connection{
		&mMassPoints[point1],
		&mMassPoints[point2],
		sqrt((float)pow(mMassPoints[point1].getPosition().x - mMassPoints[point2].getPosition().x, 2) + \
			 (float)pow(mMassPoints[point1].getPosition().y - mMassPoints[point2].getPosition().y, 2))
	});
}

void SoftBody::countConnectionForce(int index, sf::Time dt) {

	sf::Vector2f ropeForce;
	float distanceX = mConnections[index].point1->getPosition().x - mConnections[index].point2->getPosition().x;
	float distanceY = mConnections[index].point1->getPosition().y - mConnections[index].point2->getPosition().y;
	float distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));
	if (distance == 0) {
	    distance = 0.0001;
	}
	
	//count direction vector
	sf::Vector2f normalized(distanceX / distance, distanceY / distance);
	
	ropeForce.x = (10000 * (distance - mConnections[index].length));
	ropeForce.y = (10000 * (distance - mConnections[index].length));
	
	ropeForce.x += normalized.x * (mConnections[index].point1->getVelocity().x - mConnections[index].point2->getVelocity().x) * 200;
	ropeForce.y += normalized.y * (mConnections[index].point1->getVelocity().y - mConnections[index].point2->getVelocity().y) * 200;
	
	//apply rope force on each mass point
	mConnections[index].point1->updateForce(sf::Vector2f(ropeForce.x * -normalized.x, ropeForce.y * -normalized.y), dt);
	mConnections[index].point2->updateForce(sf::Vector2f(ropeForce.x * normalized.x, ropeForce.y * normalized.y), dt);
}

void SoftBody::checkCollistion(int index, std::vector<Obstacle> obstacles) {

	float xPosition = mMassPoints[index].getPosition().x;
	float yPosition = mMassPoints[index].getPosition().y;
	
	//check collision with floor
	if (yPosition > 900) {
		mMassPoints[index].setPosition(sf::Vector2f(xPosition, 899.9));
		mMassPoints[index].setVelocity(sf::Vector2f(mMassPoints[index].getVelocity().x, mMassPoints[index].getVelocity().y * -0.5));
	}

	for (int i = 0; i < obstacles.size(); i++) {
		if (obstacles[i].bbCollision(mMassPoints[index].getPosition())) {
			//if point is inside obstacles bounding box than check more complex collision

			if (obstacles[i].contains(mMassPoints[index].getPosition())) {
				//if object is inside shape

				//get normalized vector perpendicular to hited edge
				sf::Vector2f newPosition = obstacles[i].getClosestPoint(mMassPoints[index].getPosition());

				float moveDistanceX = newPosition.x - xPosition;
				float moveDistanceY = newPosition.y - yPosition;
				float moveDistance = sqrt(pow(moveDistanceX, 2) + pow(moveDistanceY, 2));
				if (moveDistance == 0) {
					moveDistance = 0.0001;
				}
				sf::Vector2f moveDirection(moveDistanceX / moveDistance, moveDistanceY / moveDistance);

				//move point on edge
				mMassPoints[index].setPosition(obstacles[i].getClosestPoint(mMassPoints[index].getPosition()));

				//count new velocity
				float dotProd = mMassPoints[index].getVelocity().x * moveDirection.x + mMassPoints[index].getVelocity().y * moveDirection.y;
				mMassPoints[index].setVelocity(mMassPoints[index].getVelocity() - 2 * dotProd * moveDirection);
			}
		}
	}
}

void SoftBody::update(sf::Time dt, sf::Vector2f gravity, std::vector<Obstacle> obstacles) {

	//count force for each connection
	for (auto i = 0; i < mConnections.size(); i++) {
		countConnectionForce(i, dt);
	}

	for (int i = 0; i < mMassPoints.size(); i++) {

		mMassPoints[i].updateForce(gravity, dt);

		//count new position
		sf::Vector2f newVelocity = mMassPoints[i].getVelocity() + (mMassPoints[i].getForce() * dt.asSeconds());
		sf::Vector2f avgVelocity = sf::Vector2f(
			(newVelocity.x + mMassPoints[i].getVelocity().x) / 2,
			(newVelocity.y + mMassPoints[i].getVelocity().y) / 2
		);
		mMassPoints[i].updatePosition(dt, avgVelocity);
		mMassPoints[i].setVelocity(newVelocity);

		checkCollistion(i, obstacles);

		mMassPoints[i].zero();
	}
}

void SoftBody::drawBody(sf::RenderTexture& canvas) {

	sf::Vertex line[2];
	for (auto i = 0; i < mConnections.size(); i++) {
		line[0] = sf::Vertex(mConnections[i].point1->getPosition());
		line[1] = sf::Vertex(mConnections[i].point2->getPosition());
		canvas.draw(line, 2, sf::Lines);
	}
}
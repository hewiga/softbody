#include "obstacle.h"

#include <math.h>

Obstacle::Obstacle() {

	mTop = 0;
	mLeft = 0;
	mWidth = 0;
	mHeight = 0;
	mHitedEdge = 0;
}

sf::Vector2f Obstacle::getClosestPoint(sf::Vector2f point) {

	sf::Vector2f closestPoint;
	float closestDistance = FLT_MAX;

	for (int i = 0; i < points.size() - 1; i++) {

		sf::Vector2f v1 = points[i + 1].position - points[i].position;
		sf::Vector2f v2 = point - points[i].position;
		
		float d = (v1.x * v2.x + v1.y * v2.y) / (v1.x * v1.x + v1.y * v1.y);

		sf::Vector2f p;
		if (d > 1) {
			p = points[i + 1].position;
		}
		else if (d < 0) {
			p = points[i].position;
		}
		else {
			p = points[i].position + v1 * d;
		}
		
		float distance = sqrt(pow(p.x - point.x, 2) + pow(p.y - point.y, 2));

		if (distance < closestDistance) {
			closestPoint = p;
			closestDistance = distance;
			mHitedEdge = i;
		}
	}
	return closestPoint;
}

bool Obstacle::contains(sf::Vector2f point) {

	int numVertices = points.size();
	float x = point.x;
	float y = point.y;
	bool inside = false;

	sf::Vector2f p1 = points[0].position;
	for (int i = 0; i < numVertices; i++) {
		sf::Vector2f p2 = points[i % numVertices].position;

		if (y > std::min(p1.y, p2.y)) {
			if (y <= std::max(p1.y, p2.y)) {

				if (x <= std::max(p1.x, p2.x)) {

					float xIntersection = (y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y) + p1.x;
					if ((p1.x == p2.x) || (x <= xIntersection)) {
						inside = !inside;
					}
				}
			}
		}
		p1 = p2;
	}
	return inside;
}

bool Obstacle::bbCollision(sf::Vector2f point) {
	//check if the bounding box contains point

	if ((point.x < mLeft) || (point.x > mLeft + mWidth)) {
		
		return false;
	}
	if ((point.y < mTop) || (point.y > mTop + mHeight)) {
		
		return false;
	}
	return true;
}

void Obstacle::addPoint(sf::Vector2f coords) {

	if (points.size() > 3) {
		//if arrays length is bigger than 3 then put new point before last element to close shape
		//[point1][point2][point3] [NEW_POINT] [point1]
		points.insert(points.end() - 1, coords);
	}
	else if (points.size() == 2) {
		//if arrays length is equal 2 then add new point and repeat first one to close the shape
		points.push_back(coords);
		points.push_back(points[0]);
	}
	else {
		points.push_back(coords);
		if (points.size() == 1) {
			mLeft = coords.x;
			mTop = coords.y;
		}
	}

	//update bounding box
	if (coords.x > mLeft + mWidth) mWidth = coords.x - mLeft;
	else if (coords.x < mLeft) {
		mWidth = mLeft - coords.x + mHeight;
		mLeft = coords.x;
	} 

	if (coords.y > mTop + mHeight) mHeight = coords.y - mTop;
	else if (coords.y < mTop) {
		mHeight = mTop - coords.y + mHeight;
		mTop = coords.y;
	}
	
}
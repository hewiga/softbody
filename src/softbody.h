#pragma once
#include <vector>

#include <cmath>

#include "massPoint.h"
#include "obstacle.h"

struct Connection {
    MassPoint* point1;
    MassPoint* point2;
    float length;
};

class SoftBody {

	std::vector<MassPoint> mMassPoints;
    std::vector<Connection> mConnections;
    void countConnectionForce(int index, sf::Time dt);
    void checkCollistion(int index, std::vector<Obstacle> obstacles);

public:
    void addMassPoint(sf::Vector2f position);
    void addMassPoint(int x, int y);
    void addConnection(int point1, int point2);
    void update(sf::Time dt, sf::Vector2f gravity, std::vector<Obstacle> obstacles);
    void drawBody(sf::RenderTexture& canvas);
};
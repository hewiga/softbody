#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#include "softbody.h"
#include "obstacle.h"


const int WINDOWS_WIDTH = 1000;
const int WINDOWS_HEIGHT = 900;
sf::Vector2f gravity(0, 5000);

void preparePoints(SoftBody& sb, std::vector<Obstacle>& obstacles) {

    sb.addMassPoint(50, 0);
    sb.addMassPoint(100, 0);
    sb.addMassPoint(100, 50);
    sb.addMassPoint(50, 50);
    sb.addMassPoint(50, 100);
    sb.addMassPoint(100, 100);

    sb.addConnection(0, 1);
    sb.addConnection(1, 2);
    sb.addConnection(2, 3);
    sb.addConnection(3, 0);
    sb.addConnection(3, 1);
    sb.addConnection(0, 2);
    sb.addConnection(3, 5);
    sb.addConnection(4, 2);
    sb.addConnection(5, 4);
    sb.addConnection(5, 2);
    sb.addConnection(4, 3);

    obstacles.push_back(Obstacle());
    obstacles[0].addPoint(sf::Vector2f(20, 150));
    obstacles[0].addPoint(sf::Vector2f(100, 350));
    obstacles[0].addPoint(sf::Vector2f(400, 250));

    obstacles.push_back(Obstacle());
    obstacles[1].addPoint(sf::Vector2f(800, 150));
    obstacles[1].addPoint(sf::Vector2f(700, 350));
    obstacles[1].addPoint(sf::Vector2f(500, 350));

    obstacles.push_back(Obstacle());
    obstacles[2].addPoint(sf::Vector2f(20, 400));
    obstacles[2].addPoint(sf::Vector2f(150, 750));
    obstacles[2].addPoint(sf::Vector2f(400, 550));

    obstacles.push_back(Obstacle());
    obstacles[3].addPoint(sf::Vector2f(750, 450));
    obstacles[3].addPoint(sf::Vector2f(700, 650));
    obstacles[3].addPoint(sf::Vector2f(500, 650));

    obstacles.push_back(Obstacle());
    obstacles[4].addPoint(sf::Vector2f(250, 750));
    obstacles[4].addPoint(sf::Vector2f(550, 750));
    obstacles[4].addPoint(sf::Vector2f(450, 850));
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOWS_WIDTH, WINDOWS_HEIGHT), "SFML works!");
    sf::RenderTexture canvas;
    if (!canvas.create(WINDOWS_WIDTH, WINDOWS_HEIGHT)) {
        return -1;
    }
    window.setFramerateLimit(60);

    SoftBody sb;
    std::vector<Obstacle> obstacles;
    preparePoints(sb, obstacles);

    sf::Clock deltaClock;
    sf::Time dt;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //prepare canvas 
        canvas.clear();
        sb.update(dt, gravity, obstacles);
        sb.drawBody(canvas);

        for (int i = 0; i < obstacles.size(); i++) {
            canvas.draw(&obstacles[i].points[0], obstacles[i].points.size(), sf::LineStrip);
        }
        canvas.display();

        //prepare window
        window.clear();
        sf::Sprite canvas_sprite(canvas.getTexture());

        //draw canvas sprite
        window.draw(canvas_sprite);
        window.display();
        dt = deltaClock.restart();
    }

    return 0;
}
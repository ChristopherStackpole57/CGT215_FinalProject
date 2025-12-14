// Christopher Stackpole, 12/13/2025

#ifndef BODY_H
#define BODY_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class Body
{
public:
	Body();
	Body(sf::Vector2f position);

	void SetPosition(sf::Vector2f position);
	sf::Vector2f GetPosition();

	void SetVelocity(sf::Vector2f velocity);
	sf::Vector2f GetVelocity();

	void SetAcceleration(sf::Vector2f acceleration);
	sf::Vector2f GetAcceleration();

	void SetActive(bool active);
	bool GetActive();
private:
	bool active = true;

	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2f acceleration;
};

#endif
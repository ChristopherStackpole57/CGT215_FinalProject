// Christopher Stackpole, 12/09/2025

#ifndef ARC_H
#define ARC_H

#include <iostream>

#include <SFML/Main.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "Services.h"
#include "GameServices.h"

#include "GameObjects/GameObjectInterface.h"

#include "GameObjects/Laser.h"

class ARC : public IGameObject
{
public:
	ARC();

	void Start() override;
	void Shutdown() override;
	void Tick(float dt) override;

	sf::Vector2f GetPosition();
	void SetPosition(sf::Vector2f position);

	sf::Angle GetRotation();
	void SetRotation(sf::Angle rotation);
	void Rotate(sf::Angle rotation);
private:
	std::unique_ptr<sf::Sprite> sprite;
	std::string path = "Resources/cannon.bmp";

	bool debounce = false;									// Whether or not the cannon can fire
	int shot_time = 0;										// The previous game time a shot was registered
	float fire_rate = 20;									// Max fire rate / second
	float turn_rate = 100;									// Measured in degrees / second

	bool left_arrow_down = false;							// Track arrow states
	bool right_arrow_down = false;
};

#endif
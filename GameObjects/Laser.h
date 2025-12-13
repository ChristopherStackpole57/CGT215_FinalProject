// Christopher Stackpole, 12/11/2025

#ifndef LASER_H
#define LASER_H

#include "Services.h"

#include "GameObjects/GameObjectInterface.h"

class Laser : public IGameObject
{
public:
	Laser();

	void Start() override;
	void Shutdown() override;
	void Tick(float dt) override;

	void SetPosition(sf::Vector2f position) override;
	sf::Vector2f GetPosition() override;
	void SetRotation(sf::Angle angle);
private:
	std::unique_ptr<sf::Sprite> sprite;
	std::string path = "Resources/laser.bmp";
};

#endif
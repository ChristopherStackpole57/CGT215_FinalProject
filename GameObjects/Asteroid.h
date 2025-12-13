// Christopher Stackpole, 12/11/2025

#ifndef ASTEROID_H
#define ASTEROID_H

#include "Services.h"

#include "GameObjects/GameObjectInterface.h"

class Asteroid : public IGameObject
{
public:
	Asteroid();

	void Start() override;
	void Shutdown() override;
	void Tick(float dt) override;
	
	void SetPosition(sf::Vector2f position) override;
	sf::Vector2f GetPosition() override;
private:
	std::unique_ptr<sf::Sprite> sprite;
	std::string path = "Resources/asteroid.bmp";
};

#endif
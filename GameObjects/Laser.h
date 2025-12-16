// Christopher Stackpole, 12/11/2025

#ifndef LASER_H
#define LASER_H

#include "Services.h"
#include "GameServices.h"

#include "GameObjects/GameObjectInterface.h"

constexpr float LASER_DAMAGE_SWORD = 2.5f;
constexpr float LASER_DAMAGE_ARC = 50.f;
constexpr float LASER_SPEED = 500.f;

class Laser : public IGameObject
{
public:
	Laser();

	void Start() override;
	void Shutdown() override;
	void Tick(float dt) override;

	void SetPosition(sf::Vector2f position) override;
	sf::Vector2f GetPosition() override;
	sf::Vector2f GetSize() override;
	void SetRotation(sf::Angle angle);

	void SetDamage(float amount);
	float GetDamage();
private:
	float damage = 0.f;

	std::unique_ptr<sf::Sprite> sprite;
	std::string path = "Resources/laser.bmp";
};

#endif
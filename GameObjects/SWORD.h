// Christopher Stackpole, 12/13/2025

#ifndef SWORD_H
#define SWORD_H

#include <cfloat>
#include <cmath>
#include <iostream>
#include <memory>

#include "Services.h"
#include "Services/GameServices/AsteroidService/AsteroidService.h"
#include "Services/GameServices/SwordService/SwordService.h"
#include "Services/GameServices/PoolService/PoolService.h"

#include "GameObjects/GameObjectInterface.h"
#include "GameObjects/Asteroid.h"

constexpr float SWORD_BASE_HEALTH = 25.f;
constexpr float SWORD_BASE_FIRERATE = 1.f;
constexpr float SWORD_SPRITE_SCALE = 0.25f;

class SWORD : public IGameObject
{
public:
	SWORD();

	void Start() override;
	void Shutdown() override;
	void Tick(float dt) override;

	void SetPosition(sf::Vector2f position) override;
	sf::Vector2f GetPosition() override;
	void SetRotation(sf::Angle angle);
	sf::Angle GetRotation();
	sf::Vector2f GetSize() override;

	void Hit(float damage);
	void ResetHealth();
private:
	float health = SWORD_BASE_HEALTH;

	bool debounce = false;
	int shot_time = 0;
	int fire_rate = SWORD_BASE_FIRERATE;
	float rangesq = 200000;
	float laser_speed = 500.f;

	std::unique_ptr<sf::Sprite> sprite;
	std::string path = "Resources/sword.bmp";
};

#endif
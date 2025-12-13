// Christopher Stackpole, 12/13/2025

#ifndef SCOOP_H
#define SCOOP_H

#include <memory>

#include "Services.h"

#include "GameObjects/GameObjectInterface.h"

class SCOOP : public IGameObject
{
public:
	SCOOP();

	void Start() override;
	void Shutdown() override;
	void Tick(float dt) override;

	void SetPosition(sf::Vector2f position) override;
	sf::Vector2f GetPosition() override;
private:
	std::unique_ptr<sf::Sprite> sprite;
	std::string path = "Resources/scoop.bmp";
};

#endif
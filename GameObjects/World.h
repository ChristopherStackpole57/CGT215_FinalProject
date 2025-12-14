// Christopher Stackpole, 12/08/2025

#ifndef WORLD_H
#define WORLD_H

#include "Services/ServiceManager.h"
#include "Services/EngineServices/CallService/CallService.h"
#include "Services/EngineServices/RenderService/RenderObject.h"
#include "Services/EngineServices/RenderService/RenderService.h"
#include "Services/EngineServices/ResourceService/ResourceService.h"

#include "GameObjects/GameObjectInterface.h"

class World : public IGameObject
{
public:
	World();

	void Start() override;
	void Shutdown() override;
	void Tick(float dt);

	sf::Vector2f GetPosition() override;
	void SetPosition(sf::Vector2f position) override;

	sf::Vector2f GetSize() override;
private:
	std::unique_ptr<sf::Sprite> sprite;
	std::string path = "Resources/planet.bmp";
};

#endif
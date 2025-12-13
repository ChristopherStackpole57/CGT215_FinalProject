// Christopher Stackpole, 12/06/2025

#ifndef IGAMEOBJECT_H
#define IGAMEOBJECT_H

#include <SFML/Graphics.hpp>

#include "Services.h"

#include "Services/EngineServices/RenderService/RenderObject.h"

class IGameObject
{
public:
	virtual ~IGameObject() = default;
	
	virtual void Start() = 0;
	virtual void Shutdown() = 0;
	virtual void Tick(float dt) {};

	virtual sf::Vector2f GetPosition() = 0;
	virtual void SetPosition(sf::Vector2f position) = 0;

	virtual void Move() {};
protected:
	sf::Vector2f position = sf::Vector2f(0, 0);
	RenderObject render_object;
};

#endif
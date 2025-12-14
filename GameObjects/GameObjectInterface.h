// Christopher Stackpole, 12/06/2025

#ifndef IGAMEOBJECT_H
#define IGAMEOBJECT_H

#include <SFML/Graphics.hpp>

#include "Services/EngineServices/PhysicsService/Body.h"
#include "Services/EngineServices/RenderService/RenderObject.h"

class IGameObject
{
public:
	virtual ~IGameObject() = default;
	
	virtual void Start() = 0;
	virtual void Shutdown() = 0;
	virtual void Tick(float dt) {};

	virtual void SetPosition(sf::Vector2f position) = 0;
	virtual sf::Vector2f GetPosition() = 0;

	virtual void SetVelocity(sf::Vector2f velocity)
	{
		if (body)
		{
			body->SetVelocity(velocity);
		}
	};
	virtual sf::Vector2f GetVelocity()
	{
		if (body)
		{
			return body->GetVelocity();
		}
	};

	virtual sf::Vector2f GetSize() = 0;

	virtual void Move() {};

	virtual void SetActive(bool active)
	{
		this->active = active;
		render_object.active = active;

		if (body)
		{
			body->SetActive(active);
		}
	}
	virtual bool GetActive()
	{
		return active;
	}
protected:
	bool active = true;

	sf::Vector2f position = sf::Vector2f(0.f, 0.f);
	
	Body* body;
	RenderObject render_object;
};

#endif
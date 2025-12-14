// Christopher Stackpole, 12/13/2025

#ifndef PHYSICS_SERVICE_H
#define PHYSICS_SERVICE_H

#include <memory>
#include <vector>

#include "Services/ServiceInterface.h"

#include "Services/EngineServices/PhysicsService/Body.h"

#include "GameObjects/GameObjectInterface.h"

class PhysicsService : public IService
{
public:
	void Start() override;
	void Shutdown() override;
	void Tick(float dt) override;

	Body* RegisterPhysicsObject(IGameObject* game_object);
private:
	std::vector<std::unique_ptr<Body>> bodies;
};

#endif
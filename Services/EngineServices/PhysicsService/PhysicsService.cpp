// Christopher Stackpole, 12/13/2025

#include "PhysicsService.h"

// Service Behavior
void PhysicsService::Start()
{

}
void PhysicsService::Shutdown()
{

}
void PhysicsService::Tick(float dt)
{
	// Semi - Implicit Euler Integration

	float time = dt / 1000.f;
	for (const auto& body_ptr : bodies)
	{
		Body& body = *body_ptr;

		if (body.GetActive())
		{
			body.SetVelocity(
				body.GetVelocity() + body.GetAcceleration() * time
			);
			body.SetPosition(
				body.GetPosition() + body.GetVelocity() * time
			);
			body.SetAcceleration(sf::Vector2f());
		}
	}
}

// PhysicsService Behavior
Body* PhysicsService::RegisterPhysicsObject(IGameObject* game_object)
{
	std::unique_ptr<Body> body = std::make_unique<Body>(
		game_object->GetPosition()
	);

	Body* raw = body.get();
	bodies.emplace_back(std::move(body));
	return raw;
}
// Christopher Stackpole, 12/13/2025

#include <iostream>

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

	// Test Pairs for Overlaps
	for (int i = 0; i < bodies.size(); i++)
	{
		Body& a = *bodies[i];
		if (!(a.GetActive()))
			continue;

		for (int j = i + 1; j < bodies.size(); j++)
		{
			Body& b = *bodies[j];
			if (!(b.GetActive()))
				continue;
			bool overlap = TestOverlap(a, b);

			if (overlap)
			{
				// invoke overlap callbacks

				a.InvokeOverlap(b);
				b.InvokeOverlap(a);
			}
		}
	}
}

// PhysicsService Behavior
Body* PhysicsService::RegisterPhysicsObject(IGameObject* game_object)
{
	std::unique_ptr<Body> body = std::make_unique<Body>();
	body->SetOwner(game_object);

	Body* raw = body.get();
	bodies.emplace_back(std::move(body));
	return raw;
}
bool PhysicsService::TestOverlap(Body& a, Body& b)
{
	// Some secret c++ voodoo magic the interwebs gave me to go along with our std::variant collider
	return std::visit(
		[&](auto&& shape_a, auto&& shape_b)
		{
			return ResolveOverlap(a, shape_a, b, shape_b);
		},
		a.GetCollider(),
		b.GetCollider()
	);
}
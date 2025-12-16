// Christopher Stackpole, 12/12/2025

#include <iostream>

#include "AsteroidService.h"

#include "GameObjects/Asteroid.h"

void AsteroidService::Start()
{
}
void AsteroidService::Shutdown()
{

}
void AsteroidService::Tick(float dt)
{
	CallService* call_service = Services().Get<CallService>();

	int decayed_chance = (int)(ASTEROID_SPAWN_CHANCE * std::exp(-1 * ASTEROID_SPAWN_DECAY_FACTOR * call_service->GetGameTime()));
	int num = std::rand() % (decayed_chance + 1);
	if (num == 1)
	{ 
		// Generate asteroid
		RenderService* render_service = Services().Get<RenderService>();
		PoolService* pool_service = Services().Get<PoolService>();

		Asteroid* asteroid = pool_service->Get<Asteroid>();
		asteroids.insert(asteroid);

		asteroid->ResetHealth();
		asteroid->ResetSize();

		int width = render_service->GetWindowSize().x;
		int height = render_service->GetWindowSize().y;
		int hw = width / 2;
		int hh = height / 2;

		// determine longer side
		float dist = sf::Vector2f(hw, hh).length();
		sf::Vector2f pos = sf::Vector2f(0, -1 * dist);
		pos = pos.rotatedBy(
			sf::Angle(
				sf::degrees(
					std::rand() % 360
				)
			)
		);
		pos += sf::Vector2f(hw, hh);

		asteroid->SetPosition(pos);
		asteroid->SetVelocity((sf::Vector2f(hw, hh) - pos).normalized() * ASTEROID_SPEED);
	}
}

std::vector<Asteroid*> AsteroidService::GetAsteroids()
{
	std::vector<Asteroid*> active_asteroids;

	for (Asteroid* asteroid : asteroids)
	{
		if (asteroid->GetActive())
		{
			active_asteroids.push_back(asteroid);
		}
	}

	return active_asteroids;
}
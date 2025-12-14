// Christopher Stackpole, 12/12/2025

#include <iostream>

#include "AsteroidService.h"

void AsteroidService::Start()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
}
void AsteroidService::Shutdown()
{

}
void AsteroidService::Tick(float dt)
{
	int num = std::rand() % chance;
	if (num == 1)
	{
		// Generate asteroid
		RenderService* render_service = Services().Get<RenderService>();
		PoolService* pool_service = Services().Get<PoolService>();

		Asteroid* asteroid = pool_service->Get<Asteroid>();

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
		asteroid->SetVelocity((sf::Vector2f(hw, hh) - pos).normalized() * asteroid_speed);
	}
}
// Christopher Stackpole, 12/12/2025

#ifndef ASTEROID_SERVICE_H
#define ASTEROID_SERVICE_H

#include <vector>

#include "Services/ServiceInterface.h"

#include "GameObjects/Asteroid.h"

class AsteroidService : public IService
{
public:
	void Start() override;
	void Shutdown() override;
	void Tick(float dt) override;

private:
	std::vector<Asteroid*> asteroids;
};

#endif
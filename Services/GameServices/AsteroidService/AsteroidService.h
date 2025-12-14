// Christopher Stackpole, 12/12/2025

#ifndef ASTEROID_SERVICE_H
#define ASTEROID_SERVICE_H

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>

#include "Services/ServiceInterface.h"

#include "RenderService.h"

#include "Services/GameServices/PoolService/PoolService.h"

#include "GameObjects/Asteroid.h"

class AsteroidService : public IService
{
public:
	void Start() override;
	void Shutdown() override;
	void Tick(float dt) override;
private:
	int chance = 10000;
	float asteroid_speed = 100.f;
};

#endif
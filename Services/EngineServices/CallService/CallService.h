// Christopher Stackpole, 12/06/2025

#ifndef CALL_SERVICE_H
#define CALL_SERVICE_H

#include <iterator>

#include "Services/ServiceInterface.h"
#include "GameObjects/GameObjectInterface.h"

#include "CallOrder.h"

class CallService : public IService
{
public:
	void Start() override;
	void Shutdown() override;
	void Tick(float dt);

	void SetServiceStartupPriority(IService* service, int priority);
	void SetServiceTickPriority(IService* service, int priority);

	void SetObjectStartupPriority(IGameObject* game_object, int priority);
	void SetObjectTickPriority(IGameObject* game_object, int priority);

	int GetGameTime();

	void Pause();
	void Unpause();
private:
	CallOrder<IService*> service_startup_order;
	CallOrder<IService*> service_tick_order;

	CallOrder<IGameObject*> object_startup_order;
	CallOrder<IGameObject*> object_tick_order;

	bool paused = false;
	int game_time = 0;								// Simple accumulation of the number of ms the game has been running
};

// Startup Priority Categories
constexpr int CLST_PRELOAD_CACHE = 1;
constexpr int CLST_BASIC_SERVICE = 0;
constexpr int CLST_DEPENDANT = -1;
constexpr int CLST_BASIC_GAMEOBJECT = -2;

// Tick Priority Categories
constexpr int CLT_PREFRAME = 1;
constexpr int CLT_BASIC = 0;
constexpr int CLT_FRAMERENDER = -1;
constexpr int CLT_POSTFRAME = -2;

// Shutdown Priority Categories
constexpr int CLSH_BASIC = 0;

#endif
// Christopher Stackpole, 12/06/2025

#include <iostream>

#include "CallService.h"

std::vector<int> merge_priority_list(std::vector<int> v1, std::vector<int> v2)
{
	std::vector<int> merge;
	std::set_union(
		v1.begin(), v1.end(),
		v2.begin(), v2.end(),
		std::back_inserter(merge)
	);

	return merge;
}

// Service Behavior
void CallService::Start()
{
	// Get startup priorities from the two CallOrders and then go through the priorities in order and call the relevant code
	std::vector<int> service_priorities = service_startup_order.GetPriorities();
	std::vector<int> object_priorities = object_startup_order.GetPriorities();

	// Get the sorted union of both sets
	std::vector<int> merge = merge_priority_list(service_priorities, object_priorities);

	// Loop through merged priority list and call from highest number to lowest number
	for (int i = merge.size() - 1; i >= 0; i--)
	{
		int priority = merge[i];
		for (IService* service : service_startup_order.GetEntriesOfPriority(priority))
		{
			service->Start();
		}
		for (IGameObject* game_object : object_startup_order.GetEntriesOfPriority(priority))
		{
			game_object->Start();
		}
	}
}
void CallService::Shutdown()
{

}
void CallService::Tick(float dt)
{
	game_time += dt;

	// Get tick priorities
	std::vector<int> service_priorities = service_tick_order.GetPriorities();
	std::vector<int> object_priorities = object_tick_order.GetPriorities();

	// Get union of priorities
	std::vector<int> merge = merge_priority_list(service_priorities, object_priorities);

	// Call services and objects in order of priority
	for (int i = merge.size() - 1; i >= 0; i--)
	{
		int priority = merge[i];
		for (IService* service : service_tick_order.GetEntriesOfPriority(priority))
		{
			service->Tick(dt);
		}
		for (IGameObject* game_object : object_tick_order.GetEntriesOfPriority(priority))
		{
			game_object->Tick(dt);
		}
	}
}

// Call Service Behavior
void CallService::SetServiceStartupPriority(IService* service, int priority)
{
	// Add the provided service with to the service_startup_order CallOrder with the relevant priority
	service_startup_order.Add(service, priority);

	if (game_time > 0)
	{
		service->Start();
	}
}
void CallService::SetServiceTickPriority(IService* service, int priority)
{
	// Add the provided service with to the service_tick_order CallOrder with the relevant priority
	service_tick_order.Add(service, priority);
}
void CallService::SetObjectStartupPriority(IGameObject* game_object, int priority)
{
	// Add the provided service with to the object_startup_order CallOrder with the relevant priority
	object_startup_order.Add(game_object, priority);

	if (game_time > 0)
	{
		game_object->Start();
	}
}
void CallService::SetObjectTickPriority(IGameObject* game_object, int priority)
{
	// Add the provided service with to the object_tick_order CallOrder with the relevant priority
	object_tick_order.Add(game_object, priority);
}
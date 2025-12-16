// Christopher Stackpole, 12/06/2025

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "Services/ServiceManager.h"
#include "Services.h"
#include "GameServices.h"

#include "GameObjects/ARC.h"
#include "GameObjects/Asteroid.h"
#include "GameObjects/Laser.h"
#include "GameObjects/SCOOP.h"
#include "GameObjects/SWORD.h"
#include "GameObjects/World.h"

constexpr int WINDOW_WIDTH = 1440;
constexpr int WINDOW_HEIGHT = 1080;
constexpr int HALF_WIDTH = WINDOW_WIDTH / 2;
constexpr int HALF_HEIGHT = WINDOW_HEIGHT / 2;

void RegisterEngineServices()
{
	// Register Game Services
	Services().RegisterService<CallService>();
	Services().RegisterService<RenderService>();
	Services().RegisterService<PhysicsService>();
	Services().RegisterService<InputService>();
	Services().RegisterService<ResourceService>();
}
void SetEngineRunPriorities()
{
	// Obtain pointers to registered services
	CallService* call_service = Services().Get<CallService>();
	InputService* input_service = Services().Get<InputService>();
	PhysicsService* physics_service = Services().Get<PhysicsService>();
	RenderService* render_service = Services().Get<RenderService>();
	ResourceService* resource_service = Services().Get<ResourceService>();

	// Set startup priorities
	call_service->SetServiceStartupPriority(input_service, CLST_BASIC_SERVICE);
	call_service->SetServiceStartupPriority(physics_service, CLST_BASIC_SERVICE);
	call_service->SetServiceStartupPriority(render_service, CLST_PRELOAD_CACHE);
	call_service->SetServiceStartupPriority(resource_service, CLST_PRELOAD_CACHE);

	// Set tick priorities
	call_service->SetServiceTickPriority(input_service, CLT_PREFRAME);
	call_service->SetServiceTickPriority(physics_service, CLT_PREFRAME);
	call_service->SetServiceTickPriority(render_service, CLT_FRAMERENDER);
	call_service->SetServiceTickPriority(resource_service, CLT_BASIC);
}
void ConfigureGameWindow()
{
	RenderService* render_service = Services().Get<RenderService>();

	// Configure the game window
	render_service->SetWindowSize(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT));
	render_service->SetWindowTitle("Space Game");
	render_service->SetWindowBackground(sf::Color(10, 0, 10));
}
void RegisterGameServices()
{
	Services().RegisterService<AsteroidService>();
	Services().RegisterService<PoolService>();
	Services().RegisterService<SwordService>();
}
void SetGameRunPriorities()
{
	// Obtain pointers to registered services
	CallService* call_service = Services().Get<CallService>();

	AsteroidService* asteroid_service = Services().Get<AsteroidService>();
	PoolService* pool_service = Services().Get<PoolService>();
	SwordService* sword_service = Services().Get<SwordService>();

	// Set Startup Priorities
	call_service->SetServiceStartupPriority(asteroid_service, CLST_BASIC_SERVICE);
	call_service->SetServiceStartupPriority(pool_service, CLST_BASIC_SERVICE);
	call_service->SetServiceStartupPriority(sword_service, CLST_BASIC_SERVICE);
	
	// Set Tick Priorities
	call_service->SetServiceTickPriority(asteroid_service, CLT_BASIC);
	call_service->SetServiceTickPriority(sword_service, CLT_PREFRAME);
}

int main()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	// Setup Game
	RegisterEngineServices();
	SetEngineRunPriorities();
	ConfigureGameWindow();

	RegisterGameServices();
	SetGameRunPriorities();

	// Obtain pointers to services
	CallService* call_service = Services().Get<CallService>();
	InputService* input_service = Services().Get<InputService>();
	RenderService* render_service = Services().Get<RenderService>();
	ResourceService* resource_service = Services().Get<ResourceService>();

	// Build Game Map
	World world;
	world.SetPosition(sf::Vector2f(HALF_WIDTH, HALF_HEIGHT));

	ARC arc;
	arc.SetPosition(sf::Vector2f(HALF_WIDTH, HALF_HEIGHT - 15));

	// Tell Service Manager to run start on all services, this kicks off proper game execution
	Services().Start();

	long long prev_time = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()
	).count();
	while (render_service->WindowOpen())
	{
		// Poll and handle all events
		while (const std::optional event = render_service->PollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				// TODO: Move this to Services().Shutdown() -> requires writing ServiceManager.Shutdown
				// TODO: This also means going throughout and implementing proper resource deallocation
				render_service->CloseWindow();
			}

			// Input Service does not manage all types of events, so we only want to pass on the managed event types
			if (
				event.has_value() && (
				event->is<sf::Event::KeyPressed>() ||
				event->is<sf::Event::KeyReleased>() ||
				event->is<sf::Event::MouseButtonPressed>() ||
				event->is<sf::Event::MouseButtonReleased>()
				))
			{
				input_service->ProcessEvent(event.value());
			}
		}

		// Send off tick to services with registered time in ms between frames
		long long curr_time = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()
		).count();

		Services().Tick(curr_time - prev_time);

		prev_time = curr_time;
	}

	return 0;
}
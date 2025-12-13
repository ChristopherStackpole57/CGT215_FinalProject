// Christopher Stackpole, 12/06/2025

#include <iostream>

#include "RenderService.h"

void RenderService::Start()
{
	window = sf::RenderWindow(sf::VideoMode(window_size), window_title);
}
void RenderService::Shutdown()
{

}
void RenderService::Tick(float dt)
{
	// Enter the primary rendering loop
	// Start by setting the background color for the window
	window.clear(window_background_color);

	// Iterate through render_objects by layer and draw all active objects
	for (auto& [_, objects] : render_objects)
	{
		for (RenderObject* object : objects)
		{
			// Ensure the object is enabled and non null
			if (object->active && object->drawable)
			{
				// Render the object onto the game window
				window.draw(*object->drawable);
			}
		}
	}

	// Render the results of all operations thus far
	window.display();
}

// Render Service Behavior
void RenderService::SetWindowSize(sf::Vector2u new_size)
{
	window_size = new_size;
}
sf::Vector2u RenderService::GetWindowSize()
{
	return window_size;
}
void RenderService::SetWindowTitle(sf::String new_title)
{
	window_title = new_title;
}
void RenderService::SetWindowBackground(sf::Color new_color)
{
	window_background_color = new_color;
}

bool RenderService::WindowOpen()
{
	return window.isOpen();
}
std::optional<sf::Event> RenderService::PollEvent()
{
	return window.pollEvent();
}
void RenderService::CloseWindow()
{
	window.close();
}

void RenderService::RegisterRenderObject(RenderObject& render_object)
{
	render_objects[render_object.layer].push_back(&render_object);
}
void RenderService::UnregisterRenderObject(RenderObject& render_object)
{
	/*for (auto& [layer, objects] : render_objects)
	{
		objects.erase(
			std::remove(
				objects.begin(),
				objects.end(),
				render_object
			),
			objects.end()
		);
	}*/
}
// Christopher Stackpole, 12/06/2025

#ifndef RENDER_SERVICE_H
#define RENDER_SERVICE_H

#include <algorithm>
#include <map>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "Services/ServiceInterface.h"

#include "Services/EngineServices/RenderService/RenderObject.h"

class RenderService : public IService
{
public:
	void Start() override;
	void Shutdown() override;
	void Tick(float dt) override;

	void SetWindowSize(sf::Vector2u new_size);
	sf::Vector2u GetWindowSize();
	void SetWindowTitle(sf::String title);
	void SetWindowBackground(sf::Color new_color);

	bool WindowOpen();
	std::optional<sf::Event> PollEvent();
	void CloseWindow();

	void RegisterRenderObject(RenderObject& render_object);
	void UnregisterRenderObject(RenderObject& render_object);
	//void ChangeRenderObjectLayer(RenderObject& render_object, int layer);
private:
	sf::Vector2u window_size = sf::Vector2u(800, 600);
	sf::String window_title = "Window";
	sf::Color window_background_color = sf::Color::Black;

	sf::RenderWindow window;

	std::map<int, std::vector<RenderObject*>> render_objects;
};

#endif
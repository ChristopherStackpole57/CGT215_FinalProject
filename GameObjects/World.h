// Christopher Stackpole, 12/08/2025

#ifndef WORLD_H
#define WORLD_H

#include "Services.h"

#include "GameObjects/GameObjectInterface.h"

class World : public IGameObject
{
public:
	World();

	void Start() override;
	void Shutdown() override;
	void Tick(float dt);

	sf::Vector2f GetPosition() override;
	void SetPosition(sf::Vector2f position) override;

	sf::Vector2f GetSize() override;

	void Hit(float damage);
private:
	std::unique_ptr<sf::Sprite> sprite;
	std::string path = "Resources/planet.bmp";

	RenderObject game_over_object;
	std::unique_ptr<sf::Text> game_over_text;
	std::string font_path = "Resources/ARIALBI.TTF";

	float health = 100.f;
};

#endif
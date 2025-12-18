// Christopher Stackpole, 12/08/2025

#ifndef WORLD_H
#define WORLD_H

#include "Services.h"

#include "GameObjects/GameObjectInterface.h"

constexpr float WORLD_BASE_HEALTH = 100.f;

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

	RenderObject health_background;
	RenderObject health_frame;
	RenderObject health_slider;

	std::unique_ptr<sf::Sprite> background_sprite;
	std::unique_ptr<sf::Sprite> frame_sprite;
	std::unique_ptr<sf::Sprite> slider_sprite;

	std::string background_path = "Resources/healthbar_background.bmp";
	std::string frame_path = "Resources/healthbar_frame.bmp";
	std::string slider_path = "Resources/healthbar_slider.bmp";

	float slider_base_posx;

	float health = WORLD_BASE_HEALTH;
};

#endif
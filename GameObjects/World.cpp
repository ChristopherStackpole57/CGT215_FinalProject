// Christopher Stackpole, 12/08/2025

#include "World.h"

World::World()
{
	// Register Self with the Call Service
	CallService* call_service = Services().Get<CallService>();
	call_service->SetObjectStartupPriority(this, CLST_BASIC_GAMEOBJECT);
	//call_service->SetObjectTickPriority(this, CLT_BASIC);

	// Create Physics Body
	this->body = Services().Get<PhysicsService>()->RegisterPhysicsObject(this);
}

void World::Start()
{
	// Grab the correct graphic from the Resource Service
	ResourceService* resource_service = Services().Get<ResourceService>();
	sf::Texture& texture = resource_service->Load<sf::Texture>(path);
	sf::Vector2u texture_size = texture.getSize();

	// Update render object and register with Render Service
	sprite = std::make_unique<sf::Sprite>(texture);
	sprite->setOrigin(sf::Vector2f(texture_size.x / 2.0f, texture_size.y / 2.0f));
	sprite->move(position);
	
	render_object.drawable = sprite.get();
	
	RenderService* render_service = Services().Get<RenderService>();
	render_service->RegisterRenderObject(render_object);

	// Update physics body collider
	if (body)
	{
		body->SetCollider(Circle{ (float)texture_size.x / 2.f });
	}

	// Create game over screen
	sf::Font& font = resource_service->Load<sf::Font>(font_path);
	game_over_text = std::make_unique<sf::Text>(font, "Game Over", 145);
	game_over_text->setFillColor({ 200, 100, 100, 255});

	sf::Vector2f size = game_over_text->getLocalBounds().size;
	game_over_text->setOrigin(size / 2.f);

	sf::Vector2u screen_size = render_service->GetWindowSize();
	game_over_text->move(sf::Vector2f{ screen_size.x / 2.f, screen_size.y / 2.f - 170.f});

	game_over_object.drawable = game_over_text.get();
	game_over_object.layer = 1;
	game_over_object.active = false;

	render_service->RegisterRenderObject(game_over_object);

	// Create Health Bar
	sf::Texture& background_texture = resource_service->Load<sf::Texture>(background_path);
	sf::Texture& frame_texture = resource_service->Load<sf::Texture>(frame_path);
	sf::Texture& slider_texture = resource_service->Load<sf::Texture>(slider_path);

	background_sprite = std::make_unique<sf::Sprite>(background_texture);
	frame_sprite = std::make_unique<sf::Sprite>(frame_texture);
	slider_sprite = std::make_unique<sf::Sprite>(slider_texture);

	background_sprite->setOrigin({ background_texture.getSize().x / 2.f, background_texture.getSize().y / 2.f});
	frame_sprite->setOrigin({ frame_texture.getSize().x / 2.f, frame_texture.getSize().y / 2.f });
	slider_sprite->setOrigin({ (float)slider_texture.getSize().x , slider_texture.getSize().y / 2.f});

	float cx = screen_size.x - 196.f;
	float cy = screen_size.y - 64.f;
	background_sprite->move({ cx, cy });
	frame_sprite->move({ cx, cy });
	slider_sprite->move({ cx + 0.5f * slider_texture.getSize().x, cy});
	slider_base_posx = slider_sprite->getPosition().x;

	health_background.drawable = background_sprite.get();
	health_frame.drawable = frame_sprite.get();
	health_slider.drawable = slider_sprite.get();

	health_background.layer = 1;
	health_slider.layer = 2;
	health_frame.layer = 3;

	render_service->RegisterRenderObject(health_background);
	render_service->RegisterRenderObject(health_frame);
	render_service->RegisterRenderObject(health_slider);
}
void World::Shutdown()
{
	// TODO: Unregister render object
}
void World::Tick(float dt)
{

}

// World Behavior
void World::SetPosition(sf::Vector2f position)
{
	// Set the internally stored position and move the sprite accordingly
	this->position = position;

	// Sprite may not exist yet (for example if SetPosition is called before CallService::Start)
	if (sprite)
	{
		sprite->setPosition(position);
	}
	if (body)
	{
		body->SetPosition(position);
	}
}
sf::Vector2f World::GetPosition()
{
	return position;
}
sf::Vector2f World::GetSize()
{
	if (sprite)
	{
		sf::Vector2i size = sprite->getTextureRect().size;
		return sf::Vector2f(
			size.x,
			size.y
		);
	}
	return sf::Vector2f();
}

void World::Hit(float damage)
{
	health -= damage;

	float scale = std::max(0.f, health / WORLD_BASE_HEALTH);
	slider_sprite->setScale({ scale, 1.f });
	slider_sprite->setPosition({
		slider_base_posx - 6.f * (1 - scale),
		slider_sprite->getPosition().y
	});

	if (health <= 0)
	{
		game_over_object.active = true;

		CallService* call_service = Services().Get<CallService>();
		call_service->Pause();
	}
}
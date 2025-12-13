// Christopher Stackpole, 12/11/2025

#include "Asteroid.h"

Asteroid::Asteroid()
{
	// Register self with call service
	CallService* call_service = Services().Get<CallService>();
	call_service->SetObjectStartupPriority(this, CLST_BASIC_GAMEOBJECT);
	call_service->SetObjectTickPriority(this, CLT_BASIC);
}

// GameObject Behavior
void Asteroid::Start()
{
	// Get asteroid image from resource service
	ResourceService* resource_service = Services().Get<ResourceService>();
	sf::Texture& texture = resource_service->Load<sf::Texture>(path);
	sf::Vector2u texture_size = texture.getSize();

	// Create sprite and update origin
	sprite = std::make_unique<sf::Sprite>(texture);
	sprite->setOrigin(
		sf::Vector2f(
			texture_size.x / 2.0f,
			texture_size.y / 2.0f
		)
	);
	sprite->move(position);

	render_object.drawable = sprite.get();

	// Register sprite with render service
	RenderService* render_service = Services().Get<RenderService>();
	render_service->RegisterRenderObject(render_object);
}
void Asteroid::Shutdown()
{

}
void Asteroid::Tick(float dt)
{

}

// Asteroid Behavior
void Asteroid::SetPosition(sf::Vector2f position)
{
	this->position = position;

	if (sprite)
	{
		sprite->setPosition(position);
	}
}
sf::Vector2f Asteroid::GetPosition()
{
	return position;
}
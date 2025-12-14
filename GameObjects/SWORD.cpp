// Christopher Stackpole, 12/13/2025

#include "SWORD.h"

SWORD::SWORD()
{
	// Register self with call service
	CallService* call_service = Services().Get<CallService>();
	call_service->SetObjectStartupPriority(this, CLST_BASIC_GAMEOBJECT);
	call_service->SetObjectTickPriority(this, CLT_BASIC);
}

// GameObject Behavior
void SWORD::Start()
{
	// Get the cannon sprite from resource service
	ResourceService* resource_service = Services().Get<ResourceService>();
	sf::Texture& texture = resource_service->Load<sf::Texture>(path);
	sf::Vector2u texture_size = texture.getSize();

	// Create sprite and register with render service
	sprite = std::make_unique<sf::Sprite>(texture);
	sprite->setOrigin(
		sf::Vector2f(
			texture_size.x / 2.0f,
			texture_size.y / 2.0f									// Off set to acount for barrel length
		)
	);
	sprite->move(position);

	render_object.drawable = sprite.get();

	RenderService* render_service = Services().Get<RenderService>();
	render_service->RegisterRenderObject(render_object);
}
void SWORD::Shutdown()
{

}
void SWORD::Tick(float dt)
{

}

// SCOOP Behavior
void SWORD::SetPosition(sf::Vector2f position)
{
	this->position = position;

	if (sprite)
	{
		sprite->setPosition(position);
	}
}
sf::Vector2f SWORD::GetPosition()
{
	return position;
}
sf::Vector2f SWORD::GetSize()
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
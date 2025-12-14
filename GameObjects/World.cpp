// Christopher Stackpole, 12/08/2025

#include "World.h"

World::World()
{
	// Register Self with the Call Service
	CallService* call_service = Services().Get<CallService>();
	call_service->SetObjectStartupPriority(this, CLST_BASIC_GAMEOBJECT);
	//call_service->SetObjectTickPriority(this, CLT_BASIC);
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

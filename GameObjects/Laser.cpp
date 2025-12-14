// Christopher Stackpole, 12/11/2025

#include "Laser.h"

Laser::Laser()
{
	CallService* call_service = Services().Get<CallService>();
	call_service->SetObjectStartupPriority(this, CLST_BASIC_GAMEOBJECT);
	call_service->SetObjectTickPriority(this, CLT_BASIC);

	// Create physics body
	this->body = Services().Get<PhysicsService>()->RegisterPhysicsObject(this);
}

// GameObject Behavior
void Laser::Start()
{
	// Get asteroid image from resource service
	ResourceService* resource_service = Services().Get<ResourceService>();
	sf::Texture& texture = resource_service->Load<sf::Texture>(path);
	sf::Vector2u texture_size = texture.getSize();

	// Create sprite and update origin
	sprite = std::make_unique<sf::Sprite>(texture);
	sprite->setScale(sf::Vector2f(0.5,0.5));
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

	// Update physics body collider
	if (body)
	{
		body->SetCollider(Rect{ {10.f, 10.f} });
	}
}
void Laser::Shutdown()
{
	// free self with pool service
	PoolService* pool_service = Services().Get<PoolService>();
	pool_service->Release(this);
}
void Laser::Tick(float dt)
{
	position = body->GetPosition();
	sprite->setPosition(body->GetPosition());

	// Check if outside map bounds, and if so free object
	RenderService* render_service = Services().Get<RenderService>();
	sf::Vector2u size = render_service->GetWindowSize();
	if (position.x < 0 || position.x > size.x || position.y < 0 || position.y > size.y)
	{
		PoolService* pool_service = Services().Get<PoolService>();
		pool_service->Release(this);
	}
}

// Laser Behavior
void Laser::SetPosition(sf::Vector2f position)
{
	this->position = position;

	if (sprite)
	{
		sprite->setPosition(position);
	}
	if (body)
	{
		body->SetPosition(position);
	}
}
sf::Vector2f Laser::GetPosition()
{
	return position;
}
sf::Vector2f Laser::GetSize()
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
void Laser::SetRotation(sf::Angle angle)
{
	if (sprite)
	{
		sprite->setRotation(angle);
	}
}
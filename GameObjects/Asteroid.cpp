// Christopher Stackpole, 12/11/2025

#include "Asteroid.h"

Asteroid::Asteroid()
{
	// Register self with call service
	CallService* call_service = Services().Get<CallService>();
	call_service->SetObjectStartupPriority(this, CLST_BASIC_GAMEOBJECT);
	call_service->SetObjectTickPriority(this, CLT_BASIC);

	// Create physics body
	this->body = Services().Get<PhysicsService>()->RegisterPhysicsObject(this);
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

	// Update physics body collider
	if (body)
	{
		body->SetCollider(Circle{ (float)texture_size.x / 2.f });
		
		// Bind overlap events
		body->SetOnOverlap(
			[this](Body& self, Body& other)
			{
				IGameObject* other_game_obj = other.GetOwner();
				if (!other_game_obj) return;

				PoolService* pool_service = Services().Get<PoolService>();
				if (Laser* laser = dynamic_cast<Laser*>(other_game_obj))
				{
					laser->Shutdown();

					this->health -= 50.f;
					if (this->health <= 0)
					{
						this->health = 100.f;
						pool_service->Release(this);
					}
				}
				else if (World* world = dynamic_cast<World*>(other_game_obj))
				{
					world->Hit();
					pool_service->Release(this);
				}
			}
		);
	}
}
void Asteroid::Shutdown()
{

}
void Asteroid::Tick(float dt)
{
	position = body->GetPosition();
	sprite->setPosition(body->GetPosition());

	// Rotate a little bit
	sprite->rotate(sf::Angle(sf::degrees(0.01)));
}

// Asteroid Behavior
void Asteroid::SetPosition(sf::Vector2f position)
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
sf::Vector2f Asteroid::GetPosition()
{
	return position;
}
sf::Vector2f Asteroid::GetSize()
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
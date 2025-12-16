// Christopher Stackpole, 12/11/2025

#include "Asteroid.h"

#include "GameServices.h"

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
	// Generate a random amount of health
	ResetHealth();

	// Get asteroid image from resource service
	ResourceService* resource_service = Services().Get<ResourceService>();
	sf::Texture& texture = resource_service->Load<sf::Texture>(path);
	sf::Vector2u texture_size = texture.getSize();
	tex_rad = (float)texture_size.x / 2.f;

	// Create sprite and update origin
	sprite = std::make_unique<sf::Sprite>(texture);
	ResetSize();
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

					this->health -= laser->GetDamage();
					if (this->health <= 0)
					{
						SwordService* sword_service = Services().Get<SwordService>();
						sword_service->AddOreAmount(ASTEROID_BASE_ORE);

						//this->health = 100.f;
						pool_service->Release(this);
					}
					else
					{
						this->ResetSize();
					}
				}
				else if (World* world = dynamic_cast<World*>(other_game_obj))
				{
					SwordService* sword_service = Services().Get<SwordService>();
					sword_service->AddOreAmount(ASTEROID_BASE_ORE);

					world->Hit(GetScaleAdjust() * ASTEROID_MAX_DAMAGE);
					pool_service->Release(this);
				}
				else if (SWORD* sword = dynamic_cast<SWORD*>(other_game_obj))
				{
					sword->Hit(GetScaleAdjust() * ASTEROID_MAX_DAMAGE);
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
	sprite->rotate(sf::Angle(sf::degrees(0.02 * dt)));
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

float Asteroid::GetScaleAdjust()
{
	return health / ASTEROID_MAX_HEALTH;
}
void Asteroid::ResetSize()
{
	float scale = GetScaleAdjust();
	if (sprite)
	{
		sprite->setScale({scale, scale});
	}
	if (body)
	{
		body->SetCollider(Circle{ tex_rad * scale });
	}
}
void Asteroid::ResetHealth()
{
	health = std::rand() % (int)(ASTEROID_MAX_HEALTH - ASTEROID_MIN_GEN_HEALTH + 1) + ASTEROID_MIN_GEN_HEALTH;
}
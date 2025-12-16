// Christopher Stackpole, 12/13/2025

#include "SWORD.h"

SWORD::SWORD()
{
	// Register self with call service
	CallService* call_service = Services().Get<CallService>();
	call_service->SetObjectStartupPriority(this, CLST_BASIC_GAMEOBJECT);
	call_service->SetObjectTickPriority(this, CLT_BASIC);

	// create physics body
	this->body = Services().Get<PhysicsService>()->RegisterPhysicsObject(this);
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
	sprite->setScale({ SWORD_SPRITE_SCALE, SWORD_SPRITE_SCALE });
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

	if (body)
	{
		body->SetCollider(Rect{ sf::Vector2f{
			(float)texture_size.x * SWORD_SPRITE_SCALE / 2.f,
			(float)texture_size.y * SWORD_SPRITE_SCALE / 2.f,
		} });
	}
}
void SWORD::Shutdown()
{

}
void SWORD::Tick(float dt)
{
	if (!active)
	{
		return;
	}

	// Determine whether or not the SWORD can fire this frame
	CallService* call_service = Services().Get<CallService>();
	if (debounce)
	{
		int curr_time = call_service->GetGameTime();
		if ((curr_time - shot_time) >= (1000.f / (fire_rate * Services().Get<SwordService>()->GetFirerateModifier())))
		{
			debounce = false;
		}
		else
		{
			return;
		}
	}

	// SWORDs should look for and shoot at asteroids
	AsteroidService* asteroid_service = Services().Get<AsteroidService>();
	std::vector<Asteroid*> asteroids = asteroid_service->GetAsteroids();

	RenderService* render_service = Services().Get<RenderService>();
	sf::Vector2f size = sf::Vector2f{ (float)render_service->GetWindowSize().x, (float)render_service->GetWindowSize().y };

	PoolService* pool_service = Services().Get<PoolService>();

	// TODO: Aiming system will identify two asteroids, one to either side, and will shoot at both. These asteroids must sit
	// within a certain angle range (~90-135 deg) so that the SWORD can't just shoot straight overheard or straight below
	// should make a neat visual with the SWORDs basically working to protect each other instead of themselves or the planet
	// This will also require me to add a second debounce and I'll probably want to modularize the shooting code

	Asteroid* target_asteroid = nullptr;
	float min_planet_dist = FLT_MAX;
	float distsq = FLT_MAX;
	for (Asteroid* asteroid : asteroids)
	{
		if ((asteroid->GetPosition() - position).lengthSquared() < rangesq)
		{
			float asteroid_planet_dist = (asteroid->GetPosition() - size / 2.f).lengthSquared();
			float asteroid_distsq = (asteroid->GetPosition() - position).lengthSquared();
			if (asteroid_planet_dist < min_planet_dist && asteroid_distsq < rangesq)
			{
				target_asteroid = asteroid;
				min_planet_dist = asteroid_planet_dist;
				distsq = asteroid_distsq;
			}
		}
	}
	if (target_asteroid == nullptr)
	{
		return;
	}

	debounce = true;
	shot_time = call_service->GetGameTime();

	sf::Vector2f apos = target_asteroid->GetPosition();
	sf::Vector2f avel = target_asteroid->GetVelocity();

	sf::Vector2f diff = apos - position;
	float speedsq = laser_speed * laser_speed;
	float timesq = distsq / speedsq;
	sf::Vector2f target = apos + avel * std::sqrt(timesq);
	sf::Vector2f aim = target - position;
		
	Laser* laser = pool_service->Get<Laser>();
	laser->SetPosition(position);
	laser->SetRotation(sf::Angle(sf::radians(
		std::atan2(aim.y, aim.x) + 3.141592 / 2
	)));
	laser->SetVelocity(aim.normalized() * LASER_SPEED);
	laser->SetDamage(LASER_DAMAGE_SWORD * Services().Get<SwordService>()->GetDamageModifier());
}

// SWORD Behavior
void SWORD::SetPosition(sf::Vector2f position)
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
sf::Vector2f SWORD::GetPosition()
{
	return position;
}
void SWORD::SetRotation(sf::Angle angle)
{
	if (sprite)
	{
		sprite->setRotation(angle);
	}
}
sf::Angle SWORD::GetRotation()
{
	if (sprite)
	{
		return sprite->getRotation();
	}
	return sf::Angle();
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

void SWORD::Hit(float damage)
{
	health -= damage;
	if (health <= 0)
	{
		Services().Get<PoolService>()->Release(this);
		Services().Get<SwordService>()->UpdateSwordPositions();
	}
}
void SWORD::ResetHealth()
{
	health = SWORD_BASE_HEALTH;
}
//Christopher Stackpole, 12/09/2025

#include "ARC.h"

ARC::ARC()
{
	// Register self with call service
	CallService* call_service = Services().Get<CallService>();
	call_service->SetObjectStartupPriority(this, CLST_BASIC_GAMEOBJECT);
	call_service->SetObjectTickPriority(this, CLT_BASIC);
}

// GameObject Behavior
void ARC::Start()
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
			(texture_size.y + 15) / 2.0f									// Off set to acount for barrel length
		)
	);
	sprite->move(position);

	render_object.drawable = sprite.get();

	RenderService* render_service = Services().Get<RenderService>();
	render_service->RegisterRenderObject(render_object);

	CallService* call_service = Services().Get<CallService>();

	// register input actions with input service
	// lambda functions need to capture ARC so they can manipulate the state down states
	auto left_turn_press = [this]()
		{
			this->left_arrow_down = true;
		};
	auto left_turn_release = [this]()
		{
			this->left_arrow_down = false;
		};
	auto right_turn_press = [this]()
		{
			this->right_arrow_down = true;
		};
	auto right_turn_release = [this]()
		{
			this->right_arrow_down = false;
		};
	auto shoot_press = [this, call_service]()
		{
			if (!(this->debounce))
			{
				this->debounce = true;
				shot_time = call_service->GetGameTime();
				
				// Shoot Logic
				PoolService* pool_service = Services().Get<PoolService>();
				Laser* laser = pool_service->Get<Laser>();
				laser->SetPosition(
					this->position + 
					sf::Vector2f(0, -0.5 * this->sprite->getTextureRect().size.y).rotatedBy(
						this->sprite->getRotation()
					)
				);
				laser->SetRotation(this->sprite->getRotation());
				laser->SetVelocity(sf::Vector2f(0, -500).rotatedBy(
					this->sprite->getRotation()
				));
			}
			else
			{
				// TODO: Play "Can't Shoot" sound
				std::cout << "on cooldown" << std::endl;
			}
		};

	InputService* input_service = Services().Get<InputService>();
	input_service->RegisterInputListener(sf::Keyboard::Scancode::A, left_turn_press);
	input_service->RegisterInputListener(sf::Keyboard::Scancode::A, left_turn_release, false);
	input_service->RegisterInputListener(sf::Keyboard::Scancode::D, right_turn_press);
	input_service->RegisterInputListener(sf::Keyboard::Scancode::D, right_turn_release, false);
	input_service->RegisterInputListener(sf::Keyboard::Scancode::W, shoot_press);
}
void ARC::Shutdown()
{

}
void ARC::Tick(float dt)
{
	sf::Angle delta_angle(sf::degrees(100) * dt / 1000);

	// Check if turn keys are pressed, if so turn the sprite
	if (left_arrow_down && right_arrow_down)
	{
		// both arrows down, cancels movement
		return;
	}
	else if (left_arrow_down)
	{
		sprite->rotate(-1 * delta_angle);
	}
	else if (right_arrow_down)
	{
		sprite->rotate(delta_angle);
	}

	// Check if it has been sufficient enough time to disable debounce
	if (debounce)
	{
		CallService* call_service = Services().Get<CallService>();
		int curr_time = call_service->GetGameTime();
		if ((curr_time - shot_time) >= (1000 / fire_rate))
		{
			debounce = false;
		}
	}
}

// ARC Behavior
void ARC::SetRotation(sf::Angle rotation)
{
	sprite->setRotation(rotation);
}
sf::Angle ARC::GetRotation()
{
	return sprite->getRotation();
}
sf::Vector2f ARC::GetSize()
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
void ARC::Rotate(sf::Angle rotation)
{
	sprite->rotate(rotation);
}
void ARC::SetPosition(sf::Vector2f position)
{
	this->position = position;

	if (sprite)
	{
		sprite->setPosition(position);
	}
}
sf::Vector2f ARC::GetPosition()
{
	return position;
}
// Christopher Stackpole, 12/15/2025

#include "SwordService.h"

#include "GameObjects/SWORD.h"

void SwordService::Start()
{
	// Create render objects
	float y = Services().Get<RenderService>()->GetWindowSize().y - 64.f;
	icon_sword = MakeSprite(icon_sword_path, sf::Vector2f{ 64.f, y });
	icon_damage = MakeSprite(icon_damage_path, sf::Vector2f{ 164.f, y });
	icon_firerate = MakeSprite(icon_firerate_path, sf::Vector2f{ 256.f, y });

	// Bind Callback to create swords
	InputService* input_service = Services().Get<InputService>();
	input_service->RegisterInputListener(SPAWN_SWORD,
		[this]()
		{
			// ensure player has enough ore
			// Identify Active Swords
			int count = 0;
			for (SWORD* sword : swords)
			{
				if (sword->GetActive())
				{
					count++;
				}
			}
			float cost = SWORD_COST + (SWORD_COST * SWORD_COUNT_COST_GROWTH * count);
			if (ore < cost)
			{
				return;
			}

			// Create SWORD and hold it in list
			ore -= SWORD_COST;
			PoolService* pool_service = Services().Get<PoolService>();
			SWORD* sword = pool_service->Get<SWORD>();
			sword->ResetHealth();

			swords.insert(sword);

			this->UpdateSwordPositions();
		}
	);

	// Bind upgrade callbacks
	input_service->RegisterInputListener(UPRGRADE_SWORD_DAMAGE,
		[this]()
		{
			// Calculate cost of upgrade
			int num_upgrades = (this->damage_modifier - SWORD_BASE_MODIFIER_DAMAGE) / SWORD_UPGRADE_INCREMENT_DAMAGE;
			float upgrade_cost = SWORD_UPGRADE_BASE_DAMAGE_COST + SWORD_UPGRADE_COST_GROWTH * num_upgrades;

			if (ore >= upgrade_cost)
			{
				ore -= upgrade_cost;
				this->damage_modifier += SWORD_UPGRADE_INCREMENT_DAMAGE;
				return;
			}
		}
	);
	input_service->RegisterInputListener(UPGRADE_SWORD_FIRERATE,
		[this]()
		{
			// Calculate cost of upgrade
			int num_upgrades = (this->firerate_modifier - SWORD_BASE_MODIFIER_FIRERATE) / SWORD_UPGRADE_INCREMENT_FIRERATE;
			float upgrade_cost = SWORD_UPGRADE_BASE_FIRERATE_COST + SWORD_UPGRADE_COST_GROWTH * num_upgrades;

			if (ore >= upgrade_cost)
			{
				ore -= upgrade_cost;
				this->firerate_modifier += SWORD_UPGRADE_INCREMENT_FIRERATE;
				return;
			}
		}
	);
}
void SwordService::Shutdown()
{

}
void SwordService::Tick(float dt)
{
	sf::Color cant_afford{ 100, 100, 100, 255 };
	sf::Color can_afford{ 255, 255, 255, 255 };

	// Check if can afford buying sword
	// Identify Active Swords
	int count = 0;
	for (SWORD* sword : swords)
	{
		if (sword->GetActive())
		{
			count++;
		}
	}
	float upgrade_cost = SWORD_COST + (SWORD_COST * SWORD_COUNT_COST_GROWTH * count);
	sf::Color color = (ore < upgrade_cost) ? cant_afford : can_afford;
	icon_sword->setColor(color);

	// Check if can afford buying damage upgrade
	int num_upgrades = (this->damage_modifier - SWORD_BASE_MODIFIER_DAMAGE) / SWORD_UPGRADE_INCREMENT_DAMAGE;
	upgrade_cost = SWORD_UPGRADE_BASE_DAMAGE_COST + SWORD_UPGRADE_COST_GROWTH * num_upgrades;
	color = (ore < upgrade_cost) ? cant_afford : can_afford;
	icon_damage->setColor(color);

	// Check if can afford buying firerate upgrade
	num_upgrades = (this->firerate_modifier - SWORD_BASE_MODIFIER_FIRERATE) / SWORD_UPGRADE_INCREMENT_FIRERATE;
	upgrade_cost = SWORD_UPGRADE_BASE_FIRERATE_COST + SWORD_UPGRADE_COST_GROWTH * num_upgrades;
	color = (ore < upgrade_cost) ? cant_afford : can_afford;
	icon_firerate->setColor(color);
}

void SwordService::AddOreAmount(float amount)
{
	ore += amount;
}
void SwordService::UpdateSwordPositions()
{
	RenderService* render_service = Services().Get<RenderService>();
	sf::Vector2u size = render_service->GetWindowSize();
	sf::Vector2f half{ size.x / 2.f, size.y / 2.f };

	// Identify Active Swords
	std::vector<SWORD*> active_swords;
	for (SWORD* sword : swords)
	{
		if (sword->GetActive())
		{
			active_swords.push_back(sword);
		}
	}

	float delta_angle = 360.f / active_swords.size();
	sf::Vector2f offset{ 0.f, -300.f };

	int index = 0;
	for (SWORD* sword : active_swords)
	{
		sf::Angle rot = sf::Angle(sf::degrees(delta_angle * (float)index));
		sf::Vector2f rotated = offset.rotatedBy(rot);

		sword->SetPosition(half + rotated);
		sword->SetRotation(rot);
		
		index++;
	}
}
float SwordService::GetDamageModifier()
{
	return damage_modifier;
}
float SwordService::GetFirerateModifier()
{
	return firerate_modifier;
}

std::unique_ptr<sf::Sprite> SwordService::MakeSprite(std::string path, sf::Vector2f position)
{
	RenderService* render_service = Services().Get<RenderService>();
	ResourceService* resource_service = Services().Get<ResourceService>();

	sf::Texture& texture = resource_service->Load<sf::Texture>(path);
	sf::Vector2u size = texture.getSize();

	std::unique_ptr<sf::Sprite> sprite = std::make_unique<sf::Sprite>(texture);
	sprite->setOrigin(sf::Vector2f{ size.x / 2.f, size.y / 2.f });
	sprite->move(position);

	RenderObject icon_render_object;
	icon_render_object.drawable = sprite.get();
	icon_render_object.layer = 1;

	// getting stable reference to avoid UB
	render_objects.emplace_back(icon_render_object);
	RenderObject& obj_ref = render_objects.back();
	Services().Get<RenderService>()->RegisterRenderObject(obj_ref);

	return sprite;
}
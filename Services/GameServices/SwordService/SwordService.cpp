// Christopher Stackpole, 12/15/2025

#include "SwordService.h"

#include "GameObjects/SWORD.h"

void SwordService::Start()
{
	// Bind Callback to create swords
	InputService* input_service = Services().Get<InputService>();
	input_service->RegisterInputListener(SPAWN_SWORD,
		[this]()
		{
			// ensure player has enough ore
			if (ore < SWORD_COST)
			{
				std::cout << "insufficient ore" << std::endl;
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
			std::cout << "insufficient ore, requires: " << upgrade_cost << ", have: " << ore << std::endl;
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
			std::cout << "requires: " << upgrade_cost << ", have: " << ore << std::endl;
		}
	);
}
void SwordService::Shutdown()
{

}
void SwordService::Tick(float dt)
{

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
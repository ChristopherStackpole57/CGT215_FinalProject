// Christopher Stackpole, 12/06/2025

#ifndef INPUT_SERVICE_H
#define INPUT_SERVICE_H

#include <functional>
#include <vector>
#include <unordered_map>

#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "Services/ServiceInterface.h"

constexpr sf::Keyboard::Scancode ARC_TURN_CCW = sf::Keyboard::Scancode::A;
constexpr sf::Keyboard::Scancode ARC_TURN_CW = sf::Keyboard::Scancode::D;
constexpr sf::Keyboard::Scancode ARC_SHOOT = sf::Keyboard::Scancode::W;

constexpr sf::Keyboard::Scancode SPAWN_SWORD = sf::Keyboard::Scancode::Q;
constexpr sf::Keyboard::Scancode SPAWN_SCOOP = sf::Keyboard::Scancode::E;

constexpr sf::Keyboard::Scancode UPRGRADE_SWORD_DAMAGE = sf::Keyboard::Scancode::Z;
constexpr sf::Keyboard::Scancode UPGRADE_SWORD_FIRERATE = sf::Keyboard::Scancode::X;

using ISCallback = std::function<void()>;
using ISMCallback = std::function<void(sf::Vector2i position)>;

class InputService : public IService
{
public:
	void Start() override;
	void Shutdown() override;
	void Tick(float dt) override;

	void RegisterInputListener(sf::Keyboard::Scancode bind_target, ISCallback action, bool on_press = true);
	void UnregisterInputListener(sf::Keyboard::Scancode bind_target, ISCallback action);
	void RegisterInputListener(sf::Mouse::Button bind_target, ISMCallback action, bool on_press = true);
	void UnregisterInputListener(sf::Mouse::Button bind_target, ISMCallback action);

	void ProcessEvent(const sf::Event& event);
private:
	std::unordered_map<sf::Keyboard::Scancode, std::vector<ISCallback>> key_press_bindings;
	std::unordered_map<sf::Keyboard::Scancode, std::vector<ISCallback>> key_release_bindings;
	std::unordered_map<sf::Mouse::Button, std::vector<ISMCallback>> mouse_press_bindings;
	std::unordered_map<sf::Mouse::Button, std::vector<ISMCallback>> mouse_release_bindings;

	void ProcessInput(sf::Keyboard::Scancode scancode, bool pressed);
	void ProcessInput(sf::Mouse::Button button, sf::Vector2i position, bool pressed);
};

#endif
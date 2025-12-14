// Christopher Stackpole, 12/13/2025

#include "Body.h"

Body::Body()
{

}
Body::Body(sf::Vector2f position) : position(position)
{

}

void Body::SetPosition(sf::Vector2f position)
{
	this->position = position;
}
sf::Vector2f Body::GetPosition()
{
	return position;
}
void Body::SetVelocity(sf::Vector2f velocity)
{
	this->velocity = velocity;
}
sf::Vector2f Body::GetVelocity()
{
	return velocity;
}
void Body::SetAcceleration(sf::Vector2f acceleration)
{
	this->acceleration = acceleration;
}
sf::Vector2f Body::GetAcceleration()
{
	return acceleration;
}

void Body::SetActive(bool active)
{
	this->active = active;
}
bool Body::GetActive()
{
	return active;
}
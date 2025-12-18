// Christopher Stackpole, 12/07/2025

#include "ResourceService.h"

void ResourceService::Start()
{

}
void ResourceService::Shutdown()
{

}
void ResourceService::Tick(float dt)
{

}

bool ResourceService::LoadResource(sf::Font& font, const std::string& path)
{
	bool result = font.openFromFile(path);
	if (!result)
	{
		std::cerr << "ResourceService: Encountered SFML Open Error: " << path << std::endl;
		return false;
	}

	return true;
}
bool ResourceService::LoadResource(sf::Texture& texture, const std::string& path)
{
	// TODO: Add error handling
	bool result = texture.loadFromFile(path);

	if (!result)
	{
		std::cerr << "ResourceService: Encountered SFML Load Error:" << path << std::endl;
		return false;
	}

	texture.setRepeated(false);
	texture.setSmooth(false);

	return true;
}
// Christopher Stackpole, 12/07/2025

#ifndef RESOURCE_SERVICE_H
#define RESOURCE_SERVICE_H

#include <iostream>
#include <filesystem>

#include <string>
#include <unordered_map>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Services/ServiceInterface.h"

class ResourceService : public IService
{
public:
	void Start() override;
	void Shutdown() override;
	void Tick(float dt) override;

	// While Load can be invoked at game time, the vast majority of resources are going to be loaded at game start
	// to help prevent lag spikes during gameplay
	template <typename T>
	T& Load(const std::string& path)
	{
		// Fetch the relevant resource cache
		std::unordered_map<std::string, std::unique_ptr<T>>& cache = GetResourceCache<T>();

		// Determine if the requested path already lives inside of the cache
		auto pair = cache.find(path);
		if (pair != cache.end())
		{
			// Found the requested path, return the resource living there instead of loading it
			return *(pair->second.get());
		}

		// The resource is not cached, so it must be loaded
		std::unique_ptr<T> resource = std::make_unique<T>();
		bool load_succeeded = LoadResource(*resource, path);
		if (!load_succeeded)
		{
			// TODO: Decide if I'm going to have default resources or if we're just going to throw an error
			//throw std::runtime_error("ResourceService: Failed top load resource: " + path);
			throw std::runtime_error("a");
			//return nullptr;
		}

		// Place the resource into the cache and give the user a raw pointer to the resource
		// Meanwhile ResourceService will maintain a unique pointer for stronger lifecycle management
		T* raw = resource.get();
		cache[path] = std::move(resource);
		return *raw;
	}
private:
	// By making this getter static, we ensure that only one cache for each resource type lives in ResourceService at a time
	template <typename T>
	static std::unordered_map<std::string, std::unique_ptr<T>>& GetResourceCache()
	{
		static std::unordered_map<std::string, std::unique_ptr<T>> cache;
		return cache;
	}

	// Overloads for resource loading
	bool LoadResource(sf::Font& font, const std::string& path);
	bool LoadResource(sf::Texture& texure, const std::string& path);
	bool LoadResource(sf::SoundBuffer& sound_buffer, const std::string& path);
};

#endif
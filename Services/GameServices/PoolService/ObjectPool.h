// Christopher Stackpole, 12/12/2025

#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include <iostream>
#include <memory>

#include "CallService.h"

#include "Services/GameServices/PoolService/ObjectPoolInterface.h"
#include "GameObjects/GameObjectInterface.h"

// Header only definition due to the massive usage of templates
template<typename T>
class ObjectPool : public IObjectPool
{
public:
	ObjectPool()
	{
		// TODO: 
	};
	~ObjectPool() override = default;

	void CreateCachedObject()						// The same as CreateObject but there is no reference returned
	{
	}
	T* CreateObject()
	{
		std::unique_ptr<T> new_object = std::make_unique<T>();
		T* raw = new_object.get();
		objects.push_back(std::move(new_object));

		if (IGameObject* game_obj = dynamic_cast<IGameObject*>(raw))
		{
			CallService* call_service = Services().Get<CallService>();
			call_service->SetObjectStartupPriority(game_obj, CLST_BASIC_GAMEOBJECT);
			call_service->SetObjectTickPriority(game_obj, CLT_BASIC);
		}

		return raw;
	}
	T* Get()
	{
		if (free.size() > 0)
		{
			T* obj = free.back();
			free.pop_back();
			return obj;
		}
		else
		{
			return CreateObject();
		}
	};
	void Release(T* obj)
	{
		free.push_back(obj);
	}

private:
	std::vector<std::unique_ptr<T>> objects;
	std::vector<T*> free;
};

#endif
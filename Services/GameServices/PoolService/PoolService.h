// Christopher Stackpole, 12/12/2025

#ifndef POOL_SERVICE_H
#define POOL_SERVICE_H

#include <memory>
#include <typeindex>
#include <unordered_map>

#include "Services/ServiceInterface.h"

#include "Services/GameServices/PoolService/ObjectPool.h"
#include "Services/GameServices/PoolService/ObjectPoolInterface.h"

class PoolService : public IService
{
public:
	void Start() override;
	void Shutdown() override;
	void Tick(float dt) override;

	template <typename T>
	T* Get()
	{
		std::type_index key = std::type_index(typeid(T));

		auto object_pool = object_pools.find(key);
		if (object_pool == object_pools.end())
		{
			// No pool yet, we need to create one
			std::unique_ptr<ObjectPool<T>> pool = std::make_unique<ObjectPool<T>>();
			ObjectPool<T>* raw = pool.get();
			
			// To avoid copying issues with unique_ptr
			object_pools.emplace(key, std::move(pool));

			return raw->Get();
		}

		// Get the interface pool pointer then cast to a pool of the provided type
		IObjectPool* ipool = object_pool->second.get();
		ObjectPool<T>* pool = static_cast<ObjectPool<T>*>(ipool);

		return pool->Get();
	};
	template <typename T>
	void Release(T* obj)
	{
		std::type_index key = std::type_index(typeid(T));
		auto ipool = object_pools.find(key);
		assert(ipool != object_pools.end());

		auto* pool = static_cast<ObjectPool<T>*>(ipool->second.get());
		pool->Release(obj);
	}
private:
	std::unordered_map<std::type_index, std::unique_ptr<IObjectPool>> object_pools;
};

#endif
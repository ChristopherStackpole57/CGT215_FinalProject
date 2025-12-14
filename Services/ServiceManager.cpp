// Christopher Stackpole, 12/06/2025

#include <cassert>

#include "ServiceManager.h"

#include "Services/EngineServices/CallService/CallService.h"

// Service Behavior
void ServiceManager::Start()
{
	// Ensure there is a call service that has been registered, otherwise throw an error.
	CallService* call_service = Get<CallService>();
	assert(call_service != nullptr, "ServiceManager: No registered CallService at program entry.");

	call_service->Start();
}
void ServiceManager::Shutdown()
{

}
void ServiceManager::Tick(float dt)
{
	CallService* call_service = Get<CallService>();
	assert(call_service != nullptr, "ServiceManager: No registered CallService at program tick.");

	call_service->Tick(dt);
}
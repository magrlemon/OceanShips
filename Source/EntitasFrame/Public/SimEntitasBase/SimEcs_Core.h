/** Copyright 2019 北京伯睿科技有限公司.
*	Author :	magi
	Date   :	11th July, 2019
	File   :	SimEcs_Core.h
	Summary:	Declares the engine class.
	All Rights Reserved.
	*http://www.borui.com	(c) Copyright 2019.
*/
#pragma once

#include "entt/entt.hpp"
#include <vector>
#include "EntitasFramePCH.h"
#include "LinearMemory.h"
#include "SimEntitasFrameModule.h"

DECLARE_CYCLE_STAT(TEXT("SimEcs: Total System Update"), STAT_TotalUpdate, STATGROUP_ECS);


using EntityID = std::uint64_t;
using SimEcs_Registry = entt::Registry<std::uint64_t>;
class SimEcs_World;
class AActor;

struct EntityHandle {
	EntityHandle(EntityID h = 0): handle(h) {};
	EntityID handle;
};

struct SystemT {

	AActor * OwnerActor;
	SimEcs_World * World;

	virtual ~SystemT() {}
	virtual void initialize(AActor * _Owner, SimEcs_World * _World) {
		OwnerActor = _Owner;
		World = _World;
	};
	virtual void update(SimEcs_Registry &registry, float dt) = 0;

};


class SimEcs_World {
public:
	~SimEcs_World() {
		for (auto s : systems)
		{
			delete s;
		}
		FreeLinearMemory(ScratchPad);
	}

	void InitializeSystems(AActor* _Owner)
	{
		//allocate 10 mb
		ScratchPad = AllocateLinearMemory(MEGABYTE * 10);

		Owner = _Owner;
		for (auto s : systems)
		{
			s->initialize(_Owner, this);
		}
	}


	void UpdateSystems(float DeltaTime)
	{
		SCOPE_CYCLE_COUNTER(STAT_TotalUpdate);

		for (auto s : systems)
		{
			s->update(registry, DeltaTime);
		}
	}

	template<typename T>
	SystemT* CreateAndRegisterSystem( )
	{
		SystemT * s = new T();
		if (s)
		{
			systems.push_back(s);
		}
		return s;
	}

	SystemT* GetArchetypeSpawnerSystem( )
	{
		return systems[0];   // 0 ==> ArchetypeSpawnerSystem
	}

	void RegisterSystem(SystemT* newSystem)
	{
		systems.push_back(newSystem);
	}
	EntityHandle NewEntity() {
		EntityHandle h;
		h.handle= registry.create();
		return h;
	}

	void DestroyEntity(EntityHandle ent)
	{
		registry.destroy(ent.handle);
	}

	SimEcs_Registry *GetRegistry() { return &registry; };
	FRandomStream rng;

	LinearMemory ScratchPad;

protected:

	AActor * Owner;

	std::vector<SystemT*> systems;

	SimEcs_Registry registry;
	
};
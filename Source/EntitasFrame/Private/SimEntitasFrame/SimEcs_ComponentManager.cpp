/*
	Author : magi
	Date   : 7th September, 2019
	File   : SimEcs_ComponentManager.cpp
	
	Manages all component container.

	All Rights Reserved. (c) Copyright 2019.
*/


#include "SimEcs_ComponentManager.h"



	SimEcs_ComponentManager::SimEcs_ComponentManager()
	{
		const size_t NUM_COMPONENTS = 25;// { SimEcs_FamilyTypeID<IComponentWrapper>::Get( ) };
		
		m_ComponentLUT.Init( nullptr, ENITY_LUT_GROW);
		int32 t =m_ComponentLUT.Num( );
		TArray<ComponentId > arr;
		this->m_EntityComponentMap.Init( arr, ENITY_LUT_GROW);
		for (auto i = 0; i < ENITY_LUT_GROW; ++i)
			this->m_EntityComponentMap[i].Init( INVALID_COMPONENT_ID, NUM_COMPONENTS );

	}

	SimEcs_ComponentManager::~SimEcs_ComponentManager()
	{
		for (auto cc : this->m_SimEcsComponentContainerRegistry)   
		{
			//LogDebug("Releasing remaining entities of type '%s' ...", cc.second->GetSimEcs_ComponentContainerTypeName());
			delete cc.Value;
			cc.Value = nullptr;
		}
		m_EntityComponentMap.Reset( );
		//LogInfo("Release SimEcs_ComponentManager!");
	}

	ComponentId SimEcs_ComponentManager::AqcuireComponentId(IComponentWrapper* component)
	{
		int32 i = 0;

		for (; i < this->m_ComponentLUT.Num(); ++i)
		{
			if (this->m_ComponentLUT[i] == nullptr)
			{
				this->m_ComponentLUT[i] = component;
				return i;
			}
		}

		// increase component LUT size
		this->m_ComponentLUT.SetNum(this->m_ComponentLUT.Num() + COMPONENT_LUT_GROW);

		this->m_ComponentLUT[i] = component;
		return i;
	}

	void SimEcs_ComponentManager::ReleaseComponentId(ComponentId id)
	{
		assert((id != INVALID_COMPONENT_ID && id < this->m_ComponentLUT.Num()) && "Invalid component id");
		this->m_ComponentLUT[id] = nullptr;
	}

	void SimEcs_ComponentManager::MapEntityComponent( SimEntityId entityId, ComponentId componentId, ComponentTypeId componentTypeId)
	{
		static const size_t NUM_COMPONENTS { SimEcs_FamilyTypeID<IComponentWrapper>::Get() };

		if ((this->m_EntityComponentMap.Num( ) - 1) < entityId.index)
		{
			size_t oldSize = this->m_EntityComponentMap.Num( );

			// we scale this map size along the entity lookup table size
			size_t newSize = oldSize + ENITY_LUT_GROW;
			
			this->m_EntityComponentMap.Reset(newSize);

			for (auto i = oldSize; i < newSize; ++i)
				this->m_EntityComponentMap[i].Reset(NUM_COMPONENTS);
		}

		// create mapping
		this->m_EntityComponentMap[entityId.index][componentTypeId] = componentId;
	}

	void SimEcs_ComponentManager::UnmapEntityComponent( SimEntityId entityId, ComponentId componentId, ComponentTypeId componentTypeId)
	{
		assert(this->m_EntityComponentMap[entityId.index][componentTypeId] == componentId && "FATAL: Entity Component ID mapping corruption!");

		// free mapping
		this->m_EntityComponentMap[entityId.index][componentTypeId] = INVALID_COMPONENT_ID;

		// free component id
		this->ReleaseComponentId(componentId);
	}

// namespace SimEcs
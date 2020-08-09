/*
	Author : magi
	Date   : 7th September, 2019
	File   : SimEcs_ComponentManager.h

	Manages all component container.

	All Rights Reserved. (c) Copyright 2019.
*/

#pragma once
#include "CoreMinimal.h"
#include "SimEcs_Engine.h"
#include "SimEcs_ComponentWrapper.h"
#include "SimEcs_Entity.h"
#include "util/FamilyTypeID.h"
#include "Memory/MemoryChunkAllocator.h"
//#include "SimEcs_ComponentManager.generated.h"
using SimEntityId = util::SimHandle64;

class   SimEcs_ComponentManager /*: GlobalMemoryUser*/ {

	friend class IComponentWrapper;

	//DECLARE_LOGGER

	class ISimEcs_ComponentContainer {
	public:

		virtual ~ISimEcs_ComponentContainer( )
		{}

		virtual const char* GetSimEcs_ComponentContainerTypeName( ) const = 0;

		virtual void DestroyComponent( IComponentWrapper* object ) = 0;
	};

	template<class T>
	class SimEcs_ComponentContainer : /*public MemoryChunkAllocator<T, COMPONENT_T_CHUNK_SIZE>,*/ public ISimEcs_ComponentContainer {
		SimEcs_ComponentContainer( const SimEcs_ComponentContainer& ) = delete;
		SimEcs_ComponentContainer& operator=( SimEcs_ComponentContainer& ) = delete;

	public:

		SimEcs_ComponentContainer( ) /*: MemoryChunkAllocator( "ComponentManager" )*/
		{}

		virtual ~SimEcs_ComponentContainer( )
		{}

		virtual const char* GetSimEcs_ComponentContainerTypeName( ) const override
		{
			static const char* COMPONENT_TYPE_NAME{ typeid(T).name( ) };
			return COMPONENT_TYPE_NAME;
		}

		virtual void DestroyComponent( IComponentWrapper* object ) override
		{
			// call d'tor
			//object->BeginDestory( );

			//this->DestroyObject( object );
		}

	}; // class SimEcs_ComponentContainer



	SimEcs_ComponentManager( const SimEcs_ComponentManager& ) = delete;
	SimEcs_ComponentManager& operator=( SimEcs_ComponentManager& ) = delete;

public:
	using SimEcs_ComponentContainerRegistry =  TMap<ComponentTypeId, ISimEcs_ComponentContainer*>;
	SimEcs_ComponentContainerRegistry m_SimEcsComponentContainerRegistry;

	template<class T>
	inline SimEcs_ComponentContainer<T>* GetSimEcs_ComponentContainer( )
	{
		ComponentTypeId CID = T::STATIC_COMPONENT_TYPE_ID;
		SimEcs_ComponentContainer<T>* scc = nullptr;
		auto it = m_SimEcsComponentContainerRegistry.Find( CID );
		if (!it){
			scc = new SimEcs_ComponentContainer<T>();
			this->m_SimEcsComponentContainerRegistry.Add( CID, scc );
		}
		else {
			scc = static_cast<SimEcs_ComponentContainer<T>*>(*it);
		}
		assert( scc != nullptr && "Failed to create SimEcs_ComponentContainer<T>!" );
		return scc;
	}

	using ComponentLookupTable = TArray<IComponentWrapper*>;
	ComponentLookupTable	m_ComponentLUT;

	using EntityComponentMap = TArray<TArray<ComponentId>>;
	EntityComponentMap		m_EntityComponentMap;


	ComponentId	AqcuireComponentId( IComponentWrapper* component );
	void		ReleaseComponentId( ComponentId id );

	void		MapEntityComponent( SimEntityId entityId, ComponentId componentId, ComponentTypeId componentTypeId );
	void		UnmapEntityComponent( SimEntityId entityId, ComponentId componentId, ComponentTypeId componentTypeId );

public:

	template<class T>
	using TComponentIterator = typename SimEcs_ComponentContainer<T>::iterator;

	SimEcs_ComponentManager( );
	~SimEcs_ComponentManager( );


	///-------------------------------------------------------------------------------------------------
	/// Fn:	template<class T, class ...ARGS> T* SimEcs_ComponentManager::AddComponent(const SimEntityId entityId,
	/// ARGS&&... args)
	///
	/// Summary:	Adds a component of type T to entity described by entityId.
	///
	/// Author:	magi
	///
	/// Date:	30/09/2019
	///
	/// Typeparams:
	/// T - 	   	Generic type parameter.
	/// ...ARGS -  	Type of the ...args.
	/// Parameters:
	/// entityId - 	Identifier for the entity.
	/// args - 	   	Variable arguments providing [in,out] The arguments.
	///
	/// Returns:	Null if it fails, else a pointer to a T.
	///-------------------------------------------------------------------------------------------------

	template<class T, class ...ARGS>
	T* AddComponent( const SimEntityId entityId, ARGS&&... args )
	{
		// hash operator for hashing entity and component ids
		static constexpr std::hash<ComponentId> ENTITY_COMPONENT_ID_HASHER{ std::hash<ComponentId>( ) };

		const ComponentTypeId CTID = T::STATIC_COMPONENT_TYPE_ID;

		WrapperDataInitializer wdl ;
		wdl.GrapData<u64>( std::forward<ARGS>( args )... );
		if (wdl.jsonValue.IsEmpty( ) && wdl.uHandleID<=0)
			return nullptr;

		// aqcuire memory for new component object of type T
		SimEcs_ComponentContainer<T>* pComContain = this->GetSimEcs_ComponentContainer<T>( );
		if (!pComContain)
			return nullptr; 

		T* ComponentWrapper = NewObject<T>( );
		ComponentId componentId = this->AqcuireComponentId( ComponentWrapper );
		ComponentWrapper->m_ComponentId = componentId;


		ComponentWrapper->AddToEntity( wdl.uHandleID,wdl.jsonValue );
		// create component inplace

		ComponentWrapper->m_SimOwner = entityId;
		ComponentWrapper->m_SimHashValue = ENTITY_COMPONENT_ID_HASHER( entityId ) ^ (ENTITY_COMPONENT_ID_HASHER( componentId ) << 1);

		// create mapping from entity id its component id
		MapEntityComponent( entityId, componentId, CTID );

		return  ComponentWrapper ;
	}

	///-------------------------------------------------------------------------------------------------
	/// Fn:	template<class T> void SimEcs_ComponentManager::RemoveComponent(const SimEntityId entityId)
	///
	/// Summary:	Removes the component of type T from an entity described by entityId.
	///
	/// Author:	magi
	///
	/// Date:	30/09/2019
	///
	/// Typeparams:
	/// T - 	Generic type parameter.
	/// Parameters:
	/// entityId - 	Identifier for the entity.
	///-------------------------------------------------------------------------------------------------

	template<class T>
	void RemoveComponent( const SimEntityId entityId )
	{
		const ComponentTypeId CTID = T::STATIC_COMPONENT_TYPE_ID;

		const ComponentId componentId = this->m_EntityComponentMap[entityId.index][CTID];

		IComponentWrapper* component = this->m_ComponentLUT[componentId];

		assert( component != nullptr && "FATAL: Trying to remove a component which is not used by this entity!" );

		// release object memory
		//GetSimEcs_ComponentContainer<T>( )->DestroyObject( component );

		// unmap entity id to component id
		UnmapEntityComponent( entityId, componentId, CTID );
	}

	void RemoveAllComponents( const SimEntityId entityId )
	{
		static const size_t NUM_COMPONENTS = this->m_EntityComponentMap[0].Num( );

		for (ComponentTypeId componentTypeId = 0; componentTypeId < NUM_COMPONENTS; ++componentTypeId) {
			const ComponentId componentId = this->m_EntityComponentMap[entityId.index][componentTypeId];
			if (componentId == INVALID_COMPONENT_ID)
				continue;

			IComponentWrapper* component = this->m_ComponentLUT[componentId];
			if (component != nullptr) {
				// get appropriate component container   need write by magi
			/*	auto it = this->m_SimEcsComponentContainerRegistry.find( componentTypeId );
				if (it != this->m_SimEcsComponentContainerRegistry.end( ))
					it->second->DestroyComponent( component );
				else
					assert( false && "Trying to release a component that wasn't created by SimEcs_ComponentManager!" );
*/
				// unmap entity id to component id
				UnmapEntityComponent( entityId, componentId, componentTypeId );
			}
		}
	}

	///-------------------------------------------------------------------------------------------------
	/// Fn:	template<class T> T* SimEcs_ComponentManager::GetComponent(const SimEntityId entityId)
	///
	/// Summary:	Get the component of type T of an entity. If component has no such component
	/// nullptr is returned.
	///
	/// Author:	magi
	///
	/// Date:	30/09/2019
	///
	/// Typeparams:
	/// T - 	Generic type parameter.
	/// Parameters:
	/// entityId - 	Identifier for the entity.
	///
	/// Returns:	Null if it fails, else the component.
	///-------------------------------------------------------------------------------------------------

	template<class T>
	T* GetComponent( const SimEntityId entityId )
	{
		const ComponentTypeId CTID = T::STATIC_COMPONENT_TYPE_ID;

		const ComponentId componentId = this->m_EntityComponentMap[entityId.index][CTID];

		// entity has no component of type T
		if (componentId == INVALID_COMPONENT_ID)
			return nullptr;

		return static_cast<T*>(this->m_ComponentLUT[componentId]);
	}

	///-------------------------------------------------------------------------------------------------
	/// Fn:	template<class T> inline TComponentIterator<T> SimEcs_ComponentManager::begin()
	///
	/// Summary:	Returns an forward iterator object that points to the beginning of a collection of 
	/// all components of type T.
	///
	/// Author:	magi
	///
	/// Date:	24/09/2019
	///
	/// Typeparams:
	/// T - 	Generic type parameter.
	///
	/// Returns:	A TComponentIterator&lt;T&gt;
	///-------------------------------------------------------------------------------------------------

	template<class T>
	inline TComponentIterator<T> begin( )
	{
		return GetSimEcs_ComponentContainer<T>( )->begin( );
	}

	///-------------------------------------------------------------------------------------------------
	/// Fn:	template<class T> inline TComponentIterator<T> SimEcs_ComponentManager::end()
	///
	/// Summary:	Returns an forward iterator object that points to the end of a collection of
	/// all components of type T.
	///
	/// Author:	magi
	///
	/// Date:	24/09/2019
	///
	/// Typeparams:
	/// T - 	Generic type parameter.
	///
	/// Returns:	A TComponentIterator&lt;T&gt;
	///-------------------------------------------------------------------------------------------------

	template<class T>
	inline TComponentIterator<T> end( )
	{
		return GetSimEcs_ComponentContainer<T>( )->end( );
	}

}; 
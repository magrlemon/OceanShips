///-------------------------------------------------------------------------------------------------
/// File:	include\SimEcs_IEntity.h.
///
/// Summary:	Base entity class containing no morte than entity id and state falgs.
///-------------------------------------------------------------------------------------------------

#pragma once
#include "CoreMinimal.h"
#include "Engine.h"
#include "SimApi.h"


using EntityTypeId = TypeID;
using SimEntityId = util::SimHandle64;
class SimEcs_ComponentManager;
static const SimEntityId INVALID_ENTITY_ID = util::SimHandle64::INVALID_HANDLE;

class   SimEcs_IEntity {
	friend class SimEcs_EntityManager;

private:

	// set on create; in EntityManager
	TSharedPtr<SimEcs_ComponentManager>	m_ComponentManagerInstancePtr = nullptr;

protected:

	// set on create; in EntityManager
	SimEntityId				m_EntityID;

	// if false, entity won't be updated
	bool					m_Active;

public:

	SimEcs_IEntity( ) {};
	virtual ~SimEcs_IEntity( ) {};

	template<class T>
	T* GetComponent( ) const
	{
		return this->m_ComponentManagerInstancePtr.Get( )->GetComponent<T>( this->m_EntityID );
	}

	template<class T, class ...P>
	T* AddComponent( P&&... param ) {
		return this->m_ComponentManagerInstancePtr.Get()->AddComponent<T>( this->m_EntityID, std::forward<P>( param )... );
	}

	template<class T>
	void RemoveComponent( )
	{
		this->m_ComponentManagerInstancePtr.RemoveComponent<T>( this->m_EntityID );
	}

	// COMPARE ENTITIES

	inline bool operator==( const SimEcs_IEntity& rhs ) const { return this->m_EntityID == rhs.m_EntityID; }
	inline bool operator!=( const SimEcs_IEntity& rhs ) const { return this->m_EntityID != rhs.m_EntityID; }
	inline bool operator==( const SimEcs_IEntity* rhs ) const { return this->m_EntityID == rhs->m_EntityID; }
	inline bool operator!=( const SimEcs_IEntity* rhs ) const { return this->m_EntityID != rhs->m_EntityID; }

	// ACCESORS
	virtual void GenHandEntity( )  abstract;
	virtual const EntityTypeId GetStaticEntityTypeID( ) const = 0;

	inline const SimEntityId GetEntityID( ) const { return this->m_EntityID; }


	using EntityHandleId = u64;
	virtual const EntityHandleId GetEntityHandleId( ) abstract;

	void SetActive( bool active );

	inline bool IsActive( ) const { return this->m_Active; }

	virtual void OnEnable( ) {}
	virtual void OnDisable( ) {}
};

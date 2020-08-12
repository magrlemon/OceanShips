/*
	Author : magi
	Date   : 3rd July, 2019
	File   : SimEcs_Entity.h
	
	Enity class.

	All Rights Reserved. (c) Copyright 2019.
*/
#pragma once

#include "SimEcs_Platform.h"
#include "SimEcs_IEntity.h"
#include "Sim_OceanSceneManager.h"


	///-------------------------------------------------------------------------------------------------
	/// Class:	SimEcs_Entity
	///
	/// Summary:	CRTP class. Any entity object should derive form the SimEcs_Entity class and passes itself
	/// as template parameter to the SimEcs_Entity class.
	///
	/// Author:	magi
	///
	/// Date:	30/09/2019
	///
	/// Typeparams:
	/// E - 	Type of the e.
	///-------------------------------------------------------------------------------------------------

	enum class TransformType {
		none = 0,
		position = 1 << 0,
		rotation = 1 << 1,
		scale = 1 << 2,
		all = position | rotation | scale
	};


	template<class E>
	class SimEcs_Entity : public SimEcs_IEntity
	{
		// SimEcs_Entity destruction always happens through EntityManager !!!
		void operator delete(void*) = delete;
		void operator delete[](void*) = delete;
	public:
		enum {
			// nodes
			ENTITY_DUMMY = 0,
			ENTITY_LAYER,
			ENTITY_PIVOT,
			ENTITY_TRIGGER,
			ENTITY_REFERENCE,
			ENTITY_EXTERN,

			// lights
			ENTITY_LIGHT_BEGIN,
			ENTITY_LIGHT_ENVIRONMENT_PROBE = ENTITY_LIGHT_BEGIN,
			ENTITY_LIGHT_OMNI,
			ENTITY_LIGHT_PROJ,
			ENTITY_LIGHT_WORLD,
			ENTITY_LIGHT_END = ENTITY_LIGHT_WORLD,

			// decals
			ENTITY_DECAL_BEGIN,
			ENTITY_DECAL_PROJ = ENTITY_DECAL_BEGIN,
			ENTITY_DECAL_ORTHO,
			ENTITY_DECAL_MESH,
			ENTITY_DECAL_END = ENTITY_DECAL_MESH,

			// objects
			OBJECT_BEGIN,
			OBJECT_DUMMY = OBJECT_BEGIN,
			OBJECT_DYNAMIC,

			OBJECT_MESH_STATIC,
			OBJECT_MESH_CLUSTER,
			OBJECT_MESH_CLUTTER,
			OBJECT_MESH_SKINNED,
			OBJECT_MESH_DYNAMIC,

			OBJECT_TEXT,
			OBJECT_END = OBJECT_TEXT,

			// PLAYER
			ENTITY_PLAYER_BEGIN,
			ENTITY_PLAYER_DUMMY = ENTITY_PLAYER_BEGIN,
			ENTITY_PLAYER_SPECTATOR,   // ¹ÛÖÚ
			ENTITY_PLAYER_AI_ACTOR,
			ENTITY_PLAYER_ACTOR,
			ENTITY_PLAYER_END = ENTITY_PLAYER_ACTOR,

			//Vehicle
			ENTITY_VECHICLE_TANK,
			ENTITY_VECHICLE_CAR_2W,
			ENTITY_VECHICLE_CAR_4W,
			ENTITY_VECHICLE_CAR_6W,
			ENTITY_VECHICLE_CAR_8W,

			//Short-Gun
			ENTITY_WEAPON_SHORTGUN,
			ENTITY_WEAPON_SEMI_AUTOMATIC_RIFLE,  //°ë×Ô¶¯²½Ç¹
			ENTITY_WEAPON_SNIPER_RIFLE,			 //¾Ñ»÷Ç¹
			ENTITY_WEAPON_TELESCOPE,			 //ÍûÔ¶¾µ
			ENTITY_WEAPON_RPG,					 //»ð¼ýÖúÍÆÁñµ¯

			//TEXT
			ENTITY_TEXT,
			ENTITY_END = ENTITY_TEXT,


			// obstacles
			OBSTACLE_BEGIN,
			OBSTACLE_BOX = OBSTACLE_BEGIN,
			OBSTACLE_SPHERE,
			OBSTACLE_CAPSULE,
			OBSTACLE_END = OBSTACLE_CAPSULE,


			// counters
			NUM_NENTITYS,

			NUM_LIGHTS = ENTITY_LIGHT_END - ENTITY_LIGHT_BEGIN + 1,
			NUM_DECALS = ENTITY_DECAL_END - ENTITY_DECAL_BEGIN + 1,
			NUM_OBJECTS = OBJECT_END - OBJECT_BEGIN + 1,
			NUM_PLAYERS = ENTITY_PLAYER_END - ENTITY_PLAYER_BEGIN + 1,

			NUM_OBSTACLES = OBSTACLE_BEGIN - OBSTACLE_END + 1,
		};

	public:
		using EntityTypeId = TypeID;
		static const  EntityTypeId STATIC_ENTITY_TYPE_ID;

		using EntityHandleId = u64;
		EntityHandleId m_eHandleID;
	public:

		virtual void GenHandEntity( ) override {
	
			auto osmSingleton = USimOceanSceneManager_Singleton::GetInstance( );
			if (osmSingleton == nullptr)
				return;
		
			USimEcs_ComponentSystemLink* sysLink_WeakPtr = osmSingleton->GetComponentSysLinkPtr( );
			if (!sysLink_WeakPtr)return;
			auto entFrameDate = sysLink_WeakPtr->GetEntityFrameData( );
			if (!sysLink_WeakPtr || !entFrameDate.IsValid())
				return;
			FString  strValue;			
			GEngine->AddOnScreenDebugMessage( -1, 8.f, FColor::Red, strValue.FromInt( _type ) );
			TSubclassOf<ASimEcs_Archetype> typeClass =  entFrameDate.Pin()->GetSubClassByType( _type );//
			if (!typeClass.Get( ))
				return;
			EntityHandle eh = sysLink_WeakPtr->GenHandEntityID( );
			EntityHandleId handleID = m_eHandleID = eh.handle;

			auto simRegistry = osmSingleton->GetSimRegistry();
			simRegistry->assign<FPrePosition>( handleID );
			simRegistry->assign<FRotationComponent>( handleID );
			simRegistry->assign<FLifetime>( handleID );
			simRegistry->assign<FArchetypeSpawner>( handleID );
			simRegistry->get<FPrePosition>( handleID ).pos = _pos;
			simRegistry->get<FRotationComponent>( handleID ).rot = FQuat::MakeFromEuler(FVector(0.0f,0.0f, _heading) );
			simRegistry->get<FLifetime>( handleID ).LifeLeft = 0.5f;
			FArchetypeSpawner &spawn = simRegistry->get<FArchetypeSpawner>( handleID );
			spawn.Name = *_name;
			spawn.GroupName = _groupName;
			spawn.ActorType = _type;
			spawn.bLoopSpawn = false;
			spawn.isLeader = _isLeader;
			spawn.ArchetypeClass = typeClass;
			spawn.SpawnRate = 1;
			spawn.TimeUntilSpawn = 0.1f;
			EntityHandle entityHandleID = sysLink_WeakPtr->GenHandEntityID( );
			m_eHandleID = entityHandleID.handle;
			spawn.entHandleId = m_eHandleID;
		}

		virtual const EntityTypeId GetStaticEntityTypeID() const override { return STATIC_ENTITY_TYPE_ID; }


		virtual const EntityHandleId GetEntityHandleId( ) { return m_eHandleID; };
		SimEcs_Entity(int type,FVector entPos, float heading, FString name ,bool isLeader, FString groupName ):SimEcs_IEntity()
		{
			_type = type;
			_pos = entPos;
			_heading = heading;
			_name = name;
			_isLeader = isLeader;
			_groupName = groupName;
		}

		virtual ~SimEcs_Entity()
		{
		}

		public:
		// Occurs when a component gets added. All event handlers will be added when the entity gets destroyed by the pool.
		//Event EntityChanged OnComponentAdded;

		///// Occurs when a component gets removed. All event handlers will be removed when the entity gets destroyed by the pool.
		//Event EntityChanged OnComponentRemoved;

		//// Occurs when a component gets replaced. All event handlers will be Replaced when the entity gets destroyed by the pool.
		//Event ComponentReplaced OnComponentReplaced;

		//	EventDelegate void EntityChanged( SimEcs_Entity entity, int index, Component component ) {};
		//	EventDelegate void ComponentReplaced( SimEcs_Entity entity, int index, Component previousComponent, Component newComponent ) {};

		private:
			//TSubclassOf<AActor> m_ActorClass_BP;
			TypeID _type = ENTITY_DUMMY;
			FVector _pos = FVector::ZeroVector;
			float _heading = 0.0f;
			FString _name = "";
			bool _isLeader = false ;
			FString _groupName = "";
	};

	using EntityTypeId = TypeID;
	// set unique type id for this SimEcs_Entity<T>
	template<class E>
	const EntityTypeId SimEcs_Entity<E>::STATIC_ENTITY_TYPE_ID = SimEcs_FamilyTypeID<SimEcs_IEntity>::Get<E>();



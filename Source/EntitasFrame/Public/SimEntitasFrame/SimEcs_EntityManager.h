/** Copyright 2019 北京伯睿科技有限公司.
*	Author : magi
	Date   : 11th July, 2019
	File   : SimEcs_EntityManager.h
	
	Enity manager class.

	All Rights Reserved. *http://www.borui.com 
	(c) Copyright 2019.
*/
#pragma once
#include "CoreMinimal.h"

#include "SimEcs_ComponentManager.h"

class GlobalMemoryUser;
#pragma warning(push)

// warning C4291: 'void *operator new(::size_t,void *) throw()': no matching operator delete found; memory will not be freed if initialization throws an exception
// note we are using custom memory allocator no need for delete
#pragma warning(disable: 4291)

using EntityTypeId = TypeID;
using SimEntityId = util::SimHandle64;

using SimEntityHandleTable = util::SimHandleTable<SimEcs_IEntity, SimEntityId>;

	class   SimEcs_EntityManager
	{
		//DECLARE_LOGGER

		class SimEcs_IEntityContainer
		{
		public:

			virtual ~SimEcs_IEntityContainer()
			{}

			virtual const char* GetEntityContainerTypeName() const = 0;

			virtual void DestroyEntity(SimEcs_IEntity* object) = 0;

		}; // class SimEcs_IEntityContainer

		///-------------------------------------------------------------------------------------------------
		/// Class:	EntityContainer
		///
		/// Summary:	An entity container that manages memory chunks of enities T.
		///
		/// Author:	magi
		///
		/// Date:	23/09/2019
		///
		/// Typeparams:
		/// T - 	Generic type parameter.
		///-------------------------------------------------------------------------------------------------

		template<class T>
		class EntityContainer: public MemoryChunkAllocator<T, ENITY_T_CHUNK_SIZE>, public SimEcs_IEntityContainer
		{

			EntityContainer(const EntityContainer&) = delete;
			EntityContainer& operator=(EntityContainer&) = delete;

		public:

			EntityContainer() : MemoryChunkAllocator("SimEcs_EntityManager")
			{}

			virtual ~EntityContainer()
			{}

			virtual const char* GetEntityContainerTypeName() const override
			{ 
				static const char* ENTITY_TYPE_NAME{ typeid(T).name() };
				return ENTITY_TYPE_NAME;
			}

			virtual void DestroyEntity(SimEcs_IEntity* object) override
			{
				// call d'tor
				object->~SimEcs_IEntity();

				this->DestroyObject(object);
			}

		}; // EntityContainer

		using EntityRegistry = TMap<EntityTypeId, SimEcs_IEntityContainer*>;
		EntityRegistry m_EntityRegistry;

		using PendingDestroyedEntities = std::vector<SimEntityId>;
		PendingDestroyedEntities m_PendingDestroyedEntities;
		size_t m_NumPendingDestroyedEntities;

		/// Summary:	The component manager instance.
		TSharedPtr<SimEcs_ComponentManager>	m_ComponentManagerInstancePtr;

	private:	

		SimEcs_EntityManager(const SimEcs_EntityManager&) = delete;
		SimEcs_EntityManager& operator=(SimEcs_EntityManager&) = delete;

		SimEntityHandleTable m_EntityHandleTable;

		///-------------------------------------------------------------------------------------------------
		/// Fn:	template<class T> inline EntityContainer<T>* SimEcs_EntityManager::GetEntityContainer()
		///
		/// Summary:	Returns/Creates an entity container for entities of type T.
		///
		/// Author:	magi
		///
		/// Date:	23/09/2019
		///
		/// Typeparams:
		/// T - 	Generic type parameter.
		///
		/// Returns:	Null if it fails, else the entity container.
		///-------------------------------------------------------------------------------------------------

		template<class T>
		inline EntityContainer<T>* GetEntityContainer()
		{
			EntityTypeId EID = T::STATIC_ENTITY_TYPE_ID;

			auto it = this->m_EntityRegistry.Find(EID);
			EntityContainer<T>* ec = nullptr;
						
			if (!it)
			{
				ec = new EntityContainer<T>();
				this->m_EntityRegistry.Add(EID, ec ) ;
			}
			else
				ec = (EntityContainer<T>*)(m_EntityRegistry[EID]);

			assert(ec != nullptr && "Failed to create EntityContainer<T>!");
			return ec;
		}

		///-------------------------------------------------------------------------------------------------
		/// Fn:	SimEntityId SimEcs_EntityManager::AqcuireSimEntityId(SimEcs_IEntity* entity);
		///
		/// Summary:	Aqcuire entity identifier. This method will be used be SimEcs_IEntity class c'tor to set
		/// the entity id on creation.
		///
		/// Author:	magi
		///
		/// Date:	24/09/2019
		///
		/// Parameters:
		/// entity - 	[in,out] If non-null, the entity.
		///
		/// Returns:	An SimEntityId.
		///-------------------------------------------------------------------------------------------------

		SimEntityId AqcuireSimEntityId(SimEcs_IEntity* entity);

		///-------------------------------------------------------------------------------------------------
		/// Fn:	void SimEcs_EntityManager::ReleaseSimEntityId(SimEntityId id);
		///
		/// Summary:	Releases the entity identifier for reuse.
		///
		/// Author:	magi
		///
		/// Date:	23/09/2019
		///
		/// Parameters:
		/// id - 	The identifier.
		///-------------------------------------------------------------------------------------------------

		void ReleaseSimEntityId(SimEntityId id);	

	public:

		SimEcs_EntityManager( TSharedPtr<SimEcs_ComponentManager> componentManagerInstance );
		~SimEcs_EntityManager();

		///-------------------------------------------------------------------------------------------------
		/// Fn:	template<class T, class... ARGS> SimEntityId SimEcs_EntityManager::CreateEntity(ARGS&&... args)
		///
		/// Summary:	Creates an entity of type T and returns its id.
		///
		/// Author:	magi
		///
		/// Date:	30/09/2019
		///
		/// Typeparams:
		/// T - 	   	Generic type parameter.
		/// ARGS - 	   	Type of the arguments.
		/// Parameters:
		/// args - 	Variable arguments providing [in,out] The arguments.
		///
		/// Returns:	The new entity.
		///-------------------------------------------------------------------------------------------------

		template<class T, class... ARGS>
		SimEntityId CreateEntity(ARGS&&... args)
		{		
			// aqcuire memory for new entity object of type T
			void* pObjectMemory =  GetEntityContainer<T>( )->CreateObject( );

			SimEntityId entityId = this->AqcuireSimEntityId((T*)pObjectMemory);

			// create entity inplace
			SimEcs_IEntity* entity = new (pObjectMemory)T(std::forward<ARGS>(args)...);	

			entity->m_EntityID = entityId;
			entity->m_ComponentManagerInstancePtr = this->m_ComponentManagerInstancePtr;
			entity->GenHandEntity( );
			return entityId;
		}


		void DestroyEntity(SimEntityId entityId)
		{
			SimEcs_IEntity* entity = this->m_EntityHandleTable[entityId];
		
			const EntityTypeId ETID = entity->GetStaticEntityTypeID();

			if (this->m_NumPendingDestroyedEntities < this->m_PendingDestroyedEntities.size())
			{
				this->m_PendingDestroyedEntities[this->m_NumPendingDestroyedEntities++] = entityId;
			}
			else
			{
				this->m_PendingDestroyedEntities.push_back(entityId);
				this->m_NumPendingDestroyedEntities++;
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// Fn:	inline void* SimEcs_EntityManager::GetEntity(const SimEntityId id)
		///
		/// Summary:	Get an entity object by its id.
		///
		/// Author:	magi
		///
		/// Date:	23/09/2019
		///
		/// Parameters:
		/// id - 	The identifier.
		///
		/// Returns:	Null if it fails, else the entity.
		///-------------------------------------------------------------------------------------------------

		inline SimEcs_IEntity* GetEntity(SimEntityId entityId)
		{
			return this->m_EntityHandleTable[entityId];
		}

		///-------------------------------------------------------------------------------------------------
		/// Fn:	inline SimEntityId SimEcs_EntityManager::GetSimEntityId(SimEntityId::index_type index)
		///
		/// Summary:	Gets the currently stored entity identifier at the given index.
		///
		/// Author:	magi
		///
		/// Date:	2/10/2019
		///
		/// Parameters:
		/// index - 	Zero-based index of the.
		///
		/// Returns:	The entity identifier.
		///-------------------------------------------------------------------------------------------------

		inline SimEntityId GetSimEntityId(SimEntityId::value_type index) const
		{
			return this->m_EntityHandleTable[index];
		}

		///-------------------------------------------------------------------------------------------------
		/// Fn:	void SimEcs_EntityManager::RemoveDestroyedEntities();
		///
		/// Summary:	Removes all destroyed entities.
		///
		/// Author:	magi
		///
		/// Date:	8/10/2019
		///-------------------------------------------------------------------------------------------------

		void RemoveDestroyedEntities();
	};

#pragma warning(pop)

///-------------------------------------------------------------------------------------------------
/// File:	src\SimEcs_EntityManager.cpp.
///
/// Summary:	Implements the entity manager class.
///-------------------------------------------------------------------------------------------------


#include "SimEcs_EntityManager.h"


	SimEcs_EntityManager::SimEcs_EntityManager(TSharedPtr<SimEcs_ComponentManager> componentManagerInstance) :
		m_PendingDestroyedEntities(1024),
		m_NumPendingDestroyedEntities(0),
		m_ComponentManagerInstancePtr(componentManagerInstance)
	{
		//DEFINE_LOGGER("SimEcs_EntityManager")
		//LogInfo("Initialize SimEcs_EntityManager!")

	}

	SimEcs_EntityManager::~SimEcs_EntityManager()
	{
		for (auto ec : this->m_EntityRegistry)
		{
			//LogDebug("Releasing remaining entities of type '%s' ...", ec.second->GetEntityContainerTypeName());
			//delete ec.second;
			//ec.second = nullptr;
		}

		//LogInfo("Release SimEcs_EntityManager!")
	}

	SimEntityId SimEcs_EntityManager::AqcuireSimEntityId(SimEcs_IEntity* entity)
	{
		return this->m_EntityHandleTable.AqcuireSimHandle(entity);
	}

	void SimEcs_EntityManager::ReleaseSimEntityId(SimEntityId id)
	{
		this->m_EntityHandleTable.ReleaseSimHandle(id);
	}

	void SimEcs_EntityManager::RemoveDestroyedEntities()
	{
		for (size_t i = 0; i < this->m_NumPendingDestroyedEntities; ++i)
		{
			SimEntityId entityId = this->m_PendingDestroyedEntities[i];

			SimEcs_IEntity* entity = this->m_EntityHandleTable[entityId];

			const EntityTypeId ETID = entity->GetStaticEntityTypeID();

			// get appropriate entity container and destroy entity need write by magi
			auto it = this->m_EntityRegistry.Find(ETID);
			if (!it )
			{
				// release entity's components
				this->m_ComponentManagerInstancePtr->RemoveAllComponents(entityId);
			   
				//it->DestroyEntity(entity);
			}

			// free entity id
			this->ReleaseSimEntityId(entityId);
		}

		this->m_NumPendingDestroyedEntities = 0;
	}

 // namespace SimEcs

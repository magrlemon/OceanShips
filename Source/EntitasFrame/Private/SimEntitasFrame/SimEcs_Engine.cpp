///-------------------------------------------------------------------------------------------------
/// File:	src\Engine.cpp.
///
/// Summary:	Implements the engine class.
///-------------------------------------------------------------------------------------------------


#include "SimEcs_Engine.h"
#include "SimEcs_EntityManager.h"
#include "SimEcs_ComponentManager.h"



	SimEcs_Engine::SimEcs_Engine()
	{
		m_SimEcsComponentManagerPtr	= MakeShareable(new SimEcs_ComponentManager());
		m_SimEcsEntityManagerPtr = MakeShareable<SimEcs_EntityManager>(new SimEcs_EntityManager(this->m_SimEcsComponentManagerPtr));
	}

	SimEcs_Engine::~SimEcs_Engine()
	{
		m_SimEcsEntityManagerPtr = nullptr;
			m_SimEcsComponentManagerPtr = nullptr;
			UE_LOG( LogFlying, Warning, TEXT( "	SimEcs_Engine::~SimEcs_Engine()" ) );
	}   

	 
	void SimEcs_Engine::Update(f32 tick_ms)
	{

		// Finalize pending destroyed entities
		m_SimEcsEntityManagerPtr->RemoveDestroyedEntities();

	}


	 // namespace SimEcs

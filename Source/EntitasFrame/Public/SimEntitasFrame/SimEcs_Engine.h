/** Copyright 2019 北京伯睿科技有限公司.
*	Author :	magi
	Date   :	11th July, 2019
	File   :	Engine.h
	Summary:	Declares the engine class.
	All Rights Reserved.
	*http://www.borui.com	(c) Copyright 2019.
*/
///-------------------------------------------------------------------------------------------------
/// File:	include\Engine.h.
///
/// Summary:	Declares the engine class.
///-------------------------------------------------------------------------------------------------

#ifndef __SIMECS_ENGINE_H__
#define __SIMECS_ENGINE_H__


#include "Engine.h"
#include "SimApi.h"

class   PropertyData {
public:
	PropertyData( ) {
		maxSpeed = 100.0f;
		launchSpeed = 1.0f;
		minPower = 0.0f;
		maxPower = 1000.0f;
		acceleration = 1.0f;
	};


	static const EntityTypeId STATIC_ENTITY_TYPE_ID = 5;


	float maxSpeed;
	float launchSpeed;
	float minPower;
	float maxPower;
	float acceleration;
};

	class SimEcs_EntityManager;
	class SystemManager;
	class SimEcs_ComponentManager;

	class   SimEcs_Engine
	{
		friend class SimEcs_IEntity;
		friend class IComponentWrapper;

		friend class SimEcs_EntityManager;

	private:


		TSharedPtr <SimEcs_EntityManager>			m_SimEcsEntityManagerPtr;

		TSharedPtr<SimEcs_ComponentManager>			m_SimEcsComponentManagerPtr;



		SimEcs_Engine(const SimEcs_Engine&) = delete;
		SimEcs_Engine& operator=(SimEcs_Engine&) = delete;


	public:

		SimEcs_Engine();

		~SimEcs_Engine();

		inline TSharedPtr<SimEcs_EntityManager> GetEntityManager() {
			return m_SimEcsEntityManagerPtr; }

		inline TSharedRef<SimEcs_ComponentManager> GetComponentManager() { return m_SimEcsComponentManagerPtr.ToSharedRef(); }

		///-------------------------------------------------------------------------------------------------
		/// Fn:	template<class E, class... ARGS> void SimEcs_Engine::SendEvent(ARGS&&... eventArgs)
		///
		/// Summary:	Broadcasts an event.
		///
		/// Author:	magi
		///
		/// Date:	3/10/2019
		///
		/// Typeparams:
		/// E - 	   	Type of the e.
		/// ARGS - 	   	Type of the arguments.
		/// Parameters:
		/// eventArgs - 	Variable arguments providing [in,out] The event arguments.
		///-------------------------------------------------------------------------------------------------

		template<class E, class... ARGS>
		void SendEvent(ARGS&&... eventArgs)
		{
			//ECS_EventHandler->Send<E>(std::forward<ARGS>(eventArgs)...);
		}

		///-------------------------------------------------------------------------------------------------
		/// Fn:	void SimEcs_Engine::Update(f32 tick_ms);
		///
		/// Summary:	Updates the entire SimEcs with a given delta time in milliseconds.
		///
		/// Author:	magi
		///
		/// Date:	3/10/2019
		///
		/// Parameters:
		/// tick_ms - 	The tick in milliseconds.
		///-------------------------------------------------------------------------------------------------

		void Update(f32 tick_ms);

		using TMapScenario = const  TMap<FString, FString>;
		bool GenerateOBEntitys( int entType, TMapScenario & refScenarioXmlData);

		/*Analysis XML data by Entity Type */
		void AnalysisEntityPropertys( );
	};


#endif // __SIMECS_ENGINE_H__
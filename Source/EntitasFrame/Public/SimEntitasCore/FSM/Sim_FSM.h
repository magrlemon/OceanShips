#pragma once

#include "CoreMinimal.h"
#include "SimEcs_Engine.h"
#include "FSM\Sim_IFSM.h"
#include "FSM\Sim_FsmStateDataBase.h"
/// <summary>
/// 状态机类
/// </summary>

class FSMachine : public IFSMInterface {
private:

	
	float m_CurrentStateTime = 0;
	bool m_IsRunning = false;
	FName m_Name = "";
	AActor* m_Owner = nullptr;

public:
	FSMachine( );
	~FSMachine( );
	virtual IFsmStateInterface* GetCurrentState( ) override
	{
		return m_pCurrentState;
	}

	virtual void SetCurrentState( IFsmStateInterface*  value ) override
	{
		m_pCurrentState = value;
	}

	virtual float GetCurrentStateTime( ) override
	{
		return m_CurrentStateTime;
	}

	virtual void SetCurrentStateTime( float value ) override
	{
		m_CurrentStateTime = value;
	}

	virtual bool IsRunning( ) override
	{
		return m_IsRunning;
	}

	virtual void SetRunning( bool value ) override
	{
		m_IsRunning = value;
	}

	virtual FName GetName( )  override  {
		return m_Name;
	}

	virtual void SetName( FName value ) override
	{
		m_Name = value;
	}

	virtual AActor* GetOwner( ) override
	{
		return m_Owner;
	}

	virtual void SetOwner( AActor* value ) override
	{
		m_Owner = value;
	}

	//template<class T>
	//void AddState( )
	//{
	//	string typeName = typeof( T ).ToString( );
	//	if (m_States.Contains( typeName ))
	//		return;
	//	T t = new T( );
	//	t.Init( this );
	//	m_States.Add( typeName, t );
	//}

	virtual void AddState( IFsmStateInterface* state ) ;

	/*
		template<class T>
		void RemoveState( )
		{
			string typeName = typeof( T ).ToString( );
			IFSMInterface* state = m_States.Find( typeName );
			if (state) {
				state->Destroy( this );
				m_States->Remove( typeName );
			}
		}
		template<class T>
		T* GetState( )
		{
			string typeName = typeof( T ).ToString( );
			if (m_States.Contains( typeName ))
				return Cast<T>( m_States[typeName] )  ;
			return nullptr;
		}
		*/
	/*template<class T>
	void ChangeState( FsmStateDataBase data )
	{
		FString typeName = typeof( T ).ToString( );
		auto state = m_States.Find( typeName );
		if (state) {
			if (m_pCurrentState != nullptr)
				m_pCurrentState->Exit( this );
			(*state)->Enter( this, data );
			m_pCurrentState = (*state);
		}
	}*/
	virtual void ChangeState( IFsmStateInterface* state, FsmStateDataBase data ) ;

	virtual void PauseFsm( ) ;

	virtual void ResetFsm( ) ;

	virtual void ResumeFsm( ) ;

	virtual void Update( float frameTime, float realTime ) ;

	virtual void GetAllStateType( TArray<IFsmStateInterface*>& list ) ;

	public:
		void InitFsm( AActor* owner, FName fsmName, TArray<IFsmStateInterface*>& states );
		void DestroyFsm( );
	
};


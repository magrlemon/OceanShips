#pragma once


#include "CoreMinimal.h"
#include "SimEcs_Engine.h"
//#include "FSM/Sim_IFsmState.h"

class IFsmStateInterface;

class  IFSMInterface {

public:

	IFSMInterface( ) {};
	virtual ~IFSMInterface( ) {};

	/// <summary>
   /// 状态机名称
   /// </summary>
	virtual FName GetName( )  abstract;

	/// <summary>
   /// 状态机拥有者
   /// </summary>
	virtual AActor* GetOwner( ) abstract;

	virtual IFsmStateInterface* GetCurrentState( ) abstract;

	virtual void SetCurrentState( IFsmStateInterface*  value ) abstract;



	virtual void SetName( FName value ) abstract;

	virtual void SetOwner( AActor* value ) abstract;


	virtual void SetCurrentStateTime( float value ) abstract;
	virtual float GetCurrentStateTime( ) abstract;


	virtual void Update( float frameTime, float realTime ) abstract;

	virtual void SetRunning( bool value ) abstract;

	/// <summary>
   /// 是否正在运行
   /// </summary>
	virtual bool IsRunning( ) abstract;

	/// <summary>
	/// 添加状态
	/// </summary>
	/// <typeparam name="T">要添加的状态类型</typeparam>
	template<class T>
	void AddState( T state )
	{
		FName typeName = typeid( T ).name( );
		if (m_States.Contains( typeName ))
			return;
		T t = new T( );
		t.Init( this );
		m_States.Add( typeName, t );
	}

	/// <summary>
	/// 添加状态
	/// </summary>
	/// <param name="state">要添加的状态</param>
	virtual void AddState( IFsmStateInterface* state ) abstract;
	/*/// <summary>
	/// 删除状态
	/// </summary>
	/// <typeparam name="T">要删除的状态类型</typeparam>*/
	template<class T>
	void RemoveState( T* state )
	{
		FName typeName = typeid(T).name( );
		IFSMInterface* state = m_States.Find( typeName );
		if (state) {
			state->Destroy( this );
			m_States->Remove( typeName );
		}
	}
	/// <summary>
	/// 获得状态
	/// </summary>
	/// <typeparam name="T">要获得的状态类型</typeparam>
	/// <returns>获得状态</returns>
	template<class T>
	T* GetState( )
	{
		FName typeName = typeid(T).name( );
		if (m_States.Contains( typeName ))
			return Cast<T>( m_States[typeName] );
		return nullptr;
	}
	/// <summary>
	/// 切换状态
	/// </summary>
	/// <typeparam name="T">要切换的状态类型</typeparam>
	/// <param name="data">状态数据</param>
	template<class T>
	void ChangeState( FsmStateDataBase data )
	{
		FName typeName = typeid( T ).name();
		auto state = m_States.Find( typeName );
		if (state) {
			if (m_pCurrentState != nullptr)
				m_pCurrentState->Exit( this );
			(*state)->Enter( this, data );
			m_pCurrentState = (*state);
		}
	}
	/// <summary>
	/// 切换状态
	/// </summary>
	/// <param name="state">要切换的状态</param>
	/// <param name="data">状态数据</param>
	virtual void ChangeState( IFsmStateInterface* state, FsmStateDataBase data ) abstract;
	/// <summary>
	/// 暂停状态机
	/// </summary>
	virtual void PauseFsm( ) abstract;
	/// <summary>
	/// 恢复状态机
	/// </summary>
	virtual void ResumeFsm( ) abstract;
	/// <summary>
	/// 重置状态机
	/// </summary>
	virtual void ResetFsm( ) abstract;
	/// <summary>
	/// 获得所有状态
	/// </summary>
	virtual void GetAllStateType( TArray<IFsmStateInterface*>& list ) abstract;


	IFsmStateInterface* m_pCurrentState = nullptr;

	TMap<FName, IFsmStateInterface*> m_States;
};



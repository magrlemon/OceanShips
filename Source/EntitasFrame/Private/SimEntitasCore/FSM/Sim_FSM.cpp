
#include "FSM\Sim_FSM.h"


/// <summary>
/// 状态机类
/// </summary>
FSMachine::FSMachine( )
	:  m_CurrentStateTime( 0 )
	, m_IsRunning( false )
	, m_Name( "" )
	, m_Owner( nullptr )
{
	m_States.Empty( );
}

FSMachine::~FSMachine( ) {
	m_States.Empty( );
}



void FSMachine::AddState( IFsmStateInterface* state )
{
	if (state == nullptr) return;
	FName typeName = state->GetType( );
	if (m_States.Contains( typeName ))
		return;
	state->Init( this );
	m_States.Add( typeName, state );
}

void FSMachine::ChangeState( IFsmStateInterface* state, FsmStateDataBase data )
{
	if (state == nullptr) return;
	if (m_pCurrentState != nullptr)
		m_pCurrentState->Exit( this );
	state->Enter( this, data );
	m_pCurrentState = state;
}

void FSMachine::DestroyFsm( )
{
	if (m_pCurrentState != nullptr)
		m_pCurrentState->Exit( this );
	m_pCurrentState = nullptr;

	for (auto item : m_States) {
		item.Value->Destroy( this );
	}
	m_States.Empty( );
	m_IsRunning = false;
	m_Name = "";
	m_Owner = nullptr;
	m_CurrentStateTime = 0;
}

void FSMachine::InitFsm( AActor* owner, FName fsmName, TArray<IFsmStateInterface*>& states )
{
	m_CurrentStateTime = 0;
	m_pCurrentState = nullptr;
	m_Name = fsmName;
	m_Owner = owner;
	if (states.Num( ) > 0) {
		for (int i = 0; i < states.Num( ); i++) {
			AddState( states[i] );
		}
	}
	m_IsRunning = true;
}
void FSMachine::PauseFsm( )
{
	if (m_pCurrentState != nullptr)
		m_pCurrentState->Pause( this );
	m_IsRunning = false;
}

void FSMachine::ResetFsm( )
{
	if (m_pCurrentState != nullptr)
		m_pCurrentState->Reset( this );
	m_IsRunning = true;
	m_CurrentStateTime = 0;
	m_pCurrentState = nullptr;
}
void FSMachine::ResumeFsm( )
{
	if (m_pCurrentState != nullptr)
		m_pCurrentState->Resume( this );
	m_IsRunning = true;
}
void FSMachine::Update( float frameTime, float realTime )
{
	if (!m_IsRunning) return;
	if (m_pCurrentState != nullptr)
		m_pCurrentState->Update( frameTime, realTime );
	m_CurrentStateTime += realTime;
}

void FSMachine::GetAllStateType( TArray<IFsmStateInterface*>& list )
{
	if (list.Num( ) <= 0) return;
	for (auto item : m_States) {
		list.Add( item.Value );
	}
}
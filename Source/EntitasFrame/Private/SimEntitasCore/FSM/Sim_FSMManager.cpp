
#include "FSM\Sim_FSMManager.h"
#include "FSM\Sim_FSM.h"

FsmManager::FsmManager( ) {
	m_Fsms.Empty( );
}

FsmManager::~FsmManager( ) {

}

IFSMInterface* FsmManager::CreateFsm( AActor* owner, FName fsmName, TArray<IFsmStateInterface*>& states )
{
	if (!fsmName.IsValid()) return nullptr;
	auto fsm = m_Fsms.Find( fsmName );
	if (fsm) {
		return *fsm;
	}
	FSMachine* pFsm = new FSMachine( );
	pFsm->InitFsm( owner, fsmName, states );
	m_Fsms.Add( fsmName, pFsm );
	return pFsm;
}

void FsmManager::DestroyFsm( FName fsmName )
{
	if (!fsmName.IsValid()) return;
	auto fsm =  m_Fsms.Find( fsmName );
	if (!fsm) {
		
		(*fsm)->DestroyFsm( );
		m_Fsms.Remove( fsmName );
	}
}

void FsmManager::GetAllFsm(  TArray<IFSMInterface*>& list )
{
	if (m_Fsms.Num()<= 0) return;
	for( auto item : m_Fsms )
	{
		list.Add( item.Value );
	}
}

IFSMInterface* FsmManager::GetFsm( FName fsmName )
{
	if (!fsmName.IsValid( )) return nullptr;
	if (auto fsm = m_Fsms.Find( fsmName )) {
		return  *fsm;
	}

	return nullptr;
}

bool FsmManager::HasFsm( FName fsmName )
{
	if (!fsmName.IsValid()) return false;
	return m_Fsms.Contains( fsmName );
}


void FsmManager::Init( ) {

}

/// <summary>
/// 销毁
/// </summary>
void FsmManager::Destroy( ) {

}

/// <summary>
/// 更新函数
/// </summary>
/// <param name="frameTime">逻辑一帧时间</param>
/// <param name="realTime">实际一帧时间</param>
void FsmManager::Update( float frameTime, float realTime ) {

}

/// <summary>
/// 重置接口
/// </summary>
void FsmManager::Reset( ) {

}
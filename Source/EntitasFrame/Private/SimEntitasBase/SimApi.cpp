///-------------------------------------------------------------------------------------------------
/// File:	src\API.cpp.
///
/// Summary:	Implements the API.


#include "SimApi.h"
#include "Memory/ECSMM.h"
#include "SimEcs_Engine.h"

void CheckMemoryLeaks( ) {
	// check for memory leaks
	ECSMemoryManager->CheckMemoryLeaks( );
}
// namespace SimEcs




MemoryManager*				ECSMemoryManager = new MemoryManager( );



GlobalMemoryUser::GlobalMemoryUser( ) : ECS_MEMORY_MANAGER( ECSMemoryManager )
{}

GlobalMemoryUser::~GlobalMemoryUser( )
{}

const void* GlobalMemoryUser::Allocatex( size_t memSize, const char* user )
{
	return ECS_MEMORY_MANAGER->Allocate( memSize, user );
}

void GlobalMemoryUser::Free( void* pMem )
{
	ECS_MEMORY_MANAGER->Free( pMem );
	CheckMemoryLeaks( );
}

/*
	Author : magi
	Date   : 22nd October, 2019
	File   : StackAllocator.h

	Stack allocator.

	All Rights Reserved. (c) Copyright 2019.
*/

#ifndef __STACK_ALLOC_H__
#define __STACK_ALLOC_H__

#include "Memory/Allocator/IAllocator.h"
//#include "StackAllocator.generated.h"


	/*
	*/
class   StackAllocator : public IAllocator {
private:

	struct AllocMetaInfo {
		u8 adjustment;
	};

public:

	StackAllocator( size_t memSize, const void* mem );

	virtual ~StackAllocator( );

	virtual void* allocate( size_t size, u8 alignment ) override;
	virtual void free( void* p ) override;
	virtual void clear( ) override;

}; // class StackAllocator

// namespace SimEcs::Allocator

#endif // __STACK_ALLOC_H__
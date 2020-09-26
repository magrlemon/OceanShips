/*
	Author : magi
	Date   : 8th October, 2019
	File   : CountByType.h
	
	A static counter that increments thee count for a specific type.

	All Rights Reserved. (c) Copyright 2019.
*/

#ifndef __SIMECS_FAMILY_TYPE_ID_H__
#define __SIMECS_FAMILY_TYPE_ID_H__

#include "CoreMinimal.h"
#include "SimEcs_Platform.h"

	template<class T>
	class   SimEcs_FamilyTypeID
	{
		static TypeID s_sim_count;
	
	public:
	
		template<class U>
		static const TypeID Get()
		{
			static const TypeID STATIC_TYPE_ID { U::STATIC_ENTITY_TYPE_ID };
			return STATIC_TYPE_ID;
		}

		static const TypeID Get()
		{
			return s_sim_count;
		}
	};	

#endif // __SIMECS_FAMILY_TYPE_ID_H__

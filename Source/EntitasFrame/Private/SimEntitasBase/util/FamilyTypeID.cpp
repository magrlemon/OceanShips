/*
	Author : magi
	Date   : 3rd September, 2019
	File   : FamilyTypeID.h

	A static counter that increments thee count for a specific type.

	All Rights Reserved. (c) Copyright 2019 - 2019.
*/

#include "util/FamilyTypeID.h"
#include "SimEcs_Platform.h"
#include "SimEcs_ComponentWrapper.h"
class SimEcs_IEntity;

class IComponentWrapper;

u64 SimEcs_FamilyTypeID<SimEcs_IEntity>::s_sim_count = 0u;
u64 SimEcs_FamilyTypeID<IComponentWrapper>::s_sim_count = 0u;

template class SimEcs_FamilyTypeID<SimEcs_IEntity>;
template class SimEcs_FamilyTypeID<IComponentWrapper>;




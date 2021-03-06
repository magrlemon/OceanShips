///-------------------------------------------------------------------------------------------------
/// File:	include\Platform.h.
///
/// Summary:	Declares the platform specifics.

#pragma once

#ifndef __SIMECS_PLATFORM_H__
#define __SIMECS_PLATFORM_H__


// Check if using 64-Bit architecture
#if (defined(Win64) || defined(__x86_64__) || defined(__ppc64__) || defined(_M_AMD64) || defined(_M_ARM64) || defined(_M_X64))
#define ECS_64BIT 1

// Check if using 32-Bit architecture
#elif (defined(WIN32) && !defined(Win64)) || defined(_M_IX86)
#define ECS_32BIT 1
#endif

// Platform includes
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <limits>

#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <algorithm>
#include <functional>

#include <assert.h>
#include <memory>

#include <string>
#include <math.h>

#include <chrono>

#include "SimEcs_BattleComponentsStructData.h"

	// signed integer type
	using i8			= int8_t;
	using i16			= int16_t;
	using i32			= int32_t;

#ifdef ECS_64BIT
	using i64			= int64_t;
#else
	using i64			= int32_t;
#endif

	// unsigned integer type
	using u8			= uint8_t;
	using u16			= uint16_t;
	using u32			= uint32_t;
#ifdef ECS_64BIT
	using u64			= uint64_t;
#else
	using u64			= uint32_t;
#endif

	// floating point
	using f32			= float_t;
	using f64			= double_t;

	// pointer
	using iptr			= intptr_t;
	using uptr			= uintptr_t;

	using ObjectID		= size_t;
	using TypeID		= size_t;

	static const ObjectID	INVALID_OBJECT_ID	= std::numeric_limits<ObjectID>::max();
	static const TypeID		INVALID_TYPE_ID		= std::numeric_limits<TypeID>::max();


	using ComponentId = ObjectID;
	using ComponentTypeId = TypeID;
	using EntityTypeId = ObjectID;



	union TimeStamp
	{
		f32 asFloat;
		u32 asUInt;

		TimeStamp() : asUInt(0U)
		{}

		TimeStamp(f32 floatValue) : asFloat(floatValue)
		{}

		operator u32() const { return this->asUInt; }

		inline const bool operator==(const TimeStamp& other) const { return this->asUInt == other.asUInt; }
		inline const bool operator!=(const TimeStamp& other) const { return this->asUInt != other.asUInt; }

		inline const bool operator<(const TimeStamp& other) const { return this->asFloat < other.asFloat; }
		inline const bool operator>(const TimeStamp& other) const { return this->asFloat > other.asFloat; }

	}; // union TimeStamp


#endif // __SIMECS_PLATFORM_H__
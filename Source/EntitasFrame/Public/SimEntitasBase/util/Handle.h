///-------------------------------------------------------------------------------------------------
/// File:	include\util\SimHandle.h.
///
/// Summary:	Declares the handle class.
///-------------------------------------------------------------------------------------------------

#ifndef __HANDLE_H__
#define __HANDLE_H__

#pragma warning(push)

// warning C4293: '<<': shift count negative or too big, undefined behavior
// note we are using a static_assert to ensure this won't be the case.
#pragma warning(disable: 4293)


#include "SimEcs_Platform.h"

namespace util {

	namespace Internal {

		///-------------------------------------------------------------------------------------------------
		/// Union:	SimHandle
		///
		/// Summary:	Declares a base handle template class. This template can be specialized in arbitrary 
		/// ways what allows a certain amount of max. possible unique handle versions with a certain amount 
		/// of different versions.
		///
		/// Author:	magi
		///
		/// Date:	2/10/2019
		///-------------------------------------------------------------------------------------------------

		template<
			typename handle_value_type,
			size_t version_bits,
			size_t index_bits
		>
			union SimHandle {
			static_assert(sizeof( handle_value_type ) * CHAR_BIT >= (version_bits + index_bits), "Invalid handle layout. More bits used than base value type can hold!");

			using value_type = handle_value_type;

			static constexpr size_t			NUM_VERSION_BITS{ version_bits };
			static constexpr size_t			NUM_INDEX_BITS{ index_bits };


			static constexpr value_type		MIN_VERISON{ 0 };
			static constexpr value_type		MAX_VERSION{ (1U << NUM_VERSION_BITS) - 2U };
			static constexpr value_type		MAX_INDICES{ (1U << NUM_INDEX_BITS) - 2U };

			static constexpr value_type		INVALID_HANDLE{ std::numeric_limits<value_type>::max( ) };

			private:

				value_type						value;

			public:

				struct {
					value_type					index : NUM_INDEX_BITS;
					value_type					version : NUM_VERSION_BITS;
				};


				SimHandle( )
				{}

				SimHandle( value_type value ) :
					value( value )
				{}

				SimHandle( value_type index, value_type version ) :
					index( index ),
					version( version )
				{}

				inline operator value_type( ) const { return value; }
		};
	} // namespace Internal


		/// Summary:	Defines a 32 bit handle
		/// Max. possible handles							: 1048576
		/// Max. possible versions until loop per handle	: 4096
	using SimHandle32 = Internal::SimHandle<u32, 12, 20>;


	/// Summary:	Defines a 64 bit handle
	/// Max. possible handles							: 1099511627776
	/// Max. possible versions until loop per handle	: 16777216
#ifdef ECS_64BIT
	using SimHandle64 = Internal::SimHandle<u64, 24, 40>;
#else
	using SimHandle64 = SimHandle32;
#endif 


	template<class T, class handle_type, size_t grow = 1024>
	class SimHandleTable {
		using SimHandle = handle_type;

	private:

		using TableEntry = std::pair<typename SimHandle::value_type, T*>;

		std::vector<TableEntry> m_Table;


		void GrowTable( )
		{
			size_t oldSize = this->m_Table.size( );

			assert( oldSize < SimHandle::MAX_INDICES && "Max table capacity reached!" );

			size_t newSize = std::min( oldSize + grow, SimHandle::MAX_INDICES );

			this->m_Table.resize( newSize );

			for (typename SimHandle::value_type i = oldSize; i < newSize; ++i)
				this->m_Table[i] = TableEntry( SimHandle::MIN_VERISON, nullptr );
		}

	public:

		SimHandleTable( )
		{
			this->GrowTable( );
		}

		~SimHandleTable( )
		{}

		SimHandle AqcuireSimHandle( T* rawObject )
		{
			typename SimHandle::value_type i = 0;
			for (; i < this->m_Table.size( ); ++i) {
				if (this->m_Table[i].second == nullptr) {
					this->m_Table[i].second = rawObject;

					this->m_Table[i].first = ((this->m_Table[i].first + 1) > SimHandle::MAX_VERSION) ? SimHandle::MIN_VERISON : this->m_Table[i].first + 1;

					return SimHandle( i, this->m_Table[i].first );
				}
			}

			// grow table
			this->GrowTable( );

			this->m_Table[i].first = 1;
			this->m_Table[i].second = rawObject;

			return SimHandle( i, this->m_Table[i].first );
		}

		void ReleaseSimHandle( SimHandle handle )
		{
			assert( (handle.index < this->m_Table.size( ) && handle.version == this->m_Table[handle.index].first) && "Invalid handle!" );
			this->m_Table[handle.index].second = nullptr;
		}

		///-------------------------------------------------------------------------------------------------
		/// Fn:	inline bool SimHandleTable::IsValidSimHandle(const SimHandle& handle) const
		///
		/// Summary:	Check if a 'handle' is expired, that is, its intern version is different from actual
		/// version stored in table.
		///
		/// Author:	magi
		///
		/// Date:	1/10/2019
		///
		/// Parameters:
		/// handle - 	The handle.
		///
		/// Returns:	True if valid handle, false if not.
		///-------------------------------------------------------------------------------------------------

		inline bool IsExpired( SimHandle handle ) const
		{
			return this->m_Table[handle.index].first != handle.version;
		}

		///-------------------------------------------------------------------------------------------------
		/// Fn:	SimHandle SimHandleTable::operator[](HANDLE_VALUE_TYPE i) const
		///
		/// Summary:	Returns handle from table at position described by i.
		///
		/// Author:	magi
		///
		/// Date:	1/10/2019
		///
		/// Parameters:
		/// i - 	Zero-based index of the.
		///
		/// Returns:	The indexed value.
		///-------------------------------------------------------------------------------------------------

		inline SimHandle operator[]( typename SimHandle::value_type index ) const
		{
			assert( index < this->m_Table.size( ) && "Invalid handle!" );
			return SimHandle( index, this->m_Table[index].first );
		}

		///-------------------------------------------------------------------------------------------------
		/// Fn:	inline T* SimHandleTable::operator[](SimHandle handle)
		///
		/// Summary:	Returns the raw object stored for a handle.
		///
		/// Author:	magi
		///
		/// Date:	2/10/2019
		///
		/// Parameters:
		/// handle - 	SimHandle of the handle.
		///
		/// Returns:	The indexed value.
		///-------------------------------------------------------------------------------------------------

		inline T* operator[]( SimHandle handle )
		{
			assert( (handle.index < this->m_Table.size( ) && handle.version == this->m_Table[handle.index].first) && "Invalid handle!" );
			return (this->m_Table[handle.index].first == handle.version ? this->m_Table[handle.index].second : nullptr);
		}

	}; // class SimHandleTable	

}

#pragma warning(pop)

#endif // ! __HANDLE_H__

#pragma once
#ifndef utiReferenceCounted_inl__
#define utiReferenceCounted_inl__

namespace uti
{
	//////////////////////////////////////////////////////////////////////////
	// Reference Counted Implementation
	//////////////////////////////////////////////////////////////////////////

	template< typename T, typename Allocator >
	bool ReferenceCounted<T, Allocator>::operator!=( const ReferenceCounted< T, Allocator >& rhs ) const
	{
		return m_CountedPointer != rhs.m_CountedPointer;
	}

	template< typename T, typename Allocator >
	bool ReferenceCounted<T, Allocator>::operator==( const ReferenceCounted< T, Allocator >& rhs ) const
	{
		return m_CountedPointer == rhs.m_CountedPointer;
	}

	template< typename T, typename Allocator >
	const T& ReferenceCounted<T, Allocator>::operator[]( u32 idx ) const
	{
		return m_CountedPointer[ idx ];
	}

	template< typename T, typename Allocator >
	bool ReferenceCounted< T, Allocator >::Null( void ) const
	{
		return m_CountedPointer == nullptr;
	}

	template< typename T, typename Allocator >
	u32 ReferenceCounted< T, Allocator>::Count( void ) const
	{
		if( m_Count != nullptr )
		{
			return *m_Count;
		}
		else
		{
			return 0;
		}
	}

	template< typename T, typename Allocator >
	bool ReferenceCounted< T, Allocator >::Valid( void ) const
	{
		return m_CountedPointer != nullptr;
	}

	template< typename T, typename Allocator >
	void ReferenceCounted< T, Allocator >::SetNull( void )
	{
		DecRef();
		m_CountedPointer = nullptr;
		m_Count = nullptr;
	}

	template< typename T, typename Allocator >
	T& ReferenceCounted< T, Allocator >::operator[]( u32 idx )
	{
		return m_CountedPointer[ idx ];
	}

	template< typename T, typename Allocator >
	T* ReferenceCounted< T, Allocator >::Ptr( void ) const
	{
		return m_CountedPointer;
	}

	template< typename T, typename Allocator >
	ReferenceCounted< T, Allocator >::ReferenceCounted( void ) :
		m_CountedPointer( nullptr )
	{
		m_Count = static_cast< u32* >( m_Alloc.AllocateBytes( sizeof( u32 ) ) );
		*m_Count = 1U;
	}

	template< typename T, typename Allocator >
	ReferenceCounted< T, Allocator >::ReferenceCounted( ReferenceCounted< T, Allocator >&& refCount ) :
		m_CountedPointer( refCount.m_CountedPointer ),
		m_Count( refCount.m_Count )
	{
		refCount.m_CountedPointer = nullptr;
		m_Count = nullptr;
	}

	template< typename T, typename Allocator >
	ReferenceCounted< T, Allocator >::~ReferenceCounted()
	{
		DecRef();
	}

	template< typename T, typename Allocator >
	ReferenceCounted< T, Allocator >::ReferenceCounted( const ReferenceCounted< T, Allocator >& refCount )
	{

		m_Count = refCount.m_Count;
		m_CountedPointer = refCount.m_CountedPointer;

		IncRef();
	}

	template< typename T, typename Allocator >
	ReferenceCounted< T, Allocator >::ReferenceCounted( T* pointer ) :
		m_CountedPointer( pointer )
	{
		m_Count = static_cast< u32* >( m_Alloc.AllocateBytes( sizeof( u32 ) ) );
		*m_Count = 1U;
	}

	template< typename T, typename Allocator >
	T* ReferenceCounted< T, Allocator >::operator*( )
	{
		return m_CountedPointer;
	}

	template< typename T, typename Allocator >
	T* ReferenceCounted< T, Allocator >::operator->( )
	{
		return m_CountedPointer;
	}

	template< typename T, typename Allocator >
	ReferenceCounted< T, Allocator >& ReferenceCounted< T, Allocator >::operator=( ReferenceCounted< T, Allocator >&& refCount )
	{
		if( this != &refCount )
		{
			m_CountedPointer = refCount.m_CountedPointer;
			m_Count = refCount.m_Count;

			refCount.m_CountedPointer = nullptr;
			refCount.m_Count = nullptr;
		}

		return *this;

	}

	template< typename T, typename Allocator >
	ReferenceCounted< T, Allocator >& ReferenceCounted< T, Allocator >::operator=( const ReferenceCounted< T, Allocator >& refCount )
	{
		if( m_Count != refCount.m_Count )
		{
			m_CountedPointer = refCount.m_CountedPointer;

			m_Count = refCount.m_Count;

			IncRef();
		}

		return *this;
	}

	template< typename T, typename Allocator >
	void ReferenceCounted< T, Allocator >::DecRef( void )
	{
		if( ( m_Count != nullptr ) && ( *m_Count ) > 1 )
		{
			--( *m_Count );
		}
		else
		{
			m_Alloc.FreeBytes( m_CountedPointer );
			m_CountedPointer = nullptr;
			m_Alloc.FreeBytes( m_Count );
			m_Count = nullptr;
		}
	}

	template< typename T, typename Allocator >
	void ReferenceCounted< T, Allocator >::IncRef( void )
	{
		printf( "Inc" );
		if( ( *m_Count ) > 0U )
		{
			++( *m_Count );
		}
	}
}


#endif // utiReferenceCounted_inl__

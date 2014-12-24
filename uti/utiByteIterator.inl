#pragma once
#ifndef utiByteIterator_inl__
#define utiByteIterator_inl__

namespace uti
{
	//////////////////////////////////////////////////////////////////////////
	// UTF String Iterator implementation
	//////////////////////////////////////////////////////////////////////////

	template< typename String >
	UTFByteIterator< String > UTFByteIterator< String >::operator--( int )
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if( m_uiPos > 0 )
		{
			UTFByteIterator it( *this );
			--*this;
			return it;
		}
		else
		{
			throw UTFException( "Iterator not decrementable", 0 );
		}
#else

		UTFByteIterator it( *this );
		--*this;
		return it;
#endif // _ITERATOR_DEBUG_LEVEL == 2
	}

	template< typename String >
	UTFByteIterator< String >& UTFByteIterator< String >::operator--( void )
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if( m_uiPos > 0 )
		{
			--m_uiPos;
			return *this;
		}
		else
		{
			throw UTFException( "Iterator not decrementable", 0 );
		}
#else

		--m_uiPos;
		return *this;
#endif // _ITERATOR_DEBUG_LEVEL == 2

	}

	template< typename String >
	UTFByteIterator< String >& UTFByteIterator< String >::operator=( const UTFByteIterator< String >& it )
	{
		m_String = it.m_String;
		m_uiPos = it.m_uiPos;
	}

	template< typename String >
	UTFByteIterator< String > UTFByteIterator< String >::operator++( int )
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if( m_uiPos < m_String.m_uiSize )
		{
			UTFByteIterator it( *this );
			++*this;
			return it;
		}
		else
		{
			throw UTFException( "Iterator not incrementable", 0 );
		}
#else

		UTFByteIterator it( *this );
		++*this;
		return it;
#endif // _ITERATOR_DEBUG_LEVEL == 2
	}

	template< typename String >
	UTFByteIterator< String >& UTFByteIterator< String >::operator++( void )
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if( m_uiPos < m_String.m_uiSize )
		{
			++m_uiPos;
			return *this;
		}
		else
		{
			throw UTFException( "Iterator not incrementable", 0 );
		}
#else

		++m_uiPos;
		return *this;
#endif // _ITERATOR_DEBUG_LEVEL == 2
	}

	template< typename String >
	bool UTFByteIterator< String >::operator>( const UTFByteIterator< String >& rhs ) const
	{

#if _ITERATOR_DEBUG_LEVEL == 2
		if( m_String.m_pData != rhs.m_String.m_pData )
		{
			throw UTFException( "UTFIterator mismatch on > comparison", 0 );
		}
		else
		{
			return m_uiPos > rhs.m_uiPos;
		}
#else
		return m_uiPos > rhs.m_uiPos;
#endif // _ITERATOR_DEBUG_LEVEL == 2

	}

	template< typename String >
	bool UTFByteIterator< String >::operator>=( const UTFByteIterator< String >& rhs ) const
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if( m_String.m_pData != rhs.m_String.m_pData )
		{
			throw UTFException( "UTFIterator mismatch on >= comparison", 0 );
		}
		else
		{
			return m_uiPos >= rhs.m_uiPos;
		}
#else
		return m_uiPos >= rhs.m_uiPos;
#endif // _ITERATOR_DEBUG_LEVEL == 2
	}

	template< typename String >
	bool UTFByteIterator< String >::operator<( const UTFByteIterator< String >& rhs ) const
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if( m_String.m_pData != rhs.m_String.m_pData )
		{
			throw UTFException( "UTFIterator mismatch on < comparison", 0 );
		}
		else
		{
			return m_uiPos < rhs.m_uiPos;
		}
#else
		return m_uiPos < rhs.m_uiPos;
#endif // _ITERATOR_DEBUG_LEVEL == 2
	}

	template< typename String >
	bool UTFByteIterator< String >::operator<=( const UTFByteIterator< String >& rhs ) const
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if( m_String.m_pData != rhs.m_String.m_pData )
		{
			throw UTFException( "UTFIterator mismatch on <= comparison", 0 );
		}
		else
		{
			return m_uiPos <= rhs.m_uiPos;
		}
#else
		return m_uiPos <= rhs.m_uiPos;
#endif // _ITERATOR_DEBUG_LEVEL == 2
	}

	template< typename String >
	bool UTFByteIterator< String >::operator!=( const UTFByteIterator< String >& rhs ) const
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if( m_String.m_pData != rhs.m_String.m_pData )
		{
			throw UTFException( "UTFIterator mismatch on != comparison", 0 );
		}
		else
		{
			return m_uiPos != rhs.m_uiPos;
		}
#else
		return m_uiPos != rhs.m_uiPos;
#endif // _ITERATOR_DEBUG_LEVEL == 2
	}

	template< typename String >
	bool UTFByteIterator< String >::operator==( const UTFByteIterator< String >& rhs ) const
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if( m_String.m_pData != rhs.m_String.m_pData )
		{
			throw UTFException( "UTFIterator mismatch on == comparison", 0 );
		}
		else
		{
			return m_uiPos == rhs.m_uiPos;
		}
#else
		return m_uiPos == rhs.m_uiPos;
#endif // _ITERATOR_DEBUG_LEVEL == 2
	}

	template< typename String >
	typename UTFByteIterator< String >::DataType& UTFByteIterator< String >::operator*( )
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if( Valid() )
		{
			return *( m_String.m_pData.Ptr() + m_uiPos );
		}
		else
		{
			throw UTFException( "Iterator not dereferenceable", 0 );
		}
#else
		return *( m_String.m_pData.Ptr() + m_uiPos );
#endif

	}

	template< typename String >
	bool UTFByteIterator< String >::Valid( void ) const
	{
		return m_uiPos < m_String.m_uiSize;
	}

	template< typename String >
	UTFByteIterator< String >::~UTFByteIterator()
	{
		m_uiPos = 0U;
	}

	template< typename String >
	UTFByteIterator< String >::UTFByteIterator( typename UTFByteIterator< String >::String& data, u32 uiPos ) :
		m_String( data ),
		m_uiPos( uiPos )
	{
	}

	template< typename String >
	UTFByteIterator< String >::UTFByteIterator( const UTFByteIterator< String >& it ) :
		m_String( it.m_String ),
		m_uiPos( it.m_uiPos )
	{

	}
}
#endif // utiByteIterator_inl__

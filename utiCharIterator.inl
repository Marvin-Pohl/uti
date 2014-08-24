#pragma once
#ifndef utiCharIterator_inl__
#define utiCharIterator_inl__

namespace uti
{
	//////////////////////////////////////////////////////////////////////////
	// UTF String Char Iterator implementation
	//////////////////////////////////////////////////////////////////////////

	template< typename String >
	UTFCharIterator< String > UTFCharIterator< String >::operator--( int )
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if( m_uiPos > 0 )
		{
			UTFCharIterator it( *this );
			--*this;
			return it;
		}
		else
		{
			throw UTFException( "Iterator not decrementable", 0 );
		}
#else

		UTFCharIterator it( *this );
		--*this;
		return it;
#endif // _ITERATOR_DEBUG_LEVEL == 2
	}

	template< typename String >
	UTFCharIterator< String >& UTFCharIterator< String >::operator--( void )
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if( m_uiPos > 0 )
		{
			u32 size = 0;
			while( m_uiPos > 0 && size == 0 )
			{
				size = String::CharSize( m_String.m_pData.Ptr() + m_uiPos );
				--m_uiPos;
			}
			return *this;
		}
		else
		{
			throw UTFException( "Iterator not decrementable", 0 );
		}
#else

		u32 size = 0;
		while( m_uiPos > 0 && size == 0 )
		{
			size = String::CharSize( m_String.m_pData.Ptr() + m_uiPos );
			--m_uiPos;
		}
		return *this;
#endif // _ITERATOR_DEBUG_LEVEL == 2

	}

	template< typename String >
	UTFCharIterator< String >& UTFCharIterator< String >::operator=( const UTFCharIterator< String >& it )
	{
		m_String = it.m_String;
		m_uiPos = it.m_uiPos;
	}

	template< typename String >
	UTFCharIterator< String > UTFCharIterator< String >::operator++( int )
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if( m_uiPos < m_String.m_uiSize )
		{
			UTFCharIterator it( *this );
			++*this;
			return it;
		}
		else
		{
			throw UTFException( "Iterator not incrementable", 0 );
		}
#else

		UTFCharIterator it( *this );
		++*this;
		return it;
#endif // _ITERATOR_DEBUG_LEVEL == 2
	}

	template< typename String >
	UTFCharIterator< String >& UTFCharIterator< String >::operator++( void )
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if( m_uiPos < m_String.m_uiSize )
		{
			u32 size = String::CharSize( m_String.m_pData.Ptr() + m_uiPos );
			if( size != 0 )
			{
				m_uiPos += size;
			}
			else
			{
				while( size == 0 && m_uiPos < m_String.m_uiSize )
				{
					size = String::CharSize( m_String.m_pData.Ptr() + m_uiPos );
					++m_uiPos;
				}
			}

			return *this;
		}
		else
		{
			throw UTFException( "Iterator not incrementable", 0 );
		}
#else

		u32 size = String::CharSize( m_String.m_pData.Ptr() + m_uiPos );
		if( size != 0 )
		{
			m_uiPos += size;
		}
		else
		{
			do
			{
				size = String::CharSize( m_String.m_pData.Ptr() + m_uiPos );
				++m_uiPos;
			} while( size == 0 && m_uiPos < m_String.m_uiSize );
		}

		return *this;
#endif // _ITERATOR_DEBUG_LEVEL == 2
	}

	template< typename String >
	bool UTFCharIterator< String >::operator>( const UTFCharIterator< String >& rhs ) const
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
	bool UTFCharIterator< String >::operator>=( const UTFCharIterator< String >& rhs ) const
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
	bool UTFCharIterator< String >::operator<( const UTFCharIterator< String >& rhs ) const
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
	bool UTFCharIterator< String >::operator<=( const UTFCharIterator< String >& rhs ) const
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
	bool UTFCharIterator< String >::operator!=( const UTFCharIterator< String >& rhs ) const
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
	bool UTFCharIterator< String >::operator==( const UTFCharIterator< String >& rhs ) const
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
	typename UTFCharIterator< String >::DataType* UTFCharIterator< String >::operator*( )
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if( Valid() )
		{
			return ( m_String.m_pData.Ptr() + m_uiPos );
		}
		else
		{
			throw UTFException( "Iterator not dereferenceable", 0 );
		}
#else
		return ( m_String.m_pData.Ptr() + m_uiPos );
#endif

	}

	template< typename String >
	bool UTFCharIterator< String >::Valid( void ) const
	{
		return m_uiPos < m_String.m_uiSize;
	}

	template< typename String >
	UTFCharIterator< String >::~UTFCharIterator()
	{
		m_uiPos = 0U;
	}

	template< typename String >
	UTFCharIterator< String >::UTFCharIterator( typename UTFCharIterator< String >::String& data, u32 uiPos ) :
		m_String( data ),
		m_uiPos( uiPos )
	{
	}

	template< typename String >
	UTFCharIterator< String >::UTFCharIterator( const UTFCharIterator< String >& it ) :
		m_String( it.m_String ),
		m_uiPos( it.m_uiPos )
	{

	}
}
#endif // utiCharIterator_inl__

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
			UTI_FATAL( "Iterator not decrementable" );
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
			UTI_FATAL( "Iterator not decrementable" );
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
			UTI_FATAL( "Iterator not incrementable" );
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
					++m_uiPos;
					size = String::CharSize( m_String.m_pData.Ptr() + m_uiPos );
				}
			}

			return *this;
		}
		else
		{
			UTI_FATAL( "Iterator not incrementable" );
			return *this;
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
				++m_uiPos;
				size = String::CharSize( m_String.m_pData.Ptr() + m_uiPos );
			} while( size == 0 && m_uiPos < m_String.m_uiSize );
		}

		return *this;
#endif // _ITERATOR_DEBUG_LEVEL == 2
	}

	template< typename StringType >
	inline
	UTFCharIterator<StringType>& UTFCharIterator<StringType>::operator+=( s32 offset )
	{
		if (offset < 0)
		{
			offset = -offset;
			while( offset > 0 )
			{
				--this;
			}
		}
		else
		{
			while( offset > 0 )
			{
				++this;
			}
		}
	}

	template< typename StringType >
	inline
	UTFCharIterator<StringType>& uti::UTFCharIterator<StringType>::operator-=( s32 offset )
	{
		return (( *this ) += -offset);
	}



	template< typename String >
	bool UTFCharIterator< String >::operator>( const UTFCharIterator< String >& rhs ) const
	{

#if _ITERATOR_DEBUG_LEVEL == 2
		if( m_String.m_pData != rhs.m_String.m_pData )
		{
			UTI_FATAL( "UTFIterator mismatch on > comparison" );
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
			UTI_FATAL( "UTFIterator mismatch on >= comparison" );
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
			UTI_FATAL( "UTFIterator mismatch on < comparison" );
			return false;
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
			UTI_FATAL( "UTFIterator mismatch on <= comparison" );
			return false;
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
			UTI_FATAL( "UTFIterator mismatch on != comparison" );
			return false;
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
			UTI_FATAL( "UTFIterator mismatch on == comparison" );
			return false;
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
	typename UTFCharIterator< String >::DataType* UTFCharIterator< String >::operator*( ) const
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if( Valid() )
		{
			return ( m_String.m_pData.Ptr() + m_uiPos );
		}
		else
		{
			UTI_FATAL( "Iterator not dereferenceable" );
			return nullptr;
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

#pragma once
#ifndef utiReverseIterator_inl__
#define utiReverseIterator_inl__


namespace uti
{
	//////////////////////////////////////////////////////////////////////////
	// Reverse Iterator Implementation
	//////////////////////////////////////////////////////////////////////////

	template< typename Iterator >
	ReverseIterator_tpl< Iterator > ReverseIterator_tpl<Iterator>::operator--( int )
	{
		ReverseIterator_tpl< Iterator > copy( *this );
		--*this;
		return copy;
	}

	template< typename Iterator >
	ReverseIterator_tpl< Iterator >& ReverseIterator_tpl<Iterator>::operator--( void )
	{
		++m_It;
		return *this;
	}

	template< typename Iterator >
	ReverseIterator_tpl< Iterator > ReverseIterator_tpl<Iterator>::operator++( int )
	{

		ReverseIterator_tpl< Iterator > copy( *this );
		++*this;
		return copy;
	}

	template< typename Iterator >
	ReverseIterator_tpl< Iterator >& ReverseIterator_tpl<Iterator>::operator++( void )
	{
		--m_It;
		return *this;
	}

	template< typename Iterator >
	bool ReverseIterator_tpl<Iterator>::operator>( const ReverseIterator_tpl< Iterator >& rhs ) const
	{
		return m_It < rhs.m_It;
	}

	template< typename Iterator >
	bool ReverseIterator_tpl<Iterator>::operator>=( const ReverseIterator_tpl< Iterator >& rhs ) const
	{
		return m_It <= rhs.m_It;
	}

	template< typename Iterator >
	bool ReverseIterator_tpl<Iterator>::operator<( const ReverseIterator_tpl< Iterator >& rhs ) const
	{
		return m_It > rhs.m_It;
	}

	template< typename Iterator >
	bool ReverseIterator_tpl<Iterator>::operator<=( const ReverseIterator_tpl< Iterator >& rhs ) const
	{
		return m_It >= ths.m_It;
	}

	template< typename Iterator >
	bool ReverseIterator_tpl<Iterator>::operator!=( const ReverseIterator_tpl< Iterator >& rhs ) const
	{
		return m_It != rhs.m_It;
	}

	template< typename Iterator >
	bool ReverseIterator_tpl<Iterator>::operator==( const ReverseIterator_tpl< Iterator >& rhs ) const
	{
		return m_It == rhs.m_It;
	}

	template< typename Iterator >
	bool ReverseIterator_tpl<Iterator>::Valid( void ) const
	{
		return m_It.Valid();
	}

	template< typename Iterator >
	typename Iterator::DataType& ReverseIterator_tpl<Iterator>::operator*( )
	{
		Iterator copy( m_It );
		return *( --copy );
	}

	template< typename Iterator >
	ReverseIterator_tpl< Iterator >& ReverseIterator_tpl<Iterator>::operator=( const Iterator& it )
	{
		m_It = it;
		return *this;
	}

	template< typename Iterator >
	ReverseIterator_tpl< Iterator >& ReverseIterator_tpl<Iterator>::operator=( const ReverseIterator_tpl< Iterator >& it )
	{
		m_It = it.m_It;
		return *this;
	}

	template< typename Iterator >
	ReverseIterator_tpl<Iterator>::ReverseIterator_tpl( const ReverseIterator_tpl< Iterator >& it ) :
		m_It( it.m_It )
	{

	}

	template< typename Iterator >
	ReverseIterator_tpl<Iterator>::ReverseIterator_tpl( const Iterator& it ) :
		m_It( it )
	{

	}
}

#endif // utiReverseIterator_inl__

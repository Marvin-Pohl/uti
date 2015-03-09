
template < class StringType >
inline
uti::UTFChar<StringType>::UTFChar( ConstTypePtr address, u32 size /*= 0 */ ) :
m_Address( address ),
m_Size( size )
{
	if( size == 0U )
	{
		m_Size = StringType::CharSize( m_Address );
	}
}

template < class StringType >
inline
typename uti::UTFChar<StringType>::ConstTypePtr uti::UTFChar<StringType>::Pointer() const
{
	return m_Address;
}

template < class StringType >
inline
uti::u32 uti::UTFChar<StringType>::Size() const
{
	return m_Size;
}

template < class StringType >
bool uti::UTFChar<StringType>::operator==( const UTFChar< StringType >& rhs ) const
{
	// If the size is not equal, the chars must be different
	if (m_Size != rhs.m_Size)
	{
		return false;
	}
	// If any char has a nullptr, we cannot check the bytes, hence there aren't to check....
	if (m_Address == nullptr || rhs.m_Address == nullptr)
	{
		// ....But if the other address it not a nullptr the strings are different....
		if( m_Address != nullptr || rhs.m_Address != nullptr)
		{
			return false;
		}
		// ... And if both are null, they are equivalent again.
		else
		{
			return true;
		}
	}
	// We now have two non-nullptrs with the same length,
	// search the chars for different bytes
	for( u32 i = 0; i < m_Size; i++ )
	{
		// If any byte in the char is different, the chars are different
		if( m_Address[ i ] != rhs.m_Address[ i ] )
		{
			return false;
		}
	}
	return true;
}


template < class StringType >
bool uti::UTFChar<StringType>::operator!=( const UTFChar< StringType >& rhs ) const
{
	// Implement using negation of == operator
	return !( *this == rhs );
}




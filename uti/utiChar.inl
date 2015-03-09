
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


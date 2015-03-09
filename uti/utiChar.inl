
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


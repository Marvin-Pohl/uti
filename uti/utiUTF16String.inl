#pragma once
#ifndef utiUTF16String_inl__
#define utiUTF16String_inl__
namespace uti
{
	//////////////////////////////////////////////////////////////////////////
	// UTF-16 String Implementation
	//////////////////////////////////////////////////////////////////////////

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	UTF16String< ch, order, Allocator >::UTF16String( void )
	{
		CreateEmptyString();
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	UTF16String< ch, order, Allocator >::UTF16String( const UTF16String< ch, order, Allocator >& rhs ) :
		m_pData( rhs.m_pData ),
		m_uiSize( rhs.m_uiSize ),
		m_Alloc( rhs.m_Alloc ),
		m_uiCharCount( rhs.m_uiCharCount )
	{

	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	UTF16String< ch, order, Allocator >::UTF16String( const ch* text )
	{
		if( text != nullptr )
		{
			CopyConstChar( text );
		}
		else
		{
			CreateEmptyString();
		}
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	UTF16String< ch, order, Allocator >::~UTF16String()
	{
		m_pData.SetNull();
		m_uiSize = 0U;
		m_uiCharCount = 0U;
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	void uti::UTF16String< ch, order, Allocator >::CreateEmptyString()
	{
		m_pData = static_cast< ch* >( m_Alloc.AllocateBytes( sizeof( ch ) ) );
		m_pData[ 0 ] = 0U;
		m_uiSize = 0U;
		m_uiCharCount = 0U;
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	u32 uti::UTF16String< ch, order, Allocator >::Size( void ) const
	{
		return m_uiSize * sizeof( ch );
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	u32 uti::UTF16String< ch, order, Allocator >::CharCount( void ) const
	{
		return m_uiCharCount;
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	u32 uti::UTF16String< ch, order, Allocator >::ExtractCodePoint( const ch* utfchar )
	{
		return _ExtractCodePoint_impl( utfchar, if_<order == BinaryOrder::LittleEndian, is_le, is_be>::type() );
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian*/, typename Allocator /*= ::uti::DefaultAllocator */>
	u32 uti::UTF16String<ch, order, Allocator>::_ExtractCodePoint_impl( const ch* utfchar, is_be /*= is_be() */ )
	{
		//Big Endian version
		u32 length = ValidChar( utfchar );
		if( length == 0U )
		{
			return length;
		}
		else
		{
			//Extract code point by filtering out the continuation and byte count marks and shifting the code points value together.

			u32 result = 0U;
			ch byte1;
			ch byte2;
			switch( length )
			{
				// Easiest case, Value is equal to its code point so simply return the value.
			case 1U:
				byte1 = _byteswap_ushort( *utfchar );
				result = byte1;
				break;
				// Second case using lead and trail surrogates
			case 2U:
				byte1 = _byteswap_ushort( *utfchar );
				byte2 = _byteswap_ushort( *( utfchar + 1 ) );
				byte1 -= 0xD800U;
				byte2 -= 0xDC00U;
				byte1 = byte1 << 10;
				result = byte1 | byte2;
				result += 0x10000U;
				break;
			default:
				break;
			}
			return result;
		}
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian*/, typename Allocator /*= ::uti::DefaultAllocator */>
	u32 uti::UTF16String<ch, order, Allocator>::_ExtractCodePoint_impl( const ch* utfchar, is_le /*= is_le() */ )
	{
		u32 length = ValidChar( utfchar );
		if( length == 0U )
		{
			return length;
		}
		else
		{
			//Extract code point by filtering out the continuation and byte count marks and shifting the code points value together.

			u32 result = 0U;
			ch byte1;
			ch byte2;
			switch( length )
			{
				// Easiest case, Value is equal to its code point so simply return the value.
			case 1U:
				result = *utfchar;
				break;
				// Second case using lead and trail surrogates
			case 2U:
				byte1 = *utfchar;
				byte2 = *( utfchar + 1 );
				byte1 -= 0xD800U;
				byte2 -= 0xDC00U;
				byte1 = byte1 << 10;
				result = byte1 | byte2;
				result += 0x10000U;
				break;
			default:
				break;
			}
			return result;
		}
	}



	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	u32 uti::UTF16String< ch, order, Allocator >::Concat( const UTF16String< ch, order, Allocator >& rhs )
	{
		u32 newSize = m_uiSize + rhs.m_uiSize;
		ch* newRawStringData = static_cast< ch* >( m_Alloc.AllocateBytes( newSize * sizeof( ch ) + sizeof( ch ) ) );

		std::memcpy( newRawStringData, m_pData.Ptr(), m_uiSize * sizeof( ch ) );
		std::memcpy( newRawStringData + m_uiSize, rhs.m_pData.Ptr(), rhs.m_uiSize * sizeof( ch ) );
		newRawStringData[ newSize ] = 0U;
		m_pData = newRawStringData;
		m_uiSize = newSize;
		m_uiCharCount += rhs.m_uiCharCount;
		return newSize;
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	UTF16String< ch, order, Allocator > uti::UTF16String< ch, order, Allocator >::operator+( const UTF16String< ch, order, Allocator >& rhs ) const
	{
		UTF16String< ch, order, Allocator > newString( *this );
		newString.Concat( rhs );
		return newString;
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	UTF16String< ch, order, Allocator >& uti::UTF16String< ch, order, Allocator >::operator+=( const UTF16String< ch, order, Allocator >& rhs )
	{
		Concat( rhs );
		return *this;
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	u32 UTF16String<ch, order, Allocator >::ValidChar( const ch* utfchar )
	{
		return _ValidChar_impl( utfchar, if_<order == BinaryOrder::LittleEndian, is_le, is_be>::type() );
	}


	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian*/, typename Allocator /*= ::uti::DefaultAllocator */>
	u32 uti::UTF16String<ch, order, Allocator>::_ValidChar_impl( const ch* utfchar, is_be /*= is_be() */ )
	{
		bool result = true;
		u32 numBytes = CharSize( utfchar );
		ch byte1 = _byteswap_ushort( *utfchar );
		ch byte2 = _byteswap_ushort( *( utfchar + 1 ) );
		if( numBytes == 2U )
		{
			result = result && ( byte1 >= 0xD800U || byte1 < 0xDC00 ) && ( byte2 >= 0xDC00U || byte2 < 0xE000U );
		}

		if( !result )
		{
			return 0U;
		}
		else
		{
			return numBytes;
		}
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian*/, typename Allocator /*= ::uti::DefaultAllocator */>
	u32 uti::UTF16String<ch, order, Allocator>::_ValidChar_impl( const ch* utfchar, is_le /*= is_le() */ )
	{
		bool result = true;
		u32 numBytes = CharSize( utfchar );
		ch byte1 = ( *utfchar );
		ch byte2 = ( *( utfchar + 1 ) );
		if( numBytes == 2U )
		{
			result = result && ( byte1 >= 0xD800U && byte1 < 0xDC00 ) && ( byte2 >= 0xDC00U && byte2 < 0xE000U );
		}

		if( !result )
		{
			return 0U;
		}
		else
		{
			return numBytes;
		}
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	u32 uti::UTF16String< ch, order, Allocator >::CharSize( const ch* utfchar )
	{
		return _CharSize_impl( utfchar, if_<order == BinaryOrder::LittleEndian, is_le, is_be>::type() );
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian*/, typename Allocator /*= ::uti::DefaultAllocator */>
	u32 uti::UTF16String<ch, order, Allocator>::_CharSize_impl( const ch* utfchar, is_be /*= is_be() */ )
	{
		ch myByte = *utfchar;
		myByte = _byteswap_ushort( myByte );

		if( myByte < 0xD800U || myByte >= 0xE000U )
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian*/, typename Allocator /*= ::uti::DefaultAllocator */>
	u32 uti::UTF16String<ch, order, Allocator>::_CharSize_impl( const ch* utfchar, is_le /*= is_le() */ )
	{
		if( *utfchar < 0xD800U || *utfchar >= 0xE000U )
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}


	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	ch* UTF16String< ch, order, Allocator >::ReplacementChar = nullptr;

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	UTF16String< ch, order, Allocator >& UTF16String< ch, order, Allocator >::operator=( const ch* rhs )
	{
		CopyConstChar( rhs );
		return *this;
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	UTF16String< ch, order, Allocator >& UTF16String< ch, order, Allocator >::operator=( const UTF16String< ch, order, Allocator >& rhs )
	{
		m_pData = rhs.m_pData;
		m_uiSize = rhs.m_uiSize;
		m_Alloc = rhs.m_Alloc;
		m_uiCharCount = rhs.m_uiCharCount;
		return *this;
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	typename UTF16String< ch, order, Allocator >::ReverseIterator UTF16String< ch, order, Allocator >::rEnd( void ) const
	{
		return ReverseIterator( Begin() );
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	typename UTF16String< ch, order, Allocator >::ReverseIterator UTF16String< ch, order, Allocator >::rBegin( void ) const
	{
		return ReverseIterator( End() );
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	typename UTF16String< ch, order, Allocator >::Iterator UTF16String< ch, order, Allocator >::End( void ) const
	{
		return UTF16String< ch, order, Allocator >::Iterator( ( UTF16String& ) *this, m_uiSize );
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	typename UTF16String< ch, order, Allocator >::Iterator UTF16String< ch, order, Allocator >::Begin( void ) const
	{
		return UTF16String< ch, order, Allocator >::Iterator( ( UTF16String& ) *this, 0U );
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	typename UTF16String< ch, order, Allocator >::CharReverseIterator uti::UTF16String< ch, order, Allocator >::rCharEnd( void ) const
	{
		return CharReverseIterator( CharBegin() );
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	typename UTF16String< ch, order, Allocator >::CharReverseIterator uti::UTF16String< ch, order, Allocator >::rCharBegin( void ) const
	{
		return CharReverseIterator( CharEnd() );
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	typename UTF16String< ch, order, Allocator >::CharIterator uti::UTF16String< ch, order, Allocator >::CharEnd( void ) const
	{
		return UTF16String< ch, order, Allocator >::CharIterator( ( UTF16String& ) *this, m_uiSize );
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	typename UTF16String< ch, order, Allocator >::CharIterator uti::UTF16String< ch, order, Allocator >::CharBegin( void ) const
	{
		return UTF16String< ch, order, Allocator >::CharIterator( ( UTF16String& ) *this, 0 );
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	bool UTF16String< ch, order, Allocator >::operator!=( const UTF16String& rhs ) const
	{

		if( m_uiSize != rhs.m_uiSize )
		{
			return true;
		}

		Iterator lStart = Begin();
		Iterator rStart = rhs.Begin();
		Iterator lEnd = End();
		Iterator rEnd = rhs.End();

		while( lStart != lEnd && rStart != rEnd )
		{
			if( *( lStart++ ) != *( rStart++ ) )
			{
				return true;
			}
		}

		return false;
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	bool UTF16String< ch, order, Allocator >::operator==( const UTF16String& rhs ) const
	{
		if( m_uiSize != rhs.m_uiSize )
		{
			return false;
		}

		Iterator lStart = Begin();
		Iterator rStart = rhs.Begin();
		Iterator lEnd = End();
		Iterator rEnd = rhs.End();

		while( lStart != lEnd && rStart != rEnd )
		{
			if( *( lStart++ ) != *( rStart++ ) )
			{
				return false;
			}
		}

		return true;
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	bool UTF16String< ch, order, Allocator >::Empty( void ) const
	{
		return m_pData.Null() || m_pData[ 0 ] == 0U;
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	const ch* UTF16String< ch, order, Allocator >::c_str() const
	{
		if( m_pData.Valid() )
		{
			return m_pData.Ptr();
		}
		return nullptr;
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	ch* UTF16String< ch, order, Allocator >::Data() const
	{
		return m_pData.Ptr();
	}

	template < typename ch /*= short*/, ::uti::BinaryOrder order /*= ::uti::BinaryOrder::LittleEndian */, typename Allocator /*= DefaultAllocator */>
	void UTF16String< ch, order, Allocator >::CopyConstChar( const ch* text )
	{
		u32 size = 0U;
		u32 validSize = 0U;
		while( text[ size ] != 0U )
		{
			++validSize;
			++size;
		}
		m_pData = DataType( static_cast< ch* >( m_Alloc.AllocateBytes( ( size + 1U ) * sizeof( ch ) ) ) );

		u32 bytesToNextChar = ValidChar( text );
		u32 arrayPos = 0U;
		m_uiCharCount = ( bytesToNextChar > 0U && ( *text != 0U ) ) ? 1U : 0U;
		for( u32 i = 0U; i < size; ++i )
		{
			if( bytesToNextChar == 0U )
			{
				bytesToNextChar = ValidChar( text + i );
				if( bytesToNextChar == 0U )
				{
					if( ReplacementChar != nullptr && &ReplacementChar != 0U )
					{
						u32 replaceCharCount = ValidChar( ReplacementChar );

						if( replaceCharCount != 0U ) // Replace invalid Chars with ReplacementChar
						{
							u32 replaceCount = ValidChar( text + i );
							while( i < size && replaceCount == 0U )
							{
								for( u32 k = 0U; k < replaceCharCount && i < size; ++k )
								{
									m_pData[ arrayPos++ ] = ReplacementChar[ k ];
								}
								++i;
								++m_uiCharCount;
								replaceCount = ValidChar( text + i );
							}
						}
						else //Skip invalid Bytes, because Replacement Char is also Invalid
						{
							u32 replaceCount = ValidChar( text + i );
							while( i < size && replaceCount == 0U )
							{
								++i;
								replaceCount = ValidChar( text + i );
							}
						}
					}
					else //Skip invalid Bytes, because Replacement Char is null
					{
						u32 replaceCount = ValidChar( text + i );
						while( i < size && replaceCount == 0U )
						{
							++i;
							replaceCount = ValidChar( text + i );
						}
					}
				}
				else
				{
					++m_uiCharCount;
					m_pData[ arrayPos++ ] = static_cast< ch >( text[ i ] );
					if( bytesToNextChar > 0U )
					{
						--bytesToNextChar;
					}
				}

			}
			else
			{
				m_pData[ arrayPos++ ] = static_cast< ch >( text[ i ] );
				if( bytesToNextChar > 0U )
				{
					--bytesToNextChar;
				}
			}

		}
		m_uiSize = arrayPos;
		m_pData[ m_uiSize ] = 0U;

	}
}

#endif // utiUTF16String_inl__

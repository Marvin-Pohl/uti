#pragma once
#ifndef utiUTF8String_inl__
#define utiUTF8String_inl__

namespace uti
{
	//////////////////////////////////////////////////////////////////////////
	// UTF-8 String Implementation
	//////////////////////////////////////////////////////////////////////////
	template < typename ch /*= char*/, typename Allocator /*= DefaultAllocator */>
	UTF8String<ch, Allocator>::UTF8String( void )
	{
		CreateEmptyString();
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	UTF8String<ch, Allocator>::UTF8String( const UTF8String< ch, Allocator >& rhs ) :
		m_pData( rhs.m_pData ),
		m_uiSize( rhs.m_uiSize ),
		m_Alloc( rhs.m_Alloc ),
		m_uiCharCount( rhs.m_uiCharCount )
	{

	}

	template < typename ch /*= char*/, typename Allocator /*= DefaultAllocator */>
	UTF8String<ch, Allocator>::UTF8String( const ch* text )
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

	template < typename ch /*= char*/, typename Allocator /*= DefaultAllocator */>
	UTF8String<ch, Allocator>::~UTF8String()
	{
		m_pData.SetNull();
		m_uiSize = 0U;
		m_uiCharCount = 0U;
	}

	template < typename ch /*= char*/, typename Allocator /*= DefaultAllocator */>
	template< ::uti::BinaryOrder order >
	static inline UTF8String<ch, Allocator>
		UTF8String<ch, Allocator>::FromWideString( const wchar_t* text )
	{
		UTF8String<ch, Allocator> tmpString;
		UTF16String< wchar_t, order, Allocator > utf16String = UTF16String< wchar_t, order, Allocator >( text );
		u32 curIndex = 0U;
		u32 curSize = utf16String.CharCount() + sizeof( ch );
		u32 curActualSize = 0U;
		u32 size = 0U;
		u32 codePoint = 0U;
		tmpString.m_pData = static_cast< ch* >( tmpString.m_Alloc.AllocateBytes( curSize ) );

		for( UTF16String< wchar_t, order, Allocator >::CharIterator iter = utf16String.CharBegin();
			iter != utf16String.CharEnd();
			++iter )
		{
			codePoint = UTF16String< wchar_t, order, Allocator >::ExtractCodePoint( *iter );
			size = GetCodePointSize( codePoint );
			if( curIndex + size > curSize )
			{
				DataType tmpData = static_cast< ch* >( tmpString.m_Alloc.AllocateBytes( curSize * 2U ) );
				curSize *= 2;
				memcpy( tmpData.Ptr(), tmpString.m_pData.Ptr(), curIndex );
				tmpString.m_pData = tmpData;
			}
			FromCodePoint( codePoint, tmpString.m_pData.Ptr() + curIndex );
			curActualSize += size;
			curIndex += size;

		}

		if( curIndex >= curSize )
		{
			DataType tmpData = static_cast< ch* >( tmpString.m_Alloc.AllocateBytes( curSize + sizeof( ch ) ) );
			curSize += sizeof( ch );
			memcpy( tmpData.Ptr(), tmpString.m_pData.Ptr(), curActualSize );
			tmpString.m_pData = tmpData;

		}
		tmpString.m_pData[ curIndex ] = 0U;
		tmpString.m_uiSize = curActualSize;
		tmpString.m_uiCharCount = utf16String.CharCount();
		return tmpString;
	}


	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	UTF8String<ch, Allocator> UTF8String<ch, Allocator>::FromUTF16BE( const wchar_t* text )
	{
		return FromWideString< ::uti::BinaryOrder::BigEndian >( text );
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	UTF8String<ch, Allocator> UTF8String<ch, Allocator>::FromUTF16LE( const wchar_t* text )
	{
		return FromWideString< ::uti::BinaryOrder::LittleEndian >( text );
	}


	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	u32 UTF8String<ch, Allocator>::GetCodePointSize( u32 codePoint )
	{
		if( codePoint < 0x0080U )
		{
			return 1U;
		}
		else if( codePoint < 0x0800U )
		{
			return 2U;
		}
		else if( codePoint < 0x010000U )
		{
			return 3U;
		}
		else if( codePoint < 0x0200000U )
		{
			return 4U;
		}
		else
		{
			return 0;
		}
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	void UTF8String<ch, Allocator>::FromCodePoint( u32 codePoint, ch* dst )
	{
		u32 size = GetCodePointSize( codePoint );
		static const u32 mask7bit = 0x7FU;
		static const u32 mask6bit = 0x3FU;
		static const u32 mask5bit = 0x1FU;
		static const u32 mask4bit = 0x0FU;
		static const u32 mask3bit = 0x07U;
		static const u32 followBit = 0x80U;
		if( size != 0U )
		{
			switch( size )
			{
			case 4:
				dst[ 0 ] = ( ( codePoint >> 18U ) & mask3bit ) | 0xF0U; // Add 11110000b to indicate 4 byte sequence
				dst[ 1 ] = ( ( codePoint >> 12U ) & mask6bit ) | followBit;
				dst[ 2 ] = ( ( codePoint >> 6U ) & mask6bit ) | followBit;
				dst[ 3 ] = ( ( codePoint ) & mask6bit ) | followBit;
				break;
			case 3:
				dst[ 0 ] = ( ( codePoint >> 12U ) & mask4bit ) | 0xE0U; // Add 11100000b to indicate 3 byte sequence
				dst[ 1 ] = ( ( codePoint >> 6U ) & mask6bit ) | followBit;
				dst[ 2 ] = ( ( codePoint ) & mask6bit ) | followBit;
				break;
			case 2:
				dst[ 0 ] = ( ( codePoint >> 6U ) & mask5bit ) | 0xC0U; // Add 11000000b to indicate 2 byte sequence
				dst[ 1 ] = ( ( codePoint ) & mask6bit ) | followBit;
				break;
			case 1:
				dst[ 0 ] = ( codePoint & mask7bit ); // Mask the msb to indicate single byte sequence
				break;
			default:
				break;
			}
		}
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	void UTF8String<ch, Allocator>::CreateEmptyString()
	{
		m_pData = static_cast< ch* >( m_Alloc.AllocateBytes( sizeof( ch ) ) );
		m_pData[ 0 ] = 0U;
		m_uiSize = 0U;
		m_uiCharCount = 0U;
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	u32 UTF8String<ch, Allocator>::Size( void ) const
	{
		return m_uiSize;
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	u32 UTF8String<ch, Allocator>::CharCount( void ) const
	{
		return m_uiCharCount;
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	u32 UTF8String<ch, Allocator>::ExtractCodePoint( const ch* utfchar )
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
			switch( length )
			{
				// Easiest case BXXXXXXX; we want the Xes, but B is always 0 so simply return the value.
			case 1U:
				result = *utfchar;
				// Second case is BBBXXXXX BBXXXXXX, so we filter the second byte by 0x3FU.
				// Then filter the first by 0x1FU and shift it 6 bits to the left, to attach it to the second value
			case 2U:
				result = utfchar[ 1U ] & 0x3FU; // Extract lower value, filtering continuation part
				result |= ( ( u32 ) ( utfchar[ 0U ] & 0x1FU ) ) << 6U; // Add upper value, filtering 2 byte mark
				break;
				// Third case is BBBBXXXX BBXXXXXX BBXXXXXX, so we filter the third byte by 0x3FU.
				// Then filter the second by 0x3FU and shift it 6 bits to the left, to attach it to the output value
				// Then filter the first by 0x0FU and shift it 12 bits to the left, to attach it to the output value
			case 3U:
				result = utfchar[ 2U ] & 0x3FU;
				result |= ( ( u32 ) ( utfchar[ 1U ] & 0x3FU ) ) << 6U;
				result |= ( ( u32 ) ( utfchar[ 0U ] & 0x0FU ) ) << 12U;
				break;
				// Fourth case is BBBBBXXX BBXXXXXX BBXXXXXX BBXXXXXX, so we filter the fourth byte by 0x3FU.
				// Then filter the third by 0x3FU and shift it 6 bits to the left, to attach it to the output value
				// Then filter the second by 0x3FU and shift it 12 bits to the left, to attach it to the output value
				// Then filter the first by 0x007U and shift it 18 bits to the left, to attach it to the output value
			case 4U:
				result = utfchar[ 3U ] & 0x3FU;
				result |= ( ( u32 ) ( utfchar[ 2U ] & 0x3FU ) ) << 6U;
				result |= ( ( u32 ) ( utfchar[ 1U ] & 0x3FU ) ) << 12U;
				result |= ( ( u32 ) ( utfchar[ 0U ] & 0x07U ) ) << 18U;
				break;
			default:
				break;
			}
			return result;
		}
	}



	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	u32 UTF8String<ch, Allocator>::Concat( const UTF8String<ch, Allocator>& rhs )
	{
		u32 newSize = m_uiSize + rhs.m_uiSize;
		ch* newRawStringData = static_cast< ch* >( m_Alloc.AllocateBytes( newSize + sizeof( ch ) ) );

		std::memcpy( newRawStringData, m_pData.Ptr(), m_uiSize );
		std::memcpy( newRawStringData + m_uiSize, rhs.m_pData.Ptr(), rhs.m_uiSize );
		newRawStringData[ newSize ] = 0U;
		m_pData = newRawStringData;
		m_uiSize = newSize;
		m_uiCharCount += rhs.m_uiCharCount;
		return newSize;
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	UTF8String< ch, Allocator> UTF8String<ch, Allocator>::operator+( const UTF8String<ch, Allocator>& rhs ) const
	{
		UTF8String< ch, Allocator> newString( *this );
		newString.Concat( rhs );
		return newString;
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	UTF8String< ch, Allocator>& UTF8String<ch, Allocator>::operator+=( const UTF8String<ch, Allocator>& rhs )
	{
		Concat( rhs );
		return *this;
	}

	template< typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */ >
	u32 UTF8String<ch, Allocator >::ValidChar( const ch* utfchar )
	{

		bool result = true;
		result &= ValidByte( utfchar );
		u32 numBytes = CharSize( utfchar );
		if( numBytes == 2U )
		{
			result = result && ( *utfchar < 0xD8U || *utfchar >0xDFU );
		}
		for( u32 i = 1; result && i < numBytes; ++i )
		{
			result = result && ValidByte( utfchar + i ) && ( utfchar[ i ] & 0xC0U ) == 0x80U;
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


	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	u32 UTF8String<ch, Allocator>::CharSize( const ch* utfchar )
	{
		u32 numBytes;

		if( ( *utfchar & 0x80U ) == 0 )
		{
			return ValidByte( utfchar ) ? 1 : 0;
		}
		else if( ( *utfchar & 0xE0U ) == 0xC0U )
		{
			numBytes = 2;
		}
		else if( ( *utfchar & 0xF0U ) == 0xE0U )
		{
			numBytes = 3;
		}
		else if( ( *utfchar & 0xF8U ) == 0xF0U )
		{
			numBytes = 4;
		}
		else
		{
			numBytes = 0;
		}
		return numBytes;
	}


	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	ch* UTF8String<ch, Allocator>::ReplacementChar = nullptr;

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	UTF8String< ch, Allocator>& UTF8String<ch, Allocator>::operator=( const ch* rhs )
	{
		CopyConstChar( rhs );
		return *this;
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	UTF8String< ch, Allocator>& UTF8String<ch, Allocator>::operator=( const UTF8String< ch, Allocator>& rhs )
	{
		m_pData = rhs.m_pData;
		m_uiSize = rhs.m_uiSize;
		m_Alloc = rhs.m_Alloc;
		m_uiCharCount = rhs.m_uiCharCount;
		return *this;
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	typename UTF8String<ch, Allocator>::ReverseIterator UTF8String<ch, Allocator>::rEnd( void ) const
	{
		return ReverseIterator( Begin() );
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	typename UTF8String<ch, Allocator>::ReverseIterator UTF8String<ch, Allocator>::rBegin( void ) const
	{
		return ReverseIterator( End() );
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	typename UTF8String<ch, Allocator>::Iterator UTF8String<ch, Allocator>::End( void ) const
	{
		return UTF8String<ch, Allocator>::Iterator( ( UTF8String& ) *this, m_uiSize );
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	typename UTF8String<ch, Allocator>::Iterator UTF8String<ch, Allocator>::Begin( void ) const
	{
		return UTF8String<ch, Allocator>::Iterator( ( UTF8String& ) *this, 0U );
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	typename UTF8String<ch, Allocator>::CharReverseIterator UTF8String<ch, Allocator>::rCharEnd( void ) const
	{
		return CharReverseIterator( CharBegin() );
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	typename UTF8String<ch, Allocator>::CharReverseIterator UTF8String<ch, Allocator>::rCharBegin( void ) const
	{
		return CharReverseIterator( CharEnd() );
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	typename UTF8String<ch, Allocator>::CharIterator UTF8String<ch, Allocator>::CharEnd( void ) const
	{
		return UTF8String<ch, Allocator>::CharIterator( ( UTF8String& ) *this, m_uiSize );
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	typename UTF8String<ch, Allocator>::CharIterator UTF8String<ch, Allocator>::CharBegin( void ) const
	{
		return UTF8String<ch, Allocator>::CharIterator( ( UTF8String& ) *this, 0 );
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	bool UTF8String<ch, Allocator>::operator!=( const UTF8String& rhs ) const
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

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	bool UTF8String<ch, Allocator>::operator==( const UTF8String& rhs ) const
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

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	bool UTF8String<ch, Allocator>::Empty( void ) const
	{
		return m_pData.Null() || m_pData[ 0 ] == 0U;
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	const ch* UTF8String<ch, Allocator>::c_str() const
	{
		if( m_pData.Valid() )
		{
			return m_pData.Ptr();
		}
		return nullptr;
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	ch* UTF8String<ch, Allocator>::Data() const
	{
		return m_pData.Ptr();
	}

	template < typename ch /*= char*/, typename Allocator /*= DefaultAllocator */>
	bool UTF8String<ch, Allocator>::ValidByte( const ch* utfchar )
	{
		if( utfchar == nullptr )
		{
			return false;
		}
		else
		{
			return ( unsigned char ) *utfchar <= 0xF4U && ( ( unsigned char ) *utfchar < 0xC0U || ( unsigned char ) *utfchar > 0xC1U );
		}
	}

	template < typename ch /*= char*/, typename Allocator /*= DefaultAllocator */>
	void UTF8String< ch, Allocator >::CopyConstChar( const ch* text )
	{
		u32 size = 0U;
		u32 validSize = 0U;
		while( text[ size ] != 0U )
		{
			if( ValidByte( text + size++ ) )
			{
				++validSize;
			}
			else if( validSize > 0U )
			{
				--validSize;
			}
		}
		m_pData = DataType( static_cast< ch* >( m_Alloc.AllocateBytes( ( size + sizeof( ch ) ) * sizeof( ch ) ) ) );

		u32 bytesToNextChar = ValidChar( text );
		u32 arrayPos = 0U;
		m_uiCharCount = bytesToNextChar > 0U ? 1U : 0U;
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
#endif // utiUTF8String_inl__

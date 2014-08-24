#pragma once
#ifndef utiUTF8String_h__
#define utiUTF8String_h__

namespace uti
{
	/**
	\brief Class representing a valid UTF-8 string.

	\tparam ch Is the type used for a single byte ( not a full UTF-8 char! ).
	The default is char which is ok for the most compilers.
	You might want to change this parameter if you prefer to store the UTF-8 String in a specific way.

	\tparam Allocator Is the class used to allocate the memory for the string (and memory for the Reference Counting)

	*/
	template < typename ch = char, typename Allocator = ::uti::DefaultAllocator >
	class UTF8String
	{
	public:

		typedef const ch ConstType;
		typedef ch Type;
		typedef Allocator AllocatorType;
		typedef typename UTF8String< ch, Allocator> ThisType;

		typedef typename ::uti::UTFByteIterator< ThisType > Iterator;
		typedef typename ::uti::ReverseIterator_tpl< typename Iterator > ReverseIterator;

		typedef typename ::uti::UTFCharIterator< ThisType > CharIterator;
		typedef typename ::uti::ReverseIterator_tpl< typename CharIterator > CharReverseIterator;
		typedef typename ::uti::ReferenceCounted< ch, Allocator > DataType;

		UTF8String( void );

		UTF8String( const ch* text );

		UTF8String( const UTF8String< ch, Allocator >& rhs );

		~UTF8String();

		UTF8String< ch, Allocator>& operator =( const UTF8String< ch, Allocator>& rhs );
		UTF8String< ch, Allocator>& operator =( const ch* rhs );

		UTF8String< ch, Allocator>& operator +=( const UTF8String<ch, Allocator>& rhs );
		UTF8String< ch, Allocator> operator +( const UTF8String<ch, Allocator>& rhs ) const;

		/**
		\brief Appends the given string \c rhs to this string at the End.

		The new Size of the resulting string will be the this->Size() + rhs.Size()

		\return The new Size of the string.
		*/
		u32 Concat( const UTF8String<ch, Allocator>& rhs );


		/**
		\brief Returns a pointer to the data of the String.


		\return The data pointer of the string.
		*/
		ch* Data() const;


		/**
		\brief Compatible implementation to the std::string
		Returns a const type ptr to the data of the string

		\return A const pointer to the data
		*/
		const ch* c_str() const;

		/**
		\brief Returns the size of the string without the '0' at the end,
		so the actual allocated size is \c Size() \c + \c 1
		*/
		u32 Size( void ) const;

		/**
		\brief Returns the char count of the string, as char means single character represented by a code point.

		*/
		u32 CharCount( void ) const;


		/**
		\brief Returns if the UTF-String is empty or not

		\return \c true if the string is empty or \c false if not
		*/
		bool Empty( void ) const;

		bool operator ==( const UTF8String& rhs ) const;
		bool operator !=( const UTF8String& rhs ) const;

		/**
		\brief Returns an iterator to the start of the string,
		which iterates until the end of the string.
		*/
		Iterator Begin( void ) const;
		/**
		\brief Returns an iterator to the end of the string.
		Useful for comparison with an iterator currently iterating
		*/
		Iterator End( void ) const;

		/**
		\brief Returns an iterator which iterates over every char (utf-8 code point) of the String from its start

		*/
		CharIterator CharBegin( void ) const;

		/**
		\brief Returns an iterator which iterates over every char(utf-8 code point) of the String, but placed on the end
		Useful for comparison with an iterator currently iterating

		*/
		CharIterator CharEnd( void ) const;

		/**
		\brief \brief Returns a reverse iterator to the end of the string,
		which iterates towards the start of the string.

		*/
		CharReverseIterator rCharBegin( void ) const;

		/**
		\brief Returns a reverse iterator to the start of the string.
		Useful for comparison with a reverse iterator currently iterating.
		*/
		CharReverseIterator rCharEnd( void ) const;

		/**
		\brief Returns a reverse iterator to the end of the string,
		which iterates towards the start of the string.

		*/
		ReverseIterator rBegin( void ) const;

		/**
		\brief Returns a reverse iterator to the start of the string.
		Useful for comparison with a reverse iterator currently iterating.
		*/
		ReverseIterator rEnd( void ) const;


		/**
		\brief Checks if the given Byte is in range of a valid UTF-8 Byte.
		This does not necessarily makes an array of valid Bytes a valid UTF-8 char or string.

		\param utfchar The char to be tested

		\return \c true if the byte is valid, \c false otherwise
		*/
		static inline bool ValidByte( const ch* utfchar );

		/**
		\brief Checks if the utf-8 char starting at ufchar is a valid char.

		\return The size of the char starting at utfchar or zero if it is invalid

		*/
		static inline u32 ValidChar( const ch* utfChar );

		/**
		\brief Calculates the size of the utf-8 char starting at utfchar without checking for validity.

		\return The size of the char starting at utfchar or zero if it is invalid
		(does not match any known start sequence for utf-8 chars)
		*/
		static inline u32 CharSize( const ch* utfchar );

		/**
		\brief Calculates the code point from the utfchar (the U+XXXX value)

		This method is used to decode utf-8 encoded characters for encoding to other codes.

		*/
		static inline u32 ExtractCodePoint( const ch* utfchar );

		/**
		\brief Takes a codepoint and converts it to utf-8.

		\param codePoint The codepoint which will be converted
		\param dst the target array at which position the converted data will be placed
		(Use UTF8String::GetCodePointSize(u32) to get the needed size for the dst array)
		*/
		static inline void FromCodePoint( u32 codePoint, ch* dst );

		/**
		\brief Returns the needed size for the character to be converted in UTF-8 or 0 if it is an invalid codepoint.

		*/
		static inline u32 GetCodePointSize( u32 codePoint );



		/**
		\brief Takes an UTF-16 LE or BE string and converts it to UTF-8

		\param text The UTF-16 text which will be converted in to an UTF-8 string
		\tparam order The byte order in which the text will be parsed

		\return The converted UTF-8 string
		*/
		template< ::uti::BinaryOrder order >
		static inline UTF8String<ch, Allocator> FromWideString( const wchar_t* text );

		/**
		\brief Takes an UTF-16LE string and converts it to UTF-8.

		\param text The UTF-16LE string which will be converted into UTF-8

		\return The string converted into UTF-8
		*/
		static inline UTF8String<ch, Allocator> FromUTF16LE( const wchar_t* text );

		/**
		\brief Takes an UTF-16BE string and converts it to UTF-8.

		\param text The UTF-16BE string which will be converted into UTF-8

		\return The string converted into UTF-8
		*/
		static inline UTF8String<ch, Allocator> FromUTF16BE( const wchar_t* text );

		friend class UTFByteIterator< ThisType >;
		friend class UTFCharIterator< ThisType >;

		/**
		\brief invalid characters found during creation of utf-8 strings are replaced by the contents of this variable.

		Setting it to a nullptr will skip any invalid characters.

		*/
		static ch* ReplacementChar;

	protected:
	private:
		void CopyConstChar( const ch* text );

		void CreateEmptyString();

		DataType m_pData;
		Allocator m_Alloc;
		u32 m_uiSize;
		u32 m_uiCharCount;
	};
}
#endif // utiUTF8String_h__

#pragma once
#ifndef utiUTF16String_h__
#define utiUTF16String_h__
namespace uti
{
	/**
	\brief Class representing a valid UTF-8 string.

	\tparam ch Is the type used for a single byte ( not a full UTF-8 char! ).
	The default is char which is ok for the most compilers.
	You might want to change this parameter if you prefer to store the UTF-8 String in a specific way.

	\tparam Allocator Is the class used to allocate the memory for the string (and memory for the Reference Counting)

	*/
	template < typename ch = short, ::uti::BinaryOrder order = ::uti::BinaryOrder::LittleEndian, typename Allocator = ::uti::DefaultAllocator >
	class UTF16String
	{
	public:

		typedef const ch ConstType;
		typedef ch Type;
		typedef Allocator AllocatorType;
		typedef typename UTF16String< ch, order, Allocator> ThisType;

		typedef typename ::uti::UTFByteIterator< ThisType > Iterator;
		typedef typename ::uti::ReverseIterator_tpl< typename Iterator > ReverseIterator;

		typedef typename ::uti::UTFCharIterator< ThisType > CharIterator;
		typedef typename ::uti::ReverseIterator_tpl< typename CharIterator > CharReverseIterator;
		typedef typename ::uti::ReferenceCounted< ch, Allocator > DataType;

		UTF16String( void );

		UTF16String( const ch* text );

		UTF16String( const UTF16String< ch, order, Allocator >& rhs );

		~UTF16String();

		UTF16String< ch, order, Allocator >& operator =( const UTF16String< ch, order, Allocator >& rhs );
		UTF16String< ch, order, Allocator >& operator =( const ch* rhs );

		UTF16String< ch, order, Allocator >& operator +=( const UTF16String< ch, order, Allocator >& rhs );
		UTF16String< ch, order, Allocator > operator +( const UTF16String< ch, order, Allocator >& rhs ) const;

		/**
		\brief Appends the given string \c rhs to this string at the End.

		The new Size of the resulting string will be the this->Size() + rhs.Size()

		\return The new Size of the string.
		*/
		u32 Concat( const UTF16String< ch, order, Allocator >& rhs );


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

		bool operator ==( const UTF16String& rhs ) const;
		bool operator !=( const UTF16String& rhs ) const;

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

		friend class UTFByteIterator< ThisType >;
		friend class UTFCharIterator< ThisType >;

		/**
		\brief invalid characters found during creation of utf-8 strings are replaced by the contents of this variable.

		Setting it to a nullptr will skip any invalid characters.

		*/
		static ch* ReplacementChar;

	protected:
	private:

		struct is_le
		{
		};

		struct is_be
		{
		};

		static inline u32 _CharSize_impl( const ch* utfchar, is_le = is_le() );
		static inline u32 _CharSize_impl( const ch* utfchar, is_be = is_be() );

		static inline u32 _ValidChar_impl( const ch* utfchar, is_le = is_le() );
		static inline u32 _ValidChar_impl( const ch* utfchar, is_be = is_be() );

		static inline u32 _ExtractCodePoint_impl( const ch* utfchar, is_le = is_le() );
		static inline u32 _ExtractCodePoint_impl( const ch* utfchar, is_be = is_be() );

		void CopyConstChar( const ch* text );

		void CreateEmptyString();

		DataType m_pData;
		Allocator m_Alloc;
		u32 m_uiSize;
		u32 m_uiCharCount;
	};
}
#endif // utiUTF16String_h__

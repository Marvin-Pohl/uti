#pragma once
#include <exception>
#include <intrin.h>
#ifndef uti_h__
#define uti_h__

#ifdef DISABLE_CPP11
#ifdef nullptr
#define OLD_NULLPTR nullptr
#undef nullptr
#endif // nullptr
#define nullptr NULL
#endif // DISABLE_CPP11


namespace uti
{

	typedef unsigned int u32;

	template< bool condition, typename TrueType, typename FalseType >
	struct if_
	{
	};

	template< typename TrueType, typename FalseType >
	struct if_<true, TrueType, FalseType>
	{
		typedef typename TrueType type;
	};

	template< typename TrueType, typename FalseType >
	struct if_<false, TrueType, FalseType>
	{
		typedef typename FalseType type;
	};

	template < typename ch, typename Allocator >
	class UTF8String;

	class UTFException
		: public std::exception
	{
	public:

		inline UTFException( char const * const & what, int code ) :
			std::exception( what, code )
		{
		}

	protected:
	private:
	};

	class InvalidCharException
		: public UTFException
	{
	public:

		inline InvalidCharException( u32 m_uiPosition ) :
			UTFException( "Invalid char detected!", ( int ) m_uiPosition )
		{

		}

	protected:
	private:
	};

	/**
	\brief Basic Interface for any custom Allocator for the UTFString class.
	To use a custom Allocator write a subclass of IAllocator and pass it as second parameter of the UTFString template arguments.

	The Allocator is intended to work like a proxy allocator, which means it should be lightweight,
	because any instance of the UTFString contains a copy of the Allocator.
	Therefore it is recommended to access the actual Allocator using a static or global definition.

	*/
	class IAllocator
	{
	public:

		virtual void* AllocateBytes( u32 count ) = 0;

		virtual void FreeBytes( void* ptr ) = 0;

	protected:
	private:
	};

	/**
	\brief Default implementation for the UTFString allocator,
	which utilizes new and delete directly to allocate and deallocate Memory.
	*/
	class DefaultAllocator : public IAllocator
	{
	public:

		inline virtual void* AllocateBytes( u32 size );

		inline virtual void FreeBytes( void* ptr );

	protected:

	private:
	};

	/**
	\brief Reference Counter class for copy on write functionality of the UTFString.

	*/
	template< typename T, typename Allocator >
	class ReferenceCounted
	{
	public:
		ReferenceCounted( void );
		ReferenceCounted( T* pointer );
		ReferenceCounted( const ReferenceCounted< T, Allocator > & refCount );
		ReferenceCounted( ReferenceCounted< T, Allocator >&& refCount );
		~ReferenceCounted();


		ReferenceCounted< T, Allocator >& operator =( const ReferenceCounted< T, Allocator >& refCount );
		ReferenceCounted< T, Allocator >& operator =( ReferenceCounted< T, Allocator >&& refCount );

		bool operator ==( const ReferenceCounted< T, Allocator >& rhs ) const;
		bool operator !=( const ReferenceCounted< T, Allocator >& rhs ) const;

		T* operator ->( );

		T* operator *( );

		T* Ptr( void ) const;

		T& operator []( u32 idx );
		const T& operator []( u32 idx ) const;

		void SetNull( void );

		bool Valid( void ) const;

		bool Null( void ) const;

		u32 Count( void ) const;

	private:

		void IncRef( void );

		void DecRef( void );

		T* m_CountedPointer;

		u32* m_Count;

		Allocator m_Alloc;

	};

	/**
	\brief Generic Iterator which iterates over a generic String defined in this header file.
	Each iteration step increases in the atomic size of the underlying String ( e.g. 1 Byte in UTF-8 and 2 bytes in UTF-16 )

	\tparam StringType The underlying String class type, over which the iterator is going to iterate.

	*/
	template< typename StringType >
	class UTFByteIterator
	{
	public:

		typedef typename StringType String;

		typedef typename StringType::Type DataType;
		typedef typename StringType::AllocatorType Allocator;

		/**
		\brief Creates a new iterator for the UTF-String

		\param data the data of the string
		\param m_uiPos The m_uiPos where to start the iteration
		*/
		UTFByteIterator( String& str, u32 m_uiPos );

		UTFByteIterator( const UTFByteIterator< String >& it );

		UTFByteIterator& operator =( const UTFByteIterator< String >& it );

		DataType& operator *( void );

		bool Valid( void ) const;

		bool operator ==( const UTFByteIterator< String >& rhs ) const;
		bool operator !=( const UTFByteIterator< String >& rhs ) const;
		bool operator <=( const UTFByteIterator< String >& rhs ) const;
		bool operator <( const UTFByteIterator< String >& rhs ) const;
		bool operator >=( const UTFByteIterator< String >& rhs ) const;
		bool operator >( const UTFByteIterator< String >& rhs ) const;

		UTFByteIterator< String >& operator ++( void );
		UTFByteIterator< String > operator ++( int );


		UTFByteIterator< String >& operator --( void );
		UTFByteIterator< String > operator --( int );

		~UTFByteIterator();


	protected:
	private:

		String& m_String;
		u32 m_uiPos;
	};

	/**
	\brief Generic Iterator which iterates over a generic String defined in this header file.
	Each iteration step increases in the atomic size of the underlying String ( e.g. 1 Byte in UTF-8 and 2 bytes in UTF-16 )

	\tparam StringType The underlying String class type, over which the iterator is going to iterate.

	*/
	template< typename StringType >
	class UTFCharIterator
	{
	public:

		typedef typename StringType String;

		typedef typename StringType::Type DataType;
		typedef typename StringType::AllocatorType Allocator;

		/**
		\brief Creates a new iterator for the UTF-String

		\param data the data of the string
		\param m_uiPos The m_uiPos where to start the iteration
		*/
		UTFCharIterator( String& str, u32 m_uiPos );

		UTFCharIterator( const UTFCharIterator< String >& it );

		UTFCharIterator& operator =( const UTFCharIterator< String >& it );

		DataType* operator *( void );

		bool Valid( void ) const;

		bool operator ==( const UTFCharIterator< String >& rhs ) const;
		bool operator !=( const UTFCharIterator< String >& rhs ) const;
		bool operator <=( const UTFCharIterator< String >& rhs ) const;
		bool operator <( const UTFCharIterator< String >& rhs ) const;
		bool operator >=( const UTFCharIterator< String >& rhs ) const;
		bool operator >( const UTFCharIterator< String >& rhs ) const;

		UTFCharIterator< String >& operator ++( void );
		UTFCharIterator< String > operator ++( int );


		UTFCharIterator< String >& operator --( void );
		UTFCharIterator< String > operator --( int );

		~UTFCharIterator();


	protected:
	private:

		String& m_String;
		u32 m_uiPos;
	};

	template< typename Iterator >
	class ReverseIterator_tpl
	{
	public:

		ReverseIterator_tpl( const Iterator& it );

		ReverseIterator_tpl( const ReverseIterator_tpl< Iterator >& it );

		ReverseIterator_tpl< Iterator >& operator =( const ReverseIterator_tpl< Iterator >& it );
		ReverseIterator_tpl< Iterator >& operator =( const Iterator& it );

		typename Iterator::DataType& operator *( );

		bool Valid( void ) const;

		bool operator ==( const ReverseIterator_tpl< Iterator >& rhs ) const;
		bool operator !=( const ReverseIterator_tpl< Iterator >& rhs ) const;
		bool operator <=( const ReverseIterator_tpl< Iterator >& rhs ) const;
		bool operator <( const ReverseIterator_tpl< Iterator >& rhs ) const;
		bool operator >=( const ReverseIterator_tpl< Iterator >& rhs ) const;
		bool operator >( const ReverseIterator_tpl< Iterator >& rhs ) const;

		ReverseIterator_tpl< Iterator >& operator ++( void );
		ReverseIterator_tpl< Iterator > operator ++( int );


		ReverseIterator_tpl< Iterator >& operator --( void );
		ReverseIterator_tpl< Iterator > operator --( int );

	protected:
	private:

		Iterator m_It;
	};

	enum class BinaryOrder
	{
		LittleEndian = 0x1,
		BigEndian = 0x2
	};

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

	//////////////////////////////////////////////////////////////////////////
	// Default Allocator implementation
	//////////////////////////////////////////////////////////////////////////

	void* DefaultAllocator::AllocateBytes( u32 size )
	{
		return new char[ size ];
	}

	void DefaultAllocator::FreeBytes( void* ptr )
	{
		delete ptr;
	}

	//////////////////////////////////////////////////////////////////////////
	// Reference Counted Implementation
	//////////////////////////////////////////////////////////////////////////

	template< typename T, typename Allocator >
	bool ReferenceCounted<T, Allocator>::operator!=( const ReferenceCounted< T, Allocator >& rhs ) const
	{
		return m_CountedPointer != rhs.m_CountedPointer;
	}

	template< typename T, typename Allocator >
	bool ReferenceCounted<T, Allocator>::operator==( const ReferenceCounted< T, Allocator >& rhs ) const
	{
		return m_CountedPointer == rhs.m_CountedPointer;
	}

	template< typename T, typename Allocator >
	const T& ReferenceCounted<T, Allocator>::operator[]( u32 idx ) const
	{
		return m_CountedPointer[ idx ];
	}

	template< typename T, typename Allocator >
	bool ReferenceCounted< T, Allocator >::Null( void ) const
	{
		return m_CountedPointer == nullptr;
	}

	template< typename T, typename Allocator >
	u32 ReferenceCounted< T, Allocator>::Count( void ) const
	{
		if( m_Count != nullptr )
		{
			return *m_Count;
		}
		else
		{
			return 0;
		}
	}

	template< typename T, typename Allocator >
	bool ReferenceCounted< T, Allocator >::Valid( void ) const
	{
		return m_CountedPointer != nullptr;
	}

	template< typename T, typename Allocator >
	void ReferenceCounted< T, Allocator >::SetNull( void )
	{
		DecRef();
		m_CountedPointer = nullptr;
		m_Count = nullptr;
	}

	template< typename T, typename Allocator >
	T& ReferenceCounted< T, Allocator >::operator[]( u32 idx )
	{
		return m_CountedPointer[ idx ];
	}

	template< typename T, typename Allocator >
	T* ReferenceCounted< T, Allocator >::Ptr( void ) const
	{
		return m_CountedPointer;
	}

	template< typename T, typename Allocator >
	ReferenceCounted< T, Allocator >::ReferenceCounted( void ) :
		m_CountedPointer( nullptr )
	{
		m_Count = static_cast< u32* >( m_Alloc.AllocateBytes( sizeof( u32 ) ) );
		*m_Count = 1U;
	}

	template< typename T, typename Allocator >
	ReferenceCounted< T, Allocator >::ReferenceCounted( ReferenceCounted< T, Allocator >&& refCount ) :
		m_CountedPointer( refCount.m_CountedPointer ),
		m_Count( refCount.m_Count )
	{
		refCount.m_CountedPointer = nullptr;
		m_Count = nullptr;
	}

	template< typename T, typename Allocator >
	ReferenceCounted< T, Allocator >::~ReferenceCounted()
	{
		DecRef();
	}

	template< typename T, typename Allocator >
	ReferenceCounted< T, Allocator >::ReferenceCounted( const ReferenceCounted< T, Allocator >& refCount )
	{

		m_Count = refCount.m_Count;
		m_CountedPointer = refCount.m_CountedPointer;

		IncRef();
	}

	template< typename T, typename Allocator >
	ReferenceCounted< T, Allocator >::ReferenceCounted( T* pointer ) :
		m_CountedPointer( pointer )
	{
		m_Count = static_cast< u32* >( m_Alloc.AllocateBytes( sizeof( u32 ) ) );
		*m_Count = 1U;
	}

	template< typename T, typename Allocator >
	T* ReferenceCounted< T, Allocator >::operator*( )
	{
		return m_CountedPointer;
	}

	template< typename T, typename Allocator >
	T* ReferenceCounted< T, Allocator >::operator->( )
	{
		return m_CountedPointer;
	}

	template< typename T, typename Allocator >
	ReferenceCounted< T, Allocator >& ReferenceCounted< T, Allocator >::operator=( ReferenceCounted< T, Allocator >&& refCount )
	{
		if( this != &refCount )
		{
			m_CountedPointer = refCount.m_CountedPointer;
			m_Count = refCount.m_Count;

			refCount.m_CountedPointer = nullptr;
			refCount.m_Count = nullptr;
		}

		return *this;

	}

	template< typename T, typename Allocator >
	ReferenceCounted< T, Allocator >& ReferenceCounted< T, Allocator >::operator=( const ReferenceCounted< T, Allocator >& refCount )
	{
		if( m_Count != refCount.m_Count )
		{
			m_CountedPointer = refCount.m_CountedPointer;

			m_Count = refCount.m_Count;

			IncRef();
		}

		return *this;
	}

	template< typename T, typename Allocator >
	void ReferenceCounted< T, Allocator >::DecRef( void )
	{
		if( ( m_Count != nullptr ) && ( *m_Count ) > 1 )
		{
			--( *m_Count );
		}
		else
		{
			m_Alloc.FreeBytes( m_CountedPointer );
			m_CountedPointer = nullptr;
			m_Alloc.FreeBytes( m_Count );
			m_Count = nullptr;
		}
	}

	template< typename T, typename Allocator >
	void ReferenceCounted< T, Allocator >::IncRef( void )
	{
		printf( "Inc" );
		if( ( *m_Count ) > 0U )
		{
			++( *m_Count );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// UTF String Iterator implementation
	//////////////////////////////////////////////////////////////////////////

	template< typename String >
	UTFByteIterator< String > UTFByteIterator< String >::operator--( int )
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if( m_uiPos > 0 )
		{
			UTFByteIterator it( *this );
			--*this;
			return it;
		}
		else
		{
			throw UTFException( "Iterator not decrementable", 0 );
		}
#else

		UTFByteIterator it( *this );
		--*this;
		return it;
#endif // _ITERATOR_DEBUG_LEVEL == 2
	}

	template< typename String >
	UTFByteIterator< String >& UTFByteIterator< String >::operator--( void )
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if( m_uiPos > 0 )
		{
			--m_uiPos;
			return *this;
		}
		else
		{
			throw UTFException( "Iterator not decrementable", 0 );
		}
#else

		--m_uiPos;
		return *this;
#endif // _ITERATOR_DEBUG_LEVEL == 2

	}

	template< typename String >
	UTFByteIterator< String >& UTFByteIterator< String >::operator=( const UTFByteIterator< String >& it )
	{
		m_String = it.m_String;
		m_uiPos = it.m_uiPos;
	}

	template< typename String >
	UTFByteIterator< String > UTFByteIterator< String >::operator++( int )
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if( m_uiPos < m_String.m_uiSize )
		{
			UTFByteIterator it( *this );
			++*this;
			return it;
		}
		else
		{
			throw UTFException( "Iterator not incrementable", 0 );
		}
#else

		UTFByteIterator it( *this );
		++*this;
		return it;
#endif // _ITERATOR_DEBUG_LEVEL == 2
	}

	template< typename String >
	UTFByteIterator< String >& UTFByteIterator< String >::operator++( void )
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if( m_uiPos < m_String.m_uiSize )
		{
			++m_uiPos;
			return *this;
		}
		else
		{
			throw UTFException( "Iterator not incrementable", 0 );
		}
#else

		++m_uiPos;
		return *this;
#endif // _ITERATOR_DEBUG_LEVEL == 2
	}

	template< typename String >
	bool UTFByteIterator< String >::operator>( const UTFByteIterator< String >& rhs ) const
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
	bool UTFByteIterator< String >::operator>=( const UTFByteIterator< String >& rhs ) const
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
	bool UTFByteIterator< String >::operator<( const UTFByteIterator< String >& rhs ) const
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
	bool UTFByteIterator< String >::operator<=( const UTFByteIterator< String >& rhs ) const
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
	bool UTFByteIterator< String >::operator!=( const UTFByteIterator< String >& rhs ) const
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
	bool UTFByteIterator< String >::operator==( const UTFByteIterator< String >& rhs ) const
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
	typename UTFByteIterator< String >::DataType& UTFByteIterator< String >::operator*( )
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if( Valid() )
		{
			return *( m_String.m_pData.Ptr() + m_uiPos );
		}
		else
		{
			throw UTFException( "Iterator not dereferenceable", 0 );
		}
#else
		return *(m_String.m_pData.Ptr() + m_uiPos);
#endif

	}

	template< typename String >
	bool UTFByteIterator< String >::Valid( void ) const
	{
		return m_uiPos < m_String.m_uiSize;
	}

	template< typename String >
	UTFByteIterator< String >::~UTFByteIterator()
	{
		m_uiPos = 0U;
	}

	template< typename String >
	UTFByteIterator< String >::UTFByteIterator( typename UTFByteIterator< String >::String& data, u32 uiPos ) :
		m_String( data ),
		m_uiPos( uiPos )
	{
	}

	template< typename String >
	UTFByteIterator< String >::UTFByteIterator( const UTFByteIterator< String >& it ) :
		m_String( it.m_String ),
		m_uiPos( it.m_uiPos )
	{

	}

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
		uti::UTF8String<ch, Allocator>::FromWideString( const wchar_t* text )
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
	UTF8String<ch, Allocator> uti::UTF8String<ch, Allocator>::FromUTF16BE( const wchar_t* text )
	{
		return FromWideString< ::uti::BinaryOrder::BigEndian >( text );
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	UTF8String<ch, Allocator> uti::UTF8String<ch, Allocator>::FromUTF16LE( const wchar_t* text )
	{
		return FromWideString< ::uti::BinaryOrder::LittleEndian >( text );
	}


	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	u32 uti::UTF8String<ch, Allocator>::GetCodePointSize( u32 codePoint )
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
	void uti::UTF8String<ch, Allocator>::FromCodePoint( u32 codePoint, ch* dst )
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
				dst[ 0 ] = (( codePoint >> 18U ) & mask3bit) | 0xF0U; // Add 11110000b to indicate 4 byte sequence
				dst[ 1 ] = (( codePoint >> 12U ) & mask6bit) | followBit;
				dst[ 2 ] = ( ( codePoint >> 6U ) & mask6bit ) | followBit;
				dst[ 3 ] = ( ( codePoint ) & mask6bit ) | followBit;
				break;
			case 3:
				dst[ 0 ] = ( ( codePoint >> 12U ) & mask4bit ) | 0xE0U; // Add 11100000b to indicate 3 byte sequence
				dst[ 1 ] = ( ( codePoint >> 6U ) & mask6bit ) | followBit;
				dst[ 2 ] = (( codePoint ) & mask6bit) | followBit;
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
	void uti::UTF8String<ch, Allocator>::CreateEmptyString()
	{
		m_pData = static_cast< ch* >( m_Alloc.AllocateBytes( sizeof( ch ) ) );
		m_pData[ 0 ] = 0U;
		m_uiSize = 0U;
		m_uiCharCount = 0U;
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	u32 uti::UTF8String<ch, Allocator>::Size( void ) const
	{
		return m_uiSize;
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	u32 uti::UTF8String<ch, Allocator>::CharCount( void ) const
	{
		return m_uiCharCount;
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	u32 uti::UTF8String<ch, Allocator>::ExtractCodePoint( const ch* utfchar )
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
	u32 uti::UTF8String<ch, Allocator>::Concat( const UTF8String<ch, Allocator>& rhs )
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
	UTF8String< ch, Allocator> uti::UTF8String<ch, Allocator>::operator+( const UTF8String<ch, Allocator>& rhs ) const
	{
		UTF8String< ch, Allocator> newString( *this );
		newString.Concat( rhs );
		return newString;
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	UTF8String< ch, Allocator>& uti::UTF8String<ch, Allocator>::operator+=( const UTF8String<ch, Allocator>& rhs )
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
	u32 uti::UTF8String<ch, Allocator>::CharSize( const ch* utfchar )
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
	typename UTF8String<ch, Allocator>::CharReverseIterator uti::UTF8String<ch, Allocator>::rCharEnd( void ) const
	{
		return CharReverseIterator( CharBegin() );
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	typename UTF8String<ch, Allocator>::CharReverseIterator uti::UTF8String<ch, Allocator>::rCharBegin( void ) const
	{
		return CharReverseIterator( CharEnd() );
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	typename UTF8String<ch, Allocator>::CharIterator uti::UTF8String<ch, Allocator>::CharEnd( void ) const
	{
		return UTF8String<ch, Allocator>::CharIterator( ( UTF8String& ) *this, m_uiSize );
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	typename UTF8String<ch, Allocator>::CharIterator uti::UTF8String<ch, Allocator>::CharBegin( void ) const
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
		ch* newRawStringData = static_cast< ch* >( m_Alloc.AllocateBytes( newSize * sizeof( ch ) + sizeof( ch ) ));

		std::memcpy( newRawStringData, m_pData.Ptr(), m_uiSize * sizeof( ch ));
		std::memcpy( newRawStringData + m_uiSize, rhs.m_pData.Ptr(), rhs.m_uiSize * sizeof( ch ));
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

#ifdef DISABLE_CPP11
#undef nullptr
#ifdef OLD_NULLPTR
#define nullptr OLD_NULLPTR
#undef OLD_NULLPTR
#endif // OLD_NULLPTR

#endif // DISABLE_CPP11



#endif // uti_h__


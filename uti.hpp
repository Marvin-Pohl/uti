#pragma once
#include <exception>
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


		\return The data of the string.
		*/
		ch* Data() const;


		/**
		\brief Compatible implementation to the std::string
		Returns a const type ptr to the data of the string

		\return A const pointer to the data
		*/
		const ch* c_str() const;

		/**
		\brief Returns the size of the string without the '0' at the end, so the actual allocated size is \c Size() \c + \c 1
		*/
		u32 Size( void ) const;


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
		\brief Returns a reverse iterator to the end of the string,
		which iterates towards the start of the string.

		*/
		ReverseIterator rBegin( void ) const;

		/**
		\brief Returns a reverse iterator to the start of the string.
		Useful for comparison with a reverse iterator currently iterating.
		*/
		ReverseIterator rEnd( void ) const;

		void CopyConstChar( const ch* text );

		/**
		\brief Checks if the given Byte is in range of a valid UTF-8 Byte.
		This does not necessarily makes an array of valid Bytes a valid UTF-8 char or string.

		\param utfchar The char to be tested

		\return \c true if the byte is valid, \c false otherwise
		*/
		static inline bool ValidUTF8Byte( const ch* utfchar );

		static inline u32 ValidUTF8Char( const ch* utfChar );

		static inline u32 ExtractCodePointFromUTF8( const ch* utfchar );

		friend class UTFByteIterator< ThisType >;

		static ch* ReplacementChar;

	protected:
	private:

		DataType m_pData;
		Allocator m_Alloc;
		u32 m_uiSize;
		u32 m_uiCharCount;
	};

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
			DecRef();
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
		return *(m_String.m_pData + m_uiPos);
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
	// UTF String Implementation
	//////////////////////////////////////////////////////////////////////////

	template < typename ch /*= char*/, typename Allocator /*= DefaultAllocator */>
	UTF8String<ch, Allocator>::UTF8String( void )
	{
		m_pData = static_cast< ch* >( m_Alloc.AllocateBytes( 1U ) );

		m_pData[ 0 ] = 0U;

		m_uiSize = 0U;
	}

	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	UTF8String<ch, Allocator>::UTF8String( const UTF8String< ch, Allocator >& rhs ) :
		m_pData( rhs.m_pData ),
		m_uiSize( rhs.m_uiSize ),
		m_Alloc( rhs.m_Alloc )
	{

	}

	template < typename ch /*= char*/, typename Allocator /*= DefaultAllocator */>
	UTF8String<ch, Allocator>::UTF8String( const ch* text )
	{
		CopyConstChar( text );

	}

	template < typename ch /*= char*/, typename Allocator /*= DefaultAllocator */>
	UTF8String<ch, Allocator>::~UTF8String()
	{
		m_pData.SetNull();
		m_uiSize = 0U;
	}


	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	u32 uti::UTF8String<ch, Allocator>::Size( void ) const
	{
		return m_uiSize;
	}


	template < typename ch /*= char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	u32 uti::UTF8String<ch, Allocator>::ExtractCodePointFromUTF8( const ch* utfchar )
	{
		u32 length = ValidUTF8Char( utfchar );
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
				result = utfchar[ 1U ] & ( ch ) 0x3FU; // Extract lower value, filtering continuation part
				result |= ( ( u32 ) ( utfchar[ 0U ] & ( ch ) 0x1FU ) ) << 6U; // Add upper value, filtering 2 byte mark
				break;
				// Third case is BBBBXXXX BBXXXXXX BBXXXXXX, so we filter the third byte by 0x3FU.
				// Then filter the second by 0x3FU and shift it 6 bits to the left, to attach it to the output value
				// Then filter the first by 0x0FU and shift it 12 bits to the left, to attach it to the output value
			case 3U:
				result = utfchar[ 2U ] & ( ch ) 0x3FU;
				result |= ( ( u32 ) ( utfchar[ 1U ] & ( ch ) 0x3FU ) ) << 6U;
				result |= ( ( u32 ) ( utfchar[ 0U ] & ( ch ) 0x0FU ) ) << 12U;
				break;
				// Fourth case is BBBBBXXX BBXXXXXX BBXXXXXX BBXXXXXX, so we filter the fourth byte by 0x3FU.
				// Then filter the third by 0x3FU and shift it 6 bits to the left, to attach it to the output value
				// Then filter the second by 0x3FU and shift it 6 bits to the left, to attach it to the output value
				// Then filter the first by 0x007U and shift it 18 bits to the left, to attach it to the output value
			case 4U:
				result = utfchar[ 3U ] & ( ch ) 0x3FU;
				result |= ( ( u32 ) ( utfchar[ 2U ] & ( ch ) 0x3FU ) ) << 6U;
				result |= ( ( u32 ) ( utfchar[ 1U ] & ( ch ) 0x3FU ) ) << 12U;
				result |= ( ( u32 ) ( utfchar[ 0U ] & ( ch ) 0x07U ) ) << 18U;
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
		DataType newStringData = static_cast< ch* >( m_Alloc.AllocateBytes( newSize + 1 ) );

		u32 newPos = 0U;
		for( u32 i = 0U; i < m_uiSize; ++i )
		{
			newStringData[ newPos++ ] = m_pData[ i ];
		}
		for( u32 i = 0U; i < rhs.m_uiSize; ++i )
		{
			newStringData[ newPos++ ] = rhs.m_pData[ i ];
		}
		newStringData[ newSize ] = 0U;
		m_pData = newStringData;
		m_uiSize = newSize;
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
	u32 UTF8String<ch, Allocator >::ValidUTF8Char( const ch* utfchar )
	{

		u32 numBytes;
		bool result = true;
		if( ( *utfchar & ( ch ) 0x80U ) == 0 )
		{
			return ValidUTF8Byte( utfchar ) ? 1 : 0;
		}
		else if( ( *utfchar & ( ch ) 0xE0U ) == ( ch ) 0xC0U )
		{
			numBytes = 2;
			result = result && ( *utfchar < ( ch ) 0xD8U || *utfchar >( ch )0xDFU );
		}
		else if( ( *utfchar & ( ch ) 0xF0U ) == ( ch ) 0xE0U )
		{
			numBytes = 3;
		}
		else if( ( *utfchar & ( ch ) 0xF8U ) == ( ch ) 0xF0U )
		{
			numBytes = 4;
		}
		else
		{
			numBytes = 0;
		}
		result &= ValidUTF8Byte( utfchar );
		for( u32 i = 1; result && i < numBytes; ++i )
		{
			result = result && ValidUTF8Byte( utfchar + i ) && ( utfchar[ i ] & ( ch ) 0xC0U ) == ( ch ) 0x80U;
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
	bool UTF8String<ch, Allocator>::ValidUTF8Byte( const ch* utfchar )
	{
		if( utfchar == nullptr )
		{
			return false;
		}
		else
		{
			return *utfchar <= ( ch ) 0xF4U && ( *utfchar < ( ch ) 0xC0U || *utfchar >( ch ) 0xC1U );
		}
	}

	template < typename ch /*= char*/, typename Allocator /*= DefaultAllocator */>
	void UTF8String< ch, Allocator >::CopyConstChar( const ch* text )
	{
		u32 size = 0U;
		u32 validSize = 0U;
		while( text[ size ] != 0U )
		{
			if( ValidUTF8Byte( text + size++ ) )
			{
				++validSize;
			}
			else if( validSize > 0U )
			{
				--validSize;
			}
		}
		m_pData = DataType( static_cast< ch* >( m_Alloc.AllocateBytes( ( size + 1U ) * sizeof( ch ) ) ) );

		u32 bytesToNextChar = 0U;
		u32 arrayPos = 0U;
		for( u32 i = 0U; i < size; ++i )
		{
			if( bytesToNextChar-- == 0U )
			{
				bytesToNextChar = ValidUTF8Char( text + i );
				if( bytesToNextChar == 0U )
				{
					if( ReplacementChar != nullptr && &ReplacementChar != 0U )
					{
						u32 replaceCharCount = ValidUTF8Char( ReplacementChar );

						if( replaceCharCount != 0U ) // Replace invalid Chars with ReplacementChar
						{
							u32 replaceCount = ValidUTF8Char( text + i );
							while( i < size && replaceCount == 0U )
							{
								for( u32 k = 0U; k < replaceCharCount && i < size; ++k )
								{
									m_pData[ arrayPos++ ] = ReplacementChar[ k ];
								}
								++i;
								replaceCount = ValidUTF8Char( text + i );
							}
						}
						else //Skip invalid Bytes, because Replacement Char is also Invalid
						{
							u32 replaceCount = ValidUTF8Char( text + i );
							while( i < size && replaceCount == 0U )
							{
								++i;
								replaceCount = ValidUTF8Char( text + i );
							}
						}
					}
					else //Skip invalid Bytes, because Replacement Char is null
					{
						u32 replaceCount = ValidUTF8Char( text + i );
						while( i < size && replaceCount == 0U )
						{
							++i;
							replaceCount = ValidUTF8Char( text + i );
						}
					}
				}
				else
				{
					m_pData[ arrayPos++ ] = static_cast< ch >( text[ i ] );
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


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

	template < typename ch , typename Allocator >
	class UTFString;

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
		UTFException( "Invalid char detected!", ( int )m_uiPosition )
		{

		}

	protected:
	private:
	};

	class IAllocator
	{
	public:

		virtual void* AllocateBytes( u32 count ) = 0;

		virtual void FreeBytes( void* ptr ) = 0;

	protected:
	private:
	};

	class DefaultAllocator : public IAllocator
	{
	public:

		inline virtual void* AllocateBytes( u32 size );

		inline virtual void FreeBytes( void* ptr );

	protected:

	private:
	};

	void* DefaultAllocator::AllocateBytes( u32 size )
	{
		return new char[ size ];
	}

	void DefaultAllocator::FreeBytes( void* ptr )
	{
		delete ptr;
	}

	template< typename ch = unsigned char, typename Allocator = ::uti::DefaultAllocator >
	class UTFStringIterator
	{
	public:

		typedef typename UTFString< typename ch,typename Allocator> String;

		typedef typename ch DataType;
		
		/**
		\brief Creates a new iterator for the UTF-String
		
		\param data the data of the string
		\param m_uiPos The m_uiPos where to start the iteration
		*/
		UTFStringIterator( String& str, u32 m_uiPos );

		UTFStringIterator( const UTFStringIterator< ch, Allocator >& it );

		UTFStringIterator& operator =( const UTFStringIterator< ch, Allocator >& it );

		ch& operator *();

		bool Valid( void ) const;

		bool operator ==( const UTFStringIterator< ch, Allocator>& rhs ) const;
		bool operator !=( const UTFStringIterator< ch, Allocator>& rhs ) const;
		bool operator <=( const UTFStringIterator< ch, Allocator>& rhs ) const;
		bool operator <( const UTFStringIterator< ch, Allocator>& rhs ) const;
		bool operator >=( const UTFStringIterator< ch, Allocator>& rhs ) const;
		bool operator >( const UTFStringIterator< ch, Allocator>& rhs ) const;

		UTFStringIterator< ch, Allocator>& operator ++( void );
		UTFStringIterator< ch, Allocator> operator ++( int ) const;


		UTFStringIterator< ch, Allocator>& operator --( void );
		UTFStringIterator< ch, Allocator> operator --( int ) const;

		~UTFStringIterator();


	protected:
	private:

		UTFString< ch, Allocator>& m_String;
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

		typename Iterator::DataType& operator *();

		bool Valid( void ) const;

		bool operator ==( const ReverseIterator_tpl< Iterator >& rhs ) const;
		bool operator !=( const ReverseIterator_tpl< Iterator >& rhs ) const;
		bool operator <=( const ReverseIterator_tpl< Iterator >& rhs ) const;
		bool operator <( const ReverseIterator_tpl< Iterator >& rhs ) const;
		bool operator >=( const ReverseIterator_tpl< Iterator >& rhs ) const;
		bool operator >( const ReverseIterator_tpl< Iterator >& rhs ) const;

		ReverseIterator_tpl< Iterator >& operator ++( void );
		ReverseIterator_tpl< Iterator > operator ++( int ) const;


		ReverseIterator_tpl< Iterator >& operator --( void );
		ReverseIterator_tpl< Iterator > operator --( int ) const;

	protected:
	private:

		Iterator m_It;
	};


	/**
	\brief Class representing a valid UTF-8 string.

	\c ch Is the type used for a single byte ( not a full UTF-8 char! ). 
	The default is unsigned char which is ok for the most compilers.
	You might want to change this parameter if you prefer to store the UTF-8 String in a specific way.

	\c Allocator Is the class used to allocate the memory for the string
	
	*/
	template < typename ch = unsigned char, typename Allocator = ::uti::DefaultAllocator >
	class UTFString
	{
	public:

		typedef const ch ConstType;
		typedef ch Type;

		typedef typename ::uti::UTFStringIterator< ch, Allocator > Iterator;
		typedef typename ::uti::ReverseIterator_tpl< typename Iterator > ReverseIterator;

		UTFString( void );

		UTFString( const ch* text );

		~UTFString();

		
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
		\brief Returns if the UTF-String is empty or not
		
		\return \c true if the string is empty or \c false if not
		*/
		bool Empty( void ) const;

		bool operator ==( const UTFString& rhs ) const;
		bool operator !=( const UTFString& rhs ) const;

		Iterator Begin( void ) const;
		Iterator End( void ) const;

		ReverseIterator rBegin( void ) const;
		ReverseIterator rEnd( void ) const;

		/**
		\brief Checks if the given Byte is in range of a valid UTF-8 Byte.
		This does not necessarily makes an array of valid Bytes a valid UTF-8 char or string.
		
		\param utfchar The char to be tested
		
		\return \c true if the byte is valid, \c false otherwise
		*/
		static inline bool ValidUTF8Byte( const ch* utfchar );


		friend class UTFStringIterator<ch,Allocator>;

	protected: 
	private:

		ch* m_pData;
		Allocator m_Alloc;
		u32 m_uiSize;
	};

	//////////////////////////////////////////////////////////////////////////
	// UTF String Iterator implemenation
	//////////////////////////////////////////////////////////////////////////

	template< typename ch /*= unsigned char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	UTFStringIterator< ch, Allocator> uti::UTFStringIterator<ch, Allocator>::operator--( int ) const
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if ( m_uiPos > 0 )
		{
			UTFStringIterator it( *this );
			return --it;
		}
		else
		{
			throw UTFException( "Iterator not decrementable", 0 );
		}
#else

		UTFStringIterator it( *this );
		return --it;
#endif // _ITERATOR_DEBUG_LEVEL == 2
	}

	template< typename ch /*= unsigned char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	UTFStringIterator< ch, Allocator>& uti::UTFStringIterator<ch, Allocator>::operator--( void )
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if ( m_uiPos > 0 )
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

	template< typename ch /*= unsigned char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	UTFStringIterator<ch, Allocator>& uti::UTFStringIterator<ch, Allocator>::operator=( const UTFStringIterator< ch, Allocator >& it )
	{
		m_String = it.m_String;
		m_uiPos = m_uiPos;
	}

	template< typename ch /*= unsigned char */, typename Allocator /* = ::uit::DefaultAllocator */>
	UTFStringIterator<ch, Allocator> uti::UTFStringIterator<ch, Allocator>::operator++( int ) const
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if ( m_uiPos < m_String.m_uiSize )
		{
			UTFStringIterator it( *this );
			return ++it;
		}
		else
		{
			throw UTFException( "Iterator not incrementable", 0 );
		}
#else

		UTFStringIterator it( *this );
		return ++it;
#endif // _ITERATOR_DEBUG_LEVEL == 2
	}

	template< typename ch /*= unsigned char */, typename Allocator /* = ::uit::DefaultAllocator */>
	UTFStringIterator<ch, Allocator>& uti::UTFStringIterator<ch, Allocator>::operator++( void )
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if ( m_uiPos < m_String.m_uiSize )
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

	template< typename ch /*= unsigned char */, typename Allocator /* = ::uit::DefaultAllocator */>
	bool uti::UTFStringIterator<ch,Allocator>::operator>( const UTFStringIterator<ch,Allocator>& rhs ) const
	{

#if _ITERATOR_DEBUG_LEVEL == 2
		if (m_String.m_pData != rhs.m_String.m_pData)
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

	template< typename ch /*= unsigned char */, typename Allocator /* = ::uit::DefaultAllocator */>
	bool uti::UTFStringIterator<ch,Allocator>::operator>=( const UTFStringIterator<ch,Allocator>& rhs ) const
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if (m_String.m_pData != rhs.m_String.m_pData)
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

	template< typename ch /*= unsigned char */, typename Allocator /* = ::uit::DefaultAllocator */>
	bool uti::UTFStringIterator<ch,Allocator>::operator<( const UTFStringIterator<ch,Allocator>& rhs ) const
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if (m_String.m_pData != rhs.m_String.m_pData)
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

	template< typename ch /*= unsigned char */, typename Allocator /* = ::uit::DefaultAllocator */>
	bool uti::UTFStringIterator<ch,Allocator>::operator<=( const UTFStringIterator<ch,Allocator>& rhs ) const
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if (m_String.m_pData != rhs.m_String.m_pData)
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

	template< typename ch /*= unsigned char */, typename Allocator /* = ::uit::DefaultAllocator */>
	bool uti::UTFStringIterator<ch,Allocator>::operator!=( const UTFStringIterator<ch,Allocator>& rhs ) const
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if (m_String.m_pData != rhs.m_String.m_pData)
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

	template< typename ch /*= unsigned char */, typename Allocator /* = ::uit::DefaultAllocator */>
	bool uti::UTFStringIterator<ch,Allocator>::operator==( const UTFStringIterator<ch,Allocator>& rhs ) const
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if (m_String.m_pData != rhs.m_String.m_pData)
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

	template< typename ch /*= unsigned char */, typename Allocator /* = ::uit::DefaultAllocator */>
	ch& uti::UTFStringIterator< ch, Allocator >::operator*()
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		if ( Valid() )
		{
			return *(m_String.m_pData + m_uiPos);
		}
		else
		{
			throw UTFException( "Iterator not dereferenceable", 0 );
		}
#else
		return *(m_String.m_pData + m_uiPos);
#endif

	}

	template< typename ch /*= unsigned char */, typename Allocator /* = ::uit::DefaultAllocator */>
	bool uti::UTFStringIterator<ch,Allocator>::Valid( void ) const
	{
		return m_uiPos < m_String.m_uiSize;
	}

	template< typename ch /*= unsigned char */, typename Allocator /* = ::uit::DefaultAllocator */>
	uti::UTFStringIterator<ch,Allocator>::~UTFStringIterator()
	{
		m_uiPos = 0;
	}

	template< typename ch /*= unsigned char */, typename Allocator /* = ::uit::DefaultAllocator */>
	uti::UTFStringIterator<ch,Allocator>::UTFStringIterator( typename UTFStringIterator<ch,Allocator>::String& data, u32 m_uiPos ) :
		m_String( data ),
		m_uiPos( m_uiPos )
	{
	}

	template< typename ch /*= unsigned char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	uti::UTFStringIterator<ch, Allocator>::UTFStringIterator( const UTFStringIterator< ch, Allocator >& it ) :
		m_String( it.m_String ),
		m_uiPos( it.m_uiPos )
	{

	}

	//////////////////////////////////////////////////////////////////////////
	// Reverse Iterator Implementation
	//////////////////////////////////////////////////////////////////////////

	template< typename Iterator >
	ReverseIterator_tpl< Iterator > uti::ReverseIterator_tpl<Iterator>::operator--( int ) const
	{
		ReverseIterator_tpl< Iterator > copy(*this);
		return --copy;
	}

	template< typename Iterator >
	ReverseIterator_tpl< Iterator >& uti::ReverseIterator_tpl<Iterator>::operator--( void )
	{
		++m_It;
		return *this;
	}

	template< typename Iterator >
	ReverseIterator_tpl< Iterator > uti::ReverseIterator_tpl<Iterator>::operator++( int ) const
	{

		ReverseIterator_tpl< Iterator > copy(*this);
		return ++copy;
	}

	template< typename Iterator >
	ReverseIterator_tpl< Iterator >& uti::ReverseIterator_tpl<Iterator>::operator++( void )
	{
		--m_It;
		return *this;
	}

	template< typename Iterator >
	bool uti::ReverseIterator_tpl<Iterator>::operator>( const ReverseIterator_tpl< Iterator >& rhs ) const
	{
		return m_It < rhs.m_It;
	}

	template< typename Iterator >
	bool uti::ReverseIterator_tpl<Iterator>::operator>=( const ReverseIterator_tpl< Iterator >& rhs ) const
	{
		return m_It <= rhs.m_It;
	}

	template< typename Iterator >
	bool uti::ReverseIterator_tpl<Iterator>::operator<( const ReverseIterator_tpl< Iterator >& rhs ) const
	{
		return m_It > rhs.m_It;
	}

	template< typename Iterator >
	bool uti::ReverseIterator_tpl<Iterator>::operator<=( const ReverseIterator_tpl< Iterator >& rhs ) const
	{
		return m_It >= ths.m_It;
	}

	template< typename Iterator >
	bool uti::ReverseIterator_tpl<Iterator>::operator!=( const ReverseIterator_tpl< Iterator >& rhs ) const
	{
		return m_It != rhs.m_It;
	}

	template< typename Iterator >
	bool uti::ReverseIterator_tpl<Iterator>::operator==( const ReverseIterator_tpl< Iterator >& rhs ) const
	{
		return m_It == rhs.m_It;
	}

	template< typename Iterator >
	bool uti::ReverseIterator_tpl<Iterator>::Valid( void ) const
	{
		return m_It.Valid();
	}

	template< typename Iterator >
	typename Iterator::DataType& uti::ReverseIterator_tpl<Iterator>::operator*()
	{
		Iterator copy( m_It );
		return *( --copy );
	}

	template< typename Iterator >
	ReverseIterator_tpl< Iterator >& uti::ReverseIterator_tpl<Iterator>::operator=( const Iterator& it )
	{
		m_It = it;
		return *this;
	}

	template< typename Iterator >
	ReverseIterator_tpl< Iterator >& uti::ReverseIterator_tpl<Iterator>::operator=( const ReverseIterator_tpl< Iterator >& it )
	{
		m_It = it.m_It;
		return *this;
	}

	template< typename Iterator >
	uti::ReverseIterator_tpl<Iterator>::ReverseIterator_tpl( const ReverseIterator_tpl< Iterator >& it ) :
		m_It( it.m_It )
	{

	}

	template< typename Iterator >
	uti::ReverseIterator_tpl<Iterator>::ReverseIterator_tpl( const Iterator& it ) :
		m_It( it )
	{

	}

	//////////////////////////////////////////////////////////////////////////
	// UTF String Implementation
	//////////////////////////////////////////////////////////////////////////

	template < typename ch /*= unsigned char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	typename UTFString<ch, Allocator>::ReverseIterator uti::UTFString<ch, Allocator>::rEnd( void ) const
	{
		return ReverseIterator( Begin() );
	}

	template < typename ch /*= unsigned char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	typename UTFString<ch, Allocator>::ReverseIterator uti::UTFString<ch, Allocator>::rBegin( void ) const
	{
		return ReverseIterator( End() );
	}

	template < typename ch /*= unsigned char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	typename UTFString<ch,Allocator>::Iterator uti::UTFString<ch, Allocator>::End( void ) const
	{
		return UTFString<ch,Allocator>::Iterator( (UTFString&)*this, m_uiSize );
	}

	template < typename ch /*= unsigned char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	typename UTFString<ch,Allocator>::Iterator uti::UTFString<ch, Allocator>::Begin( void ) const
	{
		return UTFString<ch,Allocator>::Iterator( (UTFString&)*this, 0U );
	}

	template < typename ch /*= unsigned char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	bool uti::UTFString<ch, Allocator>::operator!=( const UTFString& rhs ) const
	{

		if (m_uiSize != rhs.m_uiSize)
		{
			return true;
		}

		Iterator lStart = Begin();
		Iterator rStart = rhs.Begin();
		Iterator lEnd = End();
		Iterator rEnd = rhs.End();

		while ( lStart != lEnd && rStart != rEnd )
		{
			if (*(lStart++) != *(rStart++))
			{
				return true;
			}
		}

		return false;
	}

	template < typename ch /*= unsigned char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	bool uti::UTFString<ch, Allocator>::operator==( const UTFString& rhs ) const
	{
		if (m_uiSize != rhs.m_uiSize)
		{
			return false;
		}

		Iterator lStart = Begin();
		Iterator rStart = rhs.Begin();
		Iterator lEnd = End();
		Iterator rEnd = rhs.End();

		while ( lStart != lEnd && rStart != rEnd )
		{
			if (*(lStart++) != *(rStart++))
			{
				return false;
			}
		}

		return true;
	}

	template < typename ch /*= unsigned char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	bool uti::UTFString<ch, Allocator>::Empty( void ) const
	{
		return m_pData == nullptr || m_pData[ 0 ] == 0U;
	}

	template < typename ch /*= unsigned char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	const ch* uti::UTFString<ch, Allocator>::c_str() const
	{
		return m_pData;
	}

	template < typename ch /*= unsigned char*/, typename Allocator /*= ::uti::DefaultAllocator */>
	ch* uti::UTFString<ch, Allocator>::Data() const
	{
		return m_pData;
	}

	template < typename ch /*= unsigned char*/, typename Allocator /*= DefaultAllocator */>
	bool UTFString<ch, Allocator>::ValidUTF8Byte( const ch* utfchar )
	{
		if ( utfchar == nullptr)
		{
			return false;
		}
		else
		{
			return *utfchar <= 0xF4 && ( *utfchar < 0xC0 || *utfchar > 0xC1 );
		}
	}

	template < typename ch /*= unsigned char*/, typename Allocator /*= DefaultAllocator */>
	UTFString<ch, Allocator>::UTFString( void )
	{
		m_pData = static_cast< ch* >( m_Alloc.AllocateBytes( 1U ) );

		*m_pData = 0;

		m_uiSize = 1;
	}

	template < typename ch /*= unsigned char*/, typename Allocator /*= DefaultAllocator */>
	UTFString<ch, Allocator>::UTFString( const ch* text )
	{
		u32 size = 0;
		while ( text[ size ] != 0 )
		{
			if (!ValidUTF8Byte( text + size++ ))
			{
				throw InvalidCharException( size - 1U );
			}
		}
		m_pData= static_cast< ch* >( m_Alloc.AllocateBytes( size * sizeof( ch ) ) );

		for (u32 i = 0; i < size; i++)
		{
			m_pData[ i ] = static_cast< ch >( text[ i ] );
		}

		m_uiSize = size;
	}


	template < typename ch /*= unsigned char*/, typename Allocator /*= DefaultAllocator */>
	UTFString<ch, Allocator>::~UTFString()
	{
		if (m_pData)
		{
			m_Alloc.FreeBytes( m_pData );
		}
		m_uiSize = 0;
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

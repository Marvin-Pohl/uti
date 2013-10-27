#pragma once
#include <exception>
#ifndef uti_h__
#define uti_h__

namespace uti
{

	typedef unsigned int u32;

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

		inline InvalidCharException( int position ) :
			UTFException( "Invalid char detected!", position )
		{

		}

	protected:
	private:
	};
	class DefaultAllocator
	{
	public:

		inline void* Allocate( u32 size );

		inline void Deallocate( void* ptr );

	protected:

	private:
	};

	void* DefaultAllocator::Allocate( u32 size )
	{
		return new char[ size ];
	}

	void DefaultAllocator::Deallocate( void* ptr )
	{
		delete ptr;
	}

	template < typename ch = unsigned char, typename Allocator = DefaultAllocator >
	class UTFString
	{
	public:

		UTFString( void );

		UTFString( const ch* text );

		~UTFString();

		static inline bool ValidUTF8Byte( const ch* utfchar );

	protected: 
	private:

		ch* m_pData;
		Allocator m_Alloc;
	};

	template < typename ch /*= unsigned char*/, typename Allocator /*= DefaultAllocator */>
	bool uti::UTFString<ch, Allocator>::ValidUTF8Byte( const ch* utfchar )
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
		m_pData = m_Alloc.Allocate( 1U );

		*m_pData = 0;
	}

	template < typename ch /*= unsigned char*/, typename Allocator /*= DefaultAllocator */>
	UTFString<ch, Allocator>::UTFString( const ch* text )
	{
		u32 size = 0;
		while ( text[ size ] != 0 )
		{
			if (!ValidUTF8Byte( text + size++ ))
			{
				throw InvalidCharException( size - 1 );
			}
		}
		m_pData= static_cast< ch* >( m_Alloc.Allocate( size * sizeof( ch ) ) );

		for (u32 i = 0; i < size; i++)
		{
			m_pData[ i ] = static_cast< ch >( text[ i ] );
		}
	}


	template < typename ch /*= unsigned char*/, typename Allocator /*= DefaultAllocator */>
	UTFString<ch, Allocator>::~UTFString()
	{
		if (m_pData)
		{
			m_Alloc.Deallocate( m_pData );
		}
	}
}

#endif // uti_h__

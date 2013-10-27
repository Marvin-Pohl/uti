#pragma once
#ifndef uti_h__
#define uti_h__

namespace uti
{

	typedef unsigned int u32;

	class DefaultAllocator
	{

		inline void* Allocate( u32 size );

		inline void Deallocate( void* ptr );
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

		UTFString( const char* text );

		~UTFString();

	protected: 
	private:

		ch* m_pData;
		Allocator m_Alloc;
	};

	template < typename ch /*= unsigned char*/, typename Allocator /*= DefaultAllocator */>
	UTFString<ch, Allocator>::UTFString( void ) :
		m_pData( nullptr )
	{

	}

	template < typename ch /*= unsigned char*/, typename Allocator /*= DefaultAllocator */>
	UTFString<ch, Allocator>::UTFString( const char* text )
	{
		u32 size = 0;
		while ( text[size++] != 0 )
		{
		}
		m_pData= static_cast< ch* >( m_Alloc.Allocate( size * sizeof( ch ) ) );

		for (int i = 0; i < size; i++)
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

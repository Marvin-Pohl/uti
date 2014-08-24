#pragma once
#ifndef utiCommonHeader_h__
#define utiCommonHeader_h__
#include <intrin.h>
#include <exception>

namespace uti
{
	typedef unsigned int u32;

	template< bool condition, typename TrueType, typename FalseType >
	struct if_
	{
	};

	template< typename TrueType, typename FalseType >
	struct if_ < true, TrueType, FalseType >
	{
		typedef typename TrueType type;
	};

	template< typename TrueType, typename FalseType >
	struct if_ < false, TrueType, FalseType >
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

	enum class BinaryOrder
	{
		LittleEndian = 0x1,
		BigEndian = 0x2
	};
}

#endif // utiCommonHeader_h__

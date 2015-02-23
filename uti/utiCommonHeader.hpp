#pragma once
#ifndef utiCommonHeader_h__
#define utiCommonHeader_h__
#include <intrin.h>

#define UTI_WINDOWS 1
#define UTI_PLATFORM UTI_WINDOWS

#ifdef UTI_WINDOWS
#include <windows.h>
#endif // UTI_WINDOWS


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

	/**
	@brief Calls the corresponding debug break function for reach platform

	*/
	inline void DebugBreak()
	{
#if UTI_PLATFORM == UTI_WINDOWS
		::DebugBreak();
#endif // UTI_PLATFORM
	}

	/**
	@brief Similar to assert, but does not check if the expression is valid, just immediately stops the program

	*/
	void UtiFatal( const char* message );
#ifndef UTI_CUSTOM_FATAL


	inline void UtiFatal( const char* message )
	{
		printf( "Fatal Error: %s\n", message );
		::uti::DebugBreak();
	}
#endif // UTI_CUSTOM_FATAL


// Implements UTI_ASSERT when in debug
#if defined(_DEBUG) || defined(DEBUG)
//////////////////////////////////////////////////////////////////////////
// Debug Mode
//////////////////////////////////////////////////////////////////////////
#ifdef assert // Use the default assert if there is one defined
#define UTI_ASSERT( ... ) assert( __VA_ARGS__ )
#else // Otherwise implement our own
#define UTI_ASSERT( ... ) \
	do\
		{\
		auto result = __VA_ARGS__;\
		if( result == false )\
		{\
			printf("Assertion failed: " #__VA_ARGS__ "\n" );\
			::uti::DebugBreak();\
		}\
	} while( 0 )

#endif // assert

#define UTI_FATAL UtiFatal


#else // No assert in release mode
//////////////////////////////////////////////////////////////////////////
// Release Mode
//////////////////////////////////////////////////////////////////////////
#define UTI_ASSERT(...)
#define UTI_FATAL( ... )
#endif



	enum class BinaryOrder
	{
		LittleEndian = 0x1,
		BigEndian = 0x2
	};
}

#endif // utiCommonHeader_h__

#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\..\uti.hpp"
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

template __declspec( dllexport ) class uti::UTF16String< wchar_t, ::uti::BinaryOrder::LittleEndian>;
template __declspec( dllexport ) class uti::UTF16String< short, ::uti::BinaryOrder::BigEndian >;

typedef uti::UTF16String< wchar_t > String16LE;
typedef uti::UTF16String< wchar_t, ::uti::BinaryOrder::BigEndian > String16BE;

typedef String16LE String;

//template<> std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString( String* str )
//{
//	RETURN_WIDE_STRING( str->c_str() );
//}
template<> std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString( const String& str )
{
	RETURN_WIDE_STRING( str.c_str() );
}
//template<> std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString(const String* str )
//{
//	RETURN_WIDE_STRING( str->c_str() );
//}

namespace utiTest
{
	TEST_CLASS( UTF16Test )
	{
	public:

		TEST_METHOD( CTorTest )
		{

			try
			{
				String::ReplacementChar = L"|";
				const wchar_t blah [] = { 0xDD00u, 0xD800u, 0x00 };
				String string( blah );

				Assert::AreEqual( L'|', string.c_str()[ 0 ] );

				const wchar_t excpected [] = L"||";

				String test( excpected );

				Assert::AreEqual( test, string );
			}
			catch( ... )
			{
				Assert::Fail();
			}


			try
			{
				const wchar_t blah [] = L"Some Test";

				String string( blah );

				Assert::AreEqual( 9U, string.CharCount() );
				Assert::AreEqual( 18U, string.Size() );

			}
			catch( ... )
			{
				Assert::Fail();
			}

			try
			{
				const wchar_t test [] = L"Some Test";

				String string1 = test;
				String string2 = string1;

				String string3;
				string3 = string2;

				Assert::IsTrue( string1 == string2 );
				Assert::IsTrue( string2 == string3 );
				unsigned int iterations = 0;
				for( auto it = string3.Begin(); it != string3.End(); ++it, ++iterations )
				{
					Assert::AreEqual( *it, test[ iterations ] );

				}

				Assert::AreEqual( 9U, iterations );
			}
			catch( ... )
			{
				Assert::Fail();
			}

		}

		TEST_METHOD( CTorFailTest )
		{
			try
			{
				String string( ( const wchar_t* )nullptr );
				
				String string2 = string;
				
				String string3 = string + string2;
				
				Assert::IsTrue( string.Size() == 0 );
				Assert::IsTrue( string.CharCount() == 0 );
				Assert::IsTrue( string2.Size() == 0 );
				Assert::IsTrue( string2.CharCount() == 0 );
				Assert::IsTrue( string3.Size() == 0 );
				Assert::IsTrue( string3.CharCount() == 0 );

			}
			catch( ... )
			{
				Assert::Fail( L"Nullptr crashed the string constructor" );
			}
		}

		TEST_METHOD( IteratorTest )
		{
			String bla( L"Test" );
			const wchar_t test [] = L"Test";
			unsigned int iterations = 0;
			auto it = bla.Begin();

#ifdef DEBUG


			try
			{
				it--;

				Assert::Fail();
			}
			catch( uti::UTFException& )
			{

			}
#endif // DEBUG

			for( ; it != bla.End(); ++it, ++iterations )
			{
				Assert::AreEqual( *it, test[ iterations ] );

			}

			Assert::AreEqual( 4U, iterations );

#ifdef DEBUG
			try
			{
				it++;

				Assert::Fail();
			}
			catch( uti::UTFException& )
			{

			}
#endif // DEBUG

		}

		TEST_METHOD( ReverseIterator )
		{
			String bla( L"Test" );
			const wchar_t* test = L"Test";
			unsigned int iterations = 0;
			auto it = bla.rBegin();

#ifdef DEBUG
			try
			{
				it--;

				Assert::Fail();
			}
			catch( uti::UTFException& )
			{

			}
#endif // DEBUG


			for( ; it != bla.rEnd(); ++it, ++iterations )
			{
				Assert::AreEqual( test[ 3U - iterations ], *it );
			}

			Assert::AreEqual( 4U, iterations );

#ifdef DEBUG
			try
			{
				it++;

				Assert::Fail();
			}
			catch( uti::UTFException& )
			{

			}
#endif // DEBUG


		}

		TEST_METHOD( ConcatTest )
		{
			String first( L"Some" );
			String second( L"Test" );
			String expected( L"SomeTest" );
			Assert::IsTrue( ( first + second ) == expected,
				( ToString( first + second ) + ToString( " does not match " ) + ToString( expected ) ).c_str() );
		}

		TEST_METHOD( CompleteCodePointTest )
		{
			
		}

	};
}
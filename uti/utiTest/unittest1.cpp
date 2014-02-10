#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\..\uti.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

template __declspec( dllexport ) class uti::UTFString< >;

typedef uti::UTFString< > String;

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
	TEST_CLASS( UTF8Test )
	{
	public:

		TEST_METHOD( CTorTest )
		{

			try
			{
				String::ReplacementChar = "|";
				const char blah [] = { 0xFFu, 0xC0u, 0x00 };
				String string( blah );

				Assert::AreEqual( '|', string.c_str()[ 0 ] );

				const char excpected [] = "||";

				String test( excpected );

				Assert::AreEqual( test, string );
			}
			catch( ... )
			{
				Assert::Fail();
			}


			try
			{
				const char blah [] = "Some Test";

				String string( blah );

			}
			catch( ... )
			{
				Assert::Fail();
			}

			try
			{
				const char test [] = "Some Test";

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

		TEST_METHOD( IteratorTest )
		{
			String bla( "Test" );
			const char test [] = "Test";
			unsigned int iterations = 0;
			for( auto it = bla.Begin(); it != bla.End(); ++it, ++iterations )
			{
				Assert::AreEqual( *it, test[ iterations ] );

			}

			Assert::AreEqual( 4U, iterations );
		}

		TEST_METHOD( ReverseIterator )
		{
			String bla( "Test" );
			const char* test = "Test";
			unsigned int iterations = 0;
			for( auto it = bla.rBegin(); it != bla.rEnd(); ++it, ++iterations )
			{
				Assert::AreEqual( test[ 3U - iterations ], *it );

			}

			Assert::AreEqual( 4U, iterations );
		}

		TEST_METHOD( ConcatTest )
		{
			String first( "Some" );
			String second( "Test" );
			String expected( "SomeTest" );

			//Assert::IsTrue( first == String( "first" ) );
			Assert::IsTrue( ( first + second ) == expected, ( ToString( first + second ) + ToString( " does not match " ) + ToString( expected ) ).c_str() );
		}

		TEST_METHOD( ValidityTest )
		{
			char surrogate [] = { 0xd8U, 0x00U };

			Assert::AreEqual( String::ValidUTF8Char( surrogate ), 0U );
		}

	};
}
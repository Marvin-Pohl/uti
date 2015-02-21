#include "stdafx.h"
#include "CppUnitTest.h"

bool AssertTriggered = false;
#define RESET_ASSERT AssertTriggered = false
#ifdef assert
#undef assert
#endif // assert
#define assert ( ... ) AssertTriggered = true
#define FAIL_ON_ASSERT if(AssertTriggered) Assert::Fail()
#define FAIL_ON_NO_ASSERT if(!AssertTriggered) Assert::Fail()

#define UTI_CUSTOM_FATAL( ... ) AssertTriggered = true

#include "..\uti.hpp"
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

template __declspec( dllexport ) class uti::UTF8String < > ;

typedef uti::UTF8String< > String;

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
			{
				RESET_ASSERT;
				String::ReplacementChar = "|";
				const char blah [] = { 0xFFu, 0xC0u, 0x00 };
				String string( blah );

				Assert::AreEqual( '|', string.c_str()[ 0 ] );

				const char excpected [] = "||";

				String test( excpected );

				Assert::AreEqual( test, string );
				FAIL_ON_ASSERT;
			}
			{
				RESET_ASSERT;
				const char blah [] = "Some Test";

				String string( blah );

				Assert::AreEqual( 9U, string.CharCount() );
				Assert::AreEqual( 9U, string.Size() );

				FAIL_ON_ASSERT;
			}
			{
				RESET_ASSERT;
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
				FAIL_ON_ASSERT;
			}
		}

		TEST_METHOD( CTorFailTest )
		{
			RESET_ASSERT;
			String string( ( const char* )nullptr );

			String string2 = string;

			String string3 = string + string2;

			Assert::IsTrue( string.Size() == 0 );
			Assert::IsTrue( string.CharCount() == 0 );
			Assert::IsTrue( string2.Size() == 0 );
			Assert::IsTrue( string2.CharCount() == 0 );
			Assert::IsTrue( string3.Size() == 0 );
			Assert::IsTrue( string3.CharCount() == 0 );
			FAIL_ON_ASSERT;
		}

		TEST_METHOD( IteratorTest )
		{
			String bla( "Test" );
			const char test [] = "Test";
			unsigned int iterations = 0;
			auto it = bla.Begin();

#ifdef _DEBUG


			RESET_ASSERT;
			it--;

			FAIL_ON_NO_ASSERT;
#endif // DEBUG

			for( ; it != bla.End(); ++it, ++iterations )
			{
				Assert::AreEqual( *it, test[ iterations ] );

			}

			Assert::AreEqual( 4U, iterations );

#ifdef _DEBUG

			RESET_ASSERT;
			it++;

			FAIL_ON_NO_ASSERT;

#endif // DEBUG

		}

		TEST_METHOD( ReverseIterator )
		{
			String bla( "Test" );
			const char* test = "Test";
			unsigned int iterations = 0;
			auto it = bla.rBegin();

#ifdef _DEBUG

			RESET_ASSERT;
			it--;
			FAIL_ON_NO_ASSERT;

#endif // DEBUG


			for( ; it != bla.rEnd(); ++it, ++iterations )
			{
				Assert::AreEqual( test[ 3U - iterations ], *it );
			}

			Assert::AreEqual( 4U, iterations );

#ifdef _DEBUG
			RESET_ASSERT;
			it++;

			FAIL_ON_NO_ASSERT;
#endif // DEBUG


		}

		TEST_METHOD( ConcatTest )
		{
			String first( "Some" );
			String second( "Test" );
			String expected( "SomeTest" );
			Assert::IsTrue( ( first + second ) == expected,
				( ToString( first + second ) + ToString( " does not match " ) + ToString( expected ) ).c_str() );
		}

		TEST_METHOD( ValidityTest )
		{
			char surrogate [] = { 0xd8U, 0x00U };

			Assert::AreEqual( String::ValidChar( surrogate ), 0U );
		}

		TEST_METHOD( WCharTest )
		{
			String myString = String::FromUTF16LE( L"" );
			Assert::AreEqual( 0U, myString.CharCount(), L"Converting empty widechar didn't create an empty UTf string!" );
			Assert::AreEqual( '\0', myString.Data()[ 0 ], L"Converting empty widechar didn't has an null terminated character!" );

			myString = String::FromUTF16LE( L"WideChar" );
			String myNotWideString( "WideChar" );

			Assert::AreEqual( myNotWideString.Size(), myString.Size(), L"Converting empty widechar didn't produces correct size" );
			Assert::AreEqual( myNotWideString.CharCount(), myString.CharCount(), L"Converting empty widechar didn't produces correct charCount" );

			Assert::AreEqual( myNotWideString, myString, L"Creating the same ascii text in ascii and unicode did not match" );

		}

		TEST_METHOD( CompleteCodePointTest )
		{
			std::ifstream file;
			file.open( "../test.txt", std::ios::binary );

			if( !file.is_open() )
			{
				Assert::Fail( L"Could not open File 'test.txt'" );
			}
			else
			{
				file.seekg( 0, std::ios::end );

				size_t fileSize = ( size_t ) file.tellg().seekpos();

				file.seekg( 0, std::ios::beg );

				char* content = new char[ fileSize + 1 ];
				content[ fileSize ] = '\0';

				size_t pos = 0;

				do
				{
					file.read( content + pos, 1000 );
					pos += ( size_t ) file.gcount();
				} while( !file.eof() && file.gcount() != 0 );

				if( pos < 4416047U ) //Exact Byte size of test.txt
				{
					Assert::Fail( (
						std::wstring( L"Failed to read " ) +
						std::to_wstring( fileSize ) +
						std::wstring( L" Bytes of data, could only read " ) +
						std::to_wstring( pos ) + std::wstring( L" Bytes!" )
						).c_str() );
					return;
				}

				file.close();

				size_t charCount = 0;

				for( size_t i = 0; i < fileSize; )
				{
					size_t length = String::ValidChar( content + i );
					if( length == 0 )
					{
						Assert::Fail( ( std::wstring( L"Invalid Char at position " ) + std::to_wstring( i ) ).c_str() );
						break;
					}
					else
					{
						i += length;
						charCount++;
					}

				}

				String completeUTF8( content );
				Assert::AreEqual( fileSize, completeUTF8.Size() );

				//TODO check CharCount as soon as its available;

				delete content;
			}
		}

		TEST_METHOD( SubstrTestOneParam )
		{
			String string( "Hello World" );
			String expected( "Hello" );

			auto it = string.CharBegin();

			for( size_t i = 0; i < 5; i++ )
			{
				++it;
			}

			String sub = string.Substr( it );


			Assert::AreNotEqual( string, sub, L"Substring does match original string!" );
			Assert::AreEqual( 5U, sub.CharCount(), L"Char Size of substring does not match!" );
			Assert::AreEqual( 5U, sub.Size(), L"Byte Size of substring does not match!" );
			Assert::AreEqual( expected, sub, L"Substring does not match expected string." );
		}

		TEST_METHOD( SubstrTestTwoParam )
		{
			String string( "Hello World" );
			String expected( "Wo" );

			auto beginIterator = string.CharBegin();

			for( size_t i = 0; i < 6; i++ )
			{
				++beginIterator;
			}

			auto endIterator = beginIterator;
			for( size_t i = 0; i < 2; i++ )
			{
				++endIterator;
			}

			String sub = string.Substr( beginIterator, endIterator );


			Assert::AreNotEqual( string, sub, L"Substring does match original string!" );
			Assert::AreEqual( 2U, sub.CharCount(), L"Char Size of substring does not match!" );
			Assert::AreEqual( 2U, sub.Size(), L"Byte Size of substring does not match!" );
			Assert::IsTrue( sub.Data()[ 0 ] == 'W' );
			Assert::IsTrue( sub.Data()[ 1 ] == 'o' );
			Assert::AreEqual( expected, sub, L"Substring does not match expected string." );
		}

		TEST_METHOD( SubstrFailTest )
		{

			String str1( "FirstString" );
			String str2( "SecondString" );

			auto beginIterator = str2.CharBegin();
			auto endIterator = str2.CharEnd();

			// Using wrong iterator from other string should return empty string
			String sub = str1.Substr( beginIterator, endIterator );

			Assert::AreEqual( 0U, sub.Size(), L"String with wrong iterator does not return empty string" );
			Assert::AreNotEqual( str1, sub, L"Substring does match original string!" );
			Assert::AreNotEqual( str2, sub, L"Substring does match second string!" );

			// Using start Iterator which is greater than the end iterator should return empty string
			sub = str1.Substr( str1.CharEnd(), str1.CharBegin() );

			Assert::AreEqual( 0U, sub.Size(), L"String with wrong iterator does not return empty string" );
			Assert::AreNotEqual( str1, sub, L"Substring does match original string!" );
			Assert::AreNotEqual( str2, sub, L"Substring does match second string!" );

		}

	};
}

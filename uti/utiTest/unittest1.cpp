#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\..\uti.hpp"
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

template __declspec( dllexport ) class uti::UTF8String< >;

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

				size_t fileSize = (size_t)file.tellg().seekpos();

				file.seekg( 0, std::ios::beg );

				char* content = new char[ fileSize + 1 ];
				content[ fileSize ] = '\0';

				size_t pos = 0;

				do 
				{
					file.read( content + pos, 1000 );
					pos += (size_t)file.gcount();
				 }while( !file.eof() && file.gcount() != 0 );

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
					size_t length = String::ValidUTF8Char( content + i );
					if( length == 0 )
					{
						Assert::Fail( (std::wstring( L"Invalid Char at position " ) + std::to_wstring( i )).c_str() );
						break;
					}
					else
					{
						i += length;
						charCount++;
					}

				}

				String completeUTF8( content );
				Assert::AreEqual( fileSize - 1, completeUTF8.Size() );

				//TODO check CharCount as soon as its available;

				delete content;
			}
		}

	};
}
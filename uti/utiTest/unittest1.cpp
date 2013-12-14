#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\..\uti.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

template __declspec( dllexport ) class uti::UTFString<>;

typedef uti::UTFString<> String;

namespace utiTest
{		
	TEST_CLASS(UTF8Test)
	{
	public:
		
		TEST_METHOD(CTorTest)
		{
			
			try
			{
				const unsigned char blah[] = { 0xFFu, 0xC0 };
				uti::UTFString<> string( blah );

				Assert::Fail();
			}
			catch (uti::InvalidCharException&)
			{
			}
			catch( ... )
			{
				Assert::Fail();
			}


			try
			{
				const unsigned char blah[] = "Some Test";

				uti::UTFString<> string(blah);

			}
			catch( ... )
			{
				Assert::Fail();
			}

			try
			{
				const unsigned char test [] = "Some Test";

				String string1 = test;
				String string2 = string1;

				String string3;
				string3 = string2;

				Assert::IsTrue( string1 == string2 );
				Assert::IsTrue( string2 == string3 );
				unsigned int iterations = 0;
				for( auto it = string3.Begin(); it != string3.End(); ++it, ++iterations )
				{
					Assert::AreEqual( *it, ( unsigned char ) test[ iterations ] );

				}

				Assert::AreEqual( 9U,  iterations );
			}
			catch( ... )
			{
				Assert::Fail();
			}

		}

		TEST_METHOD(IteratorTest)
		{
			String bla((unsigned char*)"Test");
			const unsigned char test[] = "Test";
			unsigned int iterations = 0;
			for (auto it = bla.Begin(); it != bla.End(); ++it, ++iterations )
			{
				Assert::AreEqual( *it, (unsigned char)test[iterations] );

			}

			Assert::AreEqual( 4U, iterations );
		}

		TEST_METHOD(ReverseIterator)
		{
			String bla((unsigned char*)"Test");
			const char* test = "Test";
			unsigned int iterations = 0;
			for (auto it = bla.rBegin(); it != bla.rEnd(); ++it, ++iterations )
			{
				Assert::AreEqual((unsigned char)test[3U-iterations], *it );

			}

			Assert::AreEqual( 4U, iterations );
		}

	};
}
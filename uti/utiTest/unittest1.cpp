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
				const unsigned char blah[] = { 0xFF, 0xC0 };
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

		}

	};
}
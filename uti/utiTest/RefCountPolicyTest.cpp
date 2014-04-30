#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../uti.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace utiTest
{
	struct TestRefCountPolicy
	{
		static uti::u32 s_numDestroyCalls;
		static uti::u32 s_numIncRefCalls;
		static uti::u32 s_numDecRefCalls;

		inline static void Destroy(uti::IAllocator*, void*, uti::u32*)
		{
			++s_numDestroyCalls;
		}

		inline static void IncRef(uti::u32& count)
		{
			++s_numIncRefCalls;
			++count;
		}

		inline static void DecRef(uti::u32& count)
		{
			++s_numDecRefCalls;
			--count;
		}

		inline static void Reset()
		{
			s_numDestroyCalls = 0U;
			s_numIncRefCalls  = 0U;
			s_numDecRefCalls  = 0U;
		}
	};

	uti::u32 TestRefCountPolicy::s_numDestroyCalls = 0U;
	uti::u32 TestRefCountPolicy::s_numIncRefCalls  = 0U;
	uti::u32 TestRefCountPolicy::s_numDecRefCalls  = 0U;

	TEST_CLASS(RefCountPolicyTest)
	{
	public:
		
		TEST_METHOD(CustomPolicy)
		{
			typedef uti::ReferenceCounted<uti::u32, uti::DefaultAllocator, TestRefCountPolicy> RefCountedTestType;

			TestRefCountPolicy::Reset();

			Assert::AreEqual(0U, TestRefCountPolicy::s_numDestroyCalls, L"TestRefCountPolicy::Reset is broken!");
			Assert::AreEqual(0U, TestRefCountPolicy::s_numIncRefCalls, L"TestRefCountPolicy::Reset is broken!");
			Assert::AreEqual(0U, TestRefCountPolicy::s_numDecRefCalls, L"TestRefCountPolicy::Reset is broken!");

			uti::u32 testData(42U);

			Assert::AreEqual(0U, TestRefCountPolicy::s_numDestroyCalls, L"Well, it seems your compiler is broken or there's too much radiation in your area...");
			Assert::AreEqual(0U, TestRefCountPolicy::s_numIncRefCalls,  L"Well, it seems your compiler is broken or there's too much radiation in your area...");
			Assert::AreEqual(0U, TestRefCountPolicy::s_numDecRefCalls,  L"Well, it seems your compiler is broken or there's too much radiation in your area...");

			{
				RefCountedTestType refCountedTestData(&testData);

				Assert::AreEqual(0U, TestRefCountPolicy::s_numDestroyCalls);
				Assert::AreEqual(1U, TestRefCountPolicy::s_numIncRefCalls);
				Assert::AreEqual(0U, TestRefCountPolicy::s_numDecRefCalls);
				Assert::AreEqual(42U, *refCountedTestData,  L"Referenced object is not correct!");

				RefCountedTestType refCountedTestData2(refCountedTestData);

				Assert::AreEqual(0U, TestRefCountPolicy::s_numDestroyCalls);
				Assert::AreEqual(2U, TestRefCountPolicy::s_numIncRefCalls);
				Assert::AreEqual(0U, TestRefCountPolicy::s_numDecRefCalls);
				Assert::AreEqual(42U, *refCountedTestData2,  L"Referenced object after copying is not correct!");
			}

			Assert::AreEqual(1U, TestRefCountPolicy::s_numDestroyCalls);
			Assert::AreEqual(2U, TestRefCountPolicy::s_numIncRefCalls);
			Assert::AreEqual(2U, TestRefCountPolicy::s_numDecRefCalls);
		}
	};
}

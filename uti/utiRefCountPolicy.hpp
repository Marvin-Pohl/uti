
#pragma once
#ifndef utiAllocatorPolicy_h__
#define utiAllocatorPolicy_h__

namespace uti
{



	struct NoRefCountPolicy
	{
		inline static void DecRef( u32&  )
		{
		}

		inline static void IncRef( u32&  )
		{
		}

		inline static void Destroy( IAllocator* , void* , u32* )
		{
		}
	};

	struct DefaultRefCountPolicy
	{
		inline static void DecRef( u32& count )
		{
			--count;
		}

		inline static void IncRef( u32& count )
		{
			++count;
		}

		inline static void Destroy( IAllocator* pAllocator, void* pCounted, u32* pCount )
		{
			pAllocator->FreeBytes( pCounted );
			pAllocator->FreeBytes( pCount );
		}
	};

}
#endif // utiAllocatorPolicy_h__


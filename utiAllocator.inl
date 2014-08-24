#pragma once
#ifndef utiAllocator_inl__
#define utiAllocator_inl__

namespace uti
{

	//////////////////////////////////////////////////////////////////////////
	// Default Allocator implementation
	//////////////////////////////////////////////////////////////////////////

	void* DefaultAllocator::AllocateBytes( u32 size )
	{
		return new char[ size ];
	}

	void DefaultAllocator::FreeBytes( void* ptr )
	{
		delete ptr;
	}

}
#endif // utiAllocator_inl__

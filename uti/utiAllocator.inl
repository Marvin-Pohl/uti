#pragma once
#ifndef utiAllocator_inl__
#define utiAllocator_inl__

namespace uti
{

	//////////////////////////////////////////////////////////////////////////
	// Default Allocator implementation
	//////////////////////////////////////////////////////////////////////////

	void* DefaultAllocator::AllocateBytes( u32 size ) const
	{
		return new char[ size ];
	}

	void DefaultAllocator::FreeBytes( void* ptr ) const
	{
		delete ptr;
	}

}
#endif // utiAllocator_inl__

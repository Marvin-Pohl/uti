#pragma once
#ifndef utiAllocator_h__
#define utiAllocator_h__

namespace uti
{

	/**
	\brief Basic Interface for any custom Allocator for the UTFString class.
	To use a custom Allocator write a subclass of IAllocator and pass it as second parameter of the UTFString template arguments.

	The Allocator is intended to work like a proxy allocator, which means it should be lightweight,
	because any instance of the UTFString contains a copy of the Allocator.
	Therefore it is recommended to access the actual Allocator using a static or global definition.

	*/
	class IAllocator
	{
	public:

		virtual void* AllocateBytes( u32 count ) = 0;

		virtual void FreeBytes( void* ptr ) = 0;

	protected:
	private:
	};

	/**
	\brief Default implementation for the UTFString allocator,
	which utilizes new and delete directly to allocate and deallocate Memory.
	*/
	class DefaultAllocator : public IAllocator
	{
	public:

		inline virtual void* AllocateBytes( u32 size );

		inline virtual void FreeBytes( void* ptr );

	protected:

	private:
	};


}

#endif // utiAllocator_h__

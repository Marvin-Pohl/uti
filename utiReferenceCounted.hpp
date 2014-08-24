#pragma once
#ifndef utiReferenceCounted_h__
#define utiReferenceCounted_h__

namespace uti
{
	/**
	\brief Reference Counter class for copy on write functionality of the UTFString.

	*/
	template< typename T, typename Allocator >
	class ReferenceCounted
	{
	public:
		ReferenceCounted( void );
		ReferenceCounted( T* pointer );
		ReferenceCounted( const ReferenceCounted< T, Allocator > & refCount );
		ReferenceCounted( ReferenceCounted< T, Allocator >&& refCount );
		~ReferenceCounted();


		ReferenceCounted< T, Allocator >& operator =( const ReferenceCounted< T, Allocator >& refCount );
		ReferenceCounted< T, Allocator >& operator =( ReferenceCounted< T, Allocator >&& refCount );

		bool operator ==( const ReferenceCounted< T, Allocator >& rhs ) const;
		bool operator !=( const ReferenceCounted< T, Allocator >& rhs ) const;

		T* operator ->( );

		T* operator *( );

		T* Ptr( void ) const;

		T& operator []( u32 idx );
		const T& operator []( u32 idx ) const;

		void SetNull( void );

		bool Valid( void ) const;

		bool Null( void ) const;

		u32 Count( void ) const;

	private:

		void IncRef( void );

		void DecRef( void );

		T* m_CountedPointer;

		u32* m_Count;

		Allocator m_Alloc;

	};
}


#endif // utiReferenceCounted_h__

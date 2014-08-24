#pragma once
#ifndef utiCharIterator_h__
#define utiCharIterator_h__

namespace uti
{
	/**
	\brief Generic Iterator which iterates over a generic String defined in this header file.
	Each iteration step increases in the atomic size of the underlying String ( e.g. 1 Byte in UTF-8 and 2 bytes in UTF-16 )

	\tparam StringType The underlying String class type, over which the iterator is going to iterate.

	*/
	template< typename StringType >
	class UTFCharIterator
	{
	public:

		typedef typename StringType String;

		typedef typename StringType::Type DataType;
		typedef typename StringType::AllocatorType Allocator;

		/**
		\brief Creates a new iterator for the UTF-String

		\param data the data of the string
		\param m_uiPos The m_uiPos where to start the iteration
		*/
		UTFCharIterator( String& str, u32 m_uiPos );

		UTFCharIterator( const UTFCharIterator< String >& it );

		UTFCharIterator& operator =( const UTFCharIterator< String >& it );

		DataType* operator *( void );

		bool Valid( void ) const;

		bool operator ==( const UTFCharIterator< String >& rhs ) const;
		bool operator !=( const UTFCharIterator< String >& rhs ) const;
		bool operator <=( const UTFCharIterator< String >& rhs ) const;
		bool operator <( const UTFCharIterator< String >& rhs ) const;
		bool operator >=( const UTFCharIterator< String >& rhs ) const;
		bool operator >( const UTFCharIterator< String >& rhs ) const;

		UTFCharIterator< String >& operator ++( void );
		UTFCharIterator< String > operator ++( int );


		UTFCharIterator< String >& operator --( void );
		UTFCharIterator< String > operator --( int );

		~UTFCharIterator();


	protected:
	private:

		String& m_String;
		u32 m_uiPos;
	};
}
#endif // utiCharIterator_h__

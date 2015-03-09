#pragma once
#ifndef utiChar_h__
#define utiChar_h__

#include "utiCommonHeader.hpp"

namespace uti
{
	template < class StringType >
	class UTFChar
	{
	public:

		typedef typename StringType::TypePtr TypePtr;
		typedef typename StringType::ConstTypePtr ConstTypePtr;

		/**
		\brief Creates an empty char wrapper, which is not initialized and should not be used directly.
		Notice that IsValid might return true, even if it is actually not a valid char
		
		This is usually used by any kind of container or by pre-allocations to allocate the needed memory, 
		before having the data available.
		
		*/
		UTFChar();

		/**
		\brief Creates a new empty char wrapper, forcing the initialization of the UTFChar to a nullptr.

		Calling IsValid() on this created instance will return false.
		
		\param EForceInit Dummy parameter indication forced initialization
		*/
		UTFChar( EForceInit );

		/**
		\brief Creates a new char wrapper identifying a single char inside an UTFString.

		\param ConstTypePtr address The pointer address to the char which will be wrapped
		\param u32 size Optional size if it is currently known, if not provided or 0 is provided the string
		implementation will be used to identify the size of the char.
		*/
		UTFChar( ConstTypePtr address, u32 size = 0 );

		/**
		@brief Returns the address to the Char.

		This is usually never null unless it is explicitly instantiated using a nullptr.
		
		*/
		ConstTypePtr Pointer() const;

		/**
		\brief Returns the size (in bytes) of the Char saved using this object.
		
		\return uti::u32
		*/
		u32 Size() const;

		// Check for equality of chars at byte level.
		bool operator ==( const UTFChar< StringType >& rhs ) const;
		
		/**
		@brief Checks for inequality of two Chars, returns true if any byte of the chars do not match the other char,
		the length of the chars do not match or one char is invalid
		
		@param const UTFChar< StringType > & rhs
		@return bool
		*/
		bool operator !=( const UTFChar< StringType >& rhs ) const;

		/**
		@brief Checks if th wrapper actually wraps a char.
		
		@return bool \c true if the address wraps a char, \c false otherwise.
		*/
		bool IsValid() const;

	protected:
	private:
		ConstTypePtr m_Address;
		u32 m_Size;
	};


}

#endif // utiChar_h__

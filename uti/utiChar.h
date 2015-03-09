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

	protected:
	private:
		ConstTypePtr m_Address;
		u32 m_Size;
	};
}

#endif // utiChar_h__

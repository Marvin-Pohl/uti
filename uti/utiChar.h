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

		UTFChar( ConstTypePtr address, u32 size = 0 );

	protected:
	private:
		ConstTypePtr m_Address;
		u32 m_Size;
	};
}

#endif // utiChar_h__

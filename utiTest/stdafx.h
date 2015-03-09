// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// Headers for CppUnitTest
#include "CppUnitTest.h"

#define UTI_CUSTOM_FATAL
#include "../uti.hpp"

extern bool AssertTriggered;

namespace uti
{
	inline void UtiFatal( const char*)
	{
		AssertTriggered = true;
	}
}

// TODO: reference additional headers your program requires here

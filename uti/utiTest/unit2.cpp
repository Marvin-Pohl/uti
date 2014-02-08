#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\..\uti.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

template __declspec( dllexport ) class uti::UTFString<>;

typedef uti::UTFString<> String;

namespace utiTest
{

}
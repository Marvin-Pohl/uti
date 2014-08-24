#pragma once
#ifndef uti_h__
#define uti_h__

#ifdef DISABLE_CPP11
#ifdef nullptr
#define OLD_NULLPTR nullptr
#undef nullptr
#endif // nullptr
#define nullptr NULL
#endif // DISABLE_CPP11

#include "utiCommonHeader.hpp"
#include "utiAllocator.hpp"
#include "utiReferenceCounted.hpp"
#include "utiByteIterator.hpp"
#include "utiCharIterator.hpp"
#include "utiReverseIterator.hpp"
#include "utiUTF8String.hpp"
#include "utiUTF16String.hpp"

#include "utiAllocator.inl"
#include "utiReferenceCounted.inl"
#include "utiByteIterator.inl"
#include "utiCharIterator.inl"
#include "utiReverseIterator.inl"
#include "utiUTF8String.inl"
#include "utiUTF16String.inl"


#ifdef DISABLE_CPP11
#undef nullptr
#ifdef OLD_NULLPTR
#define nullptr OLD_NULLPTR
#undef OLD_NULLPTR
#endif // OLD_NULLPTR

#endif // DISABLE_CPP11



#endif // uti_h__


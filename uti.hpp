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

#include "uti/utiCommonHeader.hpp"
#include "uti/utiAllocator.hpp"
#include "uti/utiReferenceCounted.hpp"
#include "uti/utiByteIterator.hpp"
#include "uti/utiCharIterator.hpp"
#include "uti/utiReverseIterator.hpp"
#include "uti/utiUTF8String.hpp"
#include "uti/utiUTF16String.hpp"

#include "uti/utiAllocator.inl"
#include "uti/utiReferenceCounted.inl"
#include "uti/utiByteIterator.inl"
#include "uti/utiCharIterator.inl"
#include "uti/utiReverseIterator.inl"
#include "uti/utiUTF8String.inl"
#include "uti/utiUTF16String.inl"


#ifdef DISABLE_CPP11
#undef nullptr
#ifdef OLD_NULLPTR
#define nullptr OLD_NULLPTR
#undef OLD_NULLPTR
#endif // OLD_NULLPTR

#endif // DISABLE_CPP11



#endif // uti_h__


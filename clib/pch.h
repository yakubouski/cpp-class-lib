#pragma once
#include <climits>
#include <cstddef>
#include <cstdint>

#ifndef INLINE
#ifdef __GNUC__
#if (__GNUC__ > 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1))
#define forceinline         __inline__ __attribute__((always_inline))
#else
#define forceinline         __inline__
#endif
#elif defined(_MSC_VER)
#define forceinline __forceinline
#elif (defined(__BORLANDC__) || defined(__WATCOMC__))
#define forceinline __inline
#else
#define forceinline
#endif
#endif

#ifdef _UNICODE
#ifndef UNICODE
#define UNICODE
#endif
#endif

#ifdef UNICODE
#define tchar wchar_t
#define tcchar const wchar_t
#define tbyte unsigned char
#define tcbyte const unsigned char
#define _t(x)      L ## x

#else // UNICODE
#define tchar char
#define tcchar const char
#define tbyte unsigned char
#define tcbyte const unsigned char

#define _t(x)      x
#define _u(x)      L ## x

#endif // !UNICODE





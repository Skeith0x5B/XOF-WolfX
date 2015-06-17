/*
===============================================================================

	XOF
	===
	File	:	XOF_Platform.hpp
	Desc	:	Platform id & atomic data types.

===============================================================================
*/
#ifndef XOF_PLATFORM_HPP
#define XOF_PLATFORM_HPP


//namespace xof {


#include "../Core/XOF_Assert.hpp"

#include <iostream>
#include <memory>
#include <string>


// Detect OS/Platform
#define XOF_PLATFORM_WINDOWS 1
#define XOF_PLATFORM_LINUX   2

#if defined( _WIN64 )
	#define XOF_PLATFORM  XOF_PLATFORM_WINDOWS
	#define WIN64_LEAN_AND_MEAN
	#include <Windows.h>
	#include <time.h>
	typedef HWND handle;
#elif defined ( _WIN32 )
	#define XOF_PLATFORM  XOF_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include <time.h>
	typedef HWND handle;
#elif defined( __linux__ )
	#define XOF_PLATFORM  XOF_PLATFORM_XOF
	#include <time.h>
	typedef void* handle; // 7/10/12
#else
	#error Unknown/unsupported platform (OS) - compilation stopped. 
#endif // OS


// Compiler, only supporting msvc and gnuc at the moment - add more as required
// http://sourceforge.net/apps/mediawiki/predef/index.php?title=Compilers
#define XOF_COMPILER_MSVC 1
#define XOF_COMPILER_GCC  2

#if defined( _MSC_VER )
	#if _MSC_VER >= 1400 // vs 2005 onward support __cpuid intrinsic
		#include <intrin.h>
	#endif
	#define XOF_COMPILER XOF_COMPILER_MSVC
#elif defined( __GNUC__ )
	#define XOF_COMPILER XOF_COMPILER_GCC
#else
	#error Unknown/unsupported compiler - compilation stopped.
#endif // Compiler


// Unsigned data types
typedef unsigned char	U8;
typedef unsigned short	U16;
#ifdef UINT
#undef UINT
#endif
typedef unsigned int	U32, UINT;

// Signed data types
typedef char			I8 , CHAR;
typedef short			I16;
#ifdef INT
#undef INT
#endif
typedef int				I32, INT;


// 64 bit can be a little compiler-specific
#if defined( _MSC_VER )
	typedef unsigned __int64 U64;
	typedef __int64 I64;
#else
	typedef unsigned long long U64;
	typedef long long I64;
#endif // #if defined( _MSC_VER )


// Some key paths
namespace XOF_RESOURCE {
	const std::string MESH = "./Resources/Meshes/";
	const std::string TEXTURE = "./Resources/Textures/";
	const std::string SHADER = "./Resources/Shaders/";
}


//} // namespace xof


#endif // XOF_PLATFORM_HPP
/*
===============================================================================

	XOF
	===
	File	:	XOF_Assert.hpp
	Desc	:	Custom assertion...

===============================================================================
*/  
#ifndef XOF_ASSERT_HPP
#define XOF_ASSERT_HPP


#include "../Platform/XOF_Platform.hpp"


#define XOF_ASSERTIONS_ENABLED 1


#if XOF_ASSERTIONS_ENABLED


#if XOF_COMPILER == XOF_COMPILER_MSVC
	// __debugbreak() works on 32/64/Itanium. 
	// MSVC + x64 on MSVC won't support inline assembly so __asm { int 3 } is effectively out the question, 
	// other compilers on windows seem to use either __asm { int 3 } asm("int $3")
	#define XOFDebugBreak() __debugbreak() 
#elif XOF_COMPILER == XOF_COMPILER_GCC
	#define XOFDebugBreak() asm("int $3")
#endif


// TODO: Replace std::cerr with some other failure report utility?
#define XOF_ASSERT( expr )																					\
	if( expr ) {																							\
	} else {																								\
		std::cerr << "ASSERTION FAILED: " << __FILE__ << ", " << __LINE__ << ", " << #expr << std::endl;	\
		XOFDebugBreak();																						\
	}


#else 


#define XOF_ASSERT( expr ) // Disabled, do nothing


#endif // XOF_ASSERTIONS_ENABLED


#endif // XOF_ASSERT_HPP
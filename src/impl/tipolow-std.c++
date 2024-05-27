/*******************************************************************************
 *
 *	Project name:
 *		TiPoLow
 *	
 *	File name:
 *		tipolow-impl-std.cxx
 *	
 *	Description:
 *		[TODO]
 *	
 *	Exposed:
 *		* (void)function(int):
 *			Does stuff.
 *
 ******************************************************************************/

#ifndef __tipolow__impl_std__cxx__
#define __tipolow__impl_std__cxx__

#include <stdlib.h>
#include <stdint.h>

#define TIPOLOW_ALLOC malloc
#define TIPOLOW_FREE free

#ifdef TIPOLOW_DEBUG
	#include <stdio.h>
	#define TIPOLOW_PRINTF std::printf
#else
	#define TIPOLOW_PRINTF(...) (void)__VA_ARGS__;
#endif

#endif

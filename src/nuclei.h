/*******************************************************************************
 *
 *	Project name:
 *		TiPoLow
 *	
 *	File name:
 *		nuclei.h
 *	
 *	Description:
 *		Provides essential classes, functions, and types.
 *	
 *	Exposed:
 *		* [TODO]
 *
 ******************************************************************************/

#ifndef __tipolow__nuclei__h__
#define __tipolow__nuclei__h__

#ifndef TIPOLOW_NO_STDLIB
	#include <stdlib.h>
	
	#define TIPOLOW_ALLOC malloc
	#define TIPOLOW_FREE free
#endif

#ifndef TIPOLOW_NO_STDINT
	#include <stdint.h>
#endif

#ifdef TIPOLOW_DEBUG
	#include <stdio.h>
	#define TIPOLOW_PRINTF std::printf
#else
	#define TIPOLOW_PRINTF(...) (void)__VA_ARGS__;
#endif

namespace TiPoLow {
	typedef uint8_t u8;
	typedef uint16_t u16;
	typedef uint32_t u32;
	typedef uint64_t u64;
	
	typedef uint8_t i8;
	typedef uint16_t i16;
	typedef uint32_t i32;
	typedef uint64_t i64;
	
	typedef intmax_t mi;
	typedef uintmax_t mu;
	
	typedef struct { const char *error; } Error;
	
	template<typename Type>
	class ErrorOr {
		private:
			Type val;
		
		public:
			const char *error = nullptr;
			
			ErrorOr(void) {
				error = nullptr;
			};
			
			ErrorOr(Error error_struct) {
				error = error_struct.error;
			};
			
			ErrorOr(Type value_pointer) {
				error = nullptr;
				val = value_pointer;
			};
			
			Type value(void) { return val; };
	};
	
	template<typename Type>
	class ErrorOr<Type*> {
		private:
			Type *val = nullptr;
		
		public:
			const char *error = nullptr;
			
			ErrorOr(void) {
				error = nullptr;
			};
			
			ErrorOr(Error error_struct) {
				error = error_struct.error;
			};
			
			ErrorOr(Type *value_pointer) {
				error = nullptr;
				val = value_pointer;
			};
			
			Type *value(void) { return val; };
	};
	
	template<typename Type>
	class ErrorOr<Type&> {
		public:
			const char *error = nullptr;
			Type* value_pointer = nullptr;
			
			ErrorOr(void) {
				error = nullptr;
			};
			
			ErrorOr(Error error_struct) {
				error = error_struct.error;
			};
			
			ErrorOr(Type *val_ptr) {
				error = nullptr;
				value_pointer = val_ptr;
			};
			
			Type& value(void) { return (*value_pointer); };
	};
	
	template<>
	class ErrorOr<void> {
		public:
			const char *error = nullptr;
			
			ErrorOr(Error error_struct) { error = error_struct.error; };
	};
	
	template<typename Type>
	ErrorOr<Type*> alloc_type(mu n) {
		if(n == 0) return Error { "Couldn't allocate sufficient memory" };
		Type *x = (Type *)TIPOLOW_ALLOC((mu)sizeof(Type) * n);
		return x;
	};
	
	template<typename Type>
	Type* alloc_type(void) { return alloc_type<Type>(1); };
	
	template<typename Type>
	ErrorOr<void> free_type(Type *ptr) {
		if(ptr == nullptr) return Error { nullptr };
		TIPOLOW_FREE((void *)ptr);
		
		return Error { nullptr };
	};
};

#endif


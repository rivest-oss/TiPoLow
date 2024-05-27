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
 *		* (void)function(int):
 *			Does stuff.
 *
 ******************************************************************************/

#ifndef __tipolow__nuclei__h__
#define __tipolow__nuclei__h__

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
	
	template<typename Type>
	Type *alloc_type(mu n) {
		if(n == 0) return nullptr;
		Type *x = (Type *)TIPOLOW_ALLOC((mu)sizeof(Type) * n);
		return x;
	};
	
	template<typename Type>
	Type* alloc_type(void) { return alloc_type<Type>(1); };
	
	template<typename Type>
	void free_type(Type *ptr) {
		if(ptr == nullptr) return;
		TIPOLOW_FREE((void *)ptr);
	};
	
	typedef struct { const char *error; } Error;
	
	template<typename Type>
	class ErrorOr {
		public:
			const char *error = nullptr;
			Type value = 0;
			
			ErrorOr(void) {
				error = nullptr;
				value = 0;
			};
			
			ErrorOr(Error error_struct) {
				error = error_struct.error;
				value = 0;
			};
			
			ErrorOr(Type value_pointer) {
				error = nullptr;
				value = value_pointer;
			};
	};
	
	template<>
	class ErrorOr<void> {
		public:
			const char *error = nullptr;
			
			ErrorOr(Error error_struct) { error = error_struct.error; };
	};
};

#endif


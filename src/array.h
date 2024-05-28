/*******************************************************************************
 *
 *	Project name:
 *		TiPoLow
 *	
 *	File name:
 *		array.h
 *	
 *	Description:
 *		Provides a dynamic array with a growth factor of ~1.375.
 *	
 *	Exposed:
 *		* [TODO]
 *
 ******************************************************************************/

#ifndef __tipolow__array__h__
#define __tipolow__array__h__

namespace TiPoLow {
	template<typename Type>
	class DArray {
		private:
			Type *arr = nullptr;
			mu arr_size = 0;
			mu arr_capacity = 0;
			
			mu calculate_growth(mu i) {
				return (i + 1) + (i >> 3) + (i >> 2);
			};
		
		public:
			DArray() { (void)clear(); };
			~DArray() { (void)clear(); };
			
			ErrorOr<void> clear(void) {
				arr_size = 0;
				arr_capacity = 0;
				
				ErrorOr<void> e = free_type(arr);
				if(e.error) return e;
				
				return Error { nullptr };
			};
			
			ErrorOr<void> resize(mu target_capacity, bool free_after) {
				ErrorOr<Type*> new_arr = alloc_type<Type>(target_capacity);
				if(new_arr.error) return Error { new_arr.error };
				
				mu new_arr_size = arr_size;
				if(new_arr_size > target_capacity)
					new_arr_size = target_capacity;
				
				for(mu i = 0; i < new_arr_size; i++)
					new_arr.value()[i] = arr[i];
				
				Type *old_arr = arr;
				
				arr_size = new_arr_size;
				arr = new_arr.value();
				arr_capacity = target_capacity;
				
				if(free_after) {
					ErrorOr<void> e = free_type(old_arr);
					if(e.error) return e;
				}
				
				return Error { nullptr };
			};
			
			ErrorOr<void> resize(mu target_capacity) {
				return resize(target_capacity, true);
			};
			
			ErrorOr<void> reserve(mu res_capacity) {
				if(arr_capacity >= res_capacity) return Error { nullptr };
				
				ErrorOr<void> e = resize(res_capacity, true);
				if(e.error) return e;
				
				return Error { nullptr };
			};
			
			ErrorOr<void> shrink_to_fit(void) {
				if(arr_size == arr_capacity) return Error { nullptr };
				
				ErrorOr<void> e = resize(arr_size, true);
				if(e.error) e;
				
				return Error { nullptr };
			};
			
			mu size(void) { return arr_size; };
			mu capacity(void) { return arr_capacity; };
			bool is_empty(void) { return (arr_size == 0); };
			
			ErrorOr<Type&> at(mu i) {
				if(i >= arr_size) return Error { "Out of bonds" };
				
				return &(arr[i]);
			};
			
			Type& operator[](mu i) {
				if(i >= arr_capacity) return *(Type*)nullptr;
				if(arr_size < i) arr_size = i;
				
				return arr[i];
			};
			
			ErrorOr<Type&> front(void) { return at(0); };
			ErrorOr<Type&> back(void) { return at(arr_size - 1); };
			Type *data(void) { return arr; };
			
			ErrorOr<void> insert(mu i, mu n, Type value) {
				if((i + n) >= arr_capacity) {
					ErrorOr<void> e = resize(calculate_growth(i + n), true);
					if(e.error) return e;
				}
				
				if((i + n) > arr_size) arr_size = (i + n);
				
				for(mu j = 0; j < n; j++)
					arr[i + j] = value;
				
				return Error { nullptr };
			};
			
			ErrorOr<void> insert(mu i, Type value) {
				return insert(i, 1, value);
			};
			
			ErrorOr<void> push_back(Type value) {
				return insert(arr_size, value);
			};
			
			ErrorOr<void> erase(mu from, mu length) {
				if((length == 0) || (from >= arr_size))
					return Error { nullptr };
				
				mu to = (from + length);
				if(to >= arr_size) to = arr_size;
				
				for(mu i = from; i < to; i++) {
					if((i + length) >= arr_size) {
						arr[i] = (0);
					} else {
						arr[i] = arr[i + length];
					}
				};
				
				arr_size = (length > arr_size)
					? 0
					: (arr_size - length);
				
				if(arr_size <= (arr_capacity >> 1)) {
					ErrorOr<void> e = shrink_to_fit();
					if(e.error) return e;
				}
				
				return Error { nullptr };
			};
			
			ErrorOr<void> pop_back(void) {
				if(arr_size == 0) return Error { "The array is empty" };
				
				Type value = arr[(arr_size == 0) ? 0 : (arr_size - 1)];
				arr_size -= 1;
				
				if(arr_size <= (arr_capacity >> 1)) {
					ErrorOr<void> e = shrink_to_fit();
					if(e.error) return e;
				}
				
				return value;
			};
	};
};

#endif


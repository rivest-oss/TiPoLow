/*******************************************************************************
 *
 *	Project name:
 *		TiPoLow
 *	
 *	File name:
 *		string.h
 *	
 *	Description:
 *		Provides a basic String class.
 *	
 *	Exposed:
 *		* [TODO]
 *
 ******************************************************************************/

#ifndef __tipolow__string__h__
#define __tipolow__string__h__

#include "array.h"

namespace TiPoLow {
	class String : private DArray<char> {
		public:
			String(const char *s) { (void)from(s); };
			String(String *s) { (void)from(s); };
			String(String& s) { (void)from(s); };
			
			String(void) {};
			
			ErrorOr<void> from(const char *s) {
				mu string_length = 0;
				
				const char *sc = s;
				
				while((*sc) != '\0') {
					string_length++;
					sc++;
				};
				
				ErrorOr<void> e = reserve(string_length);
				if(e.error) return Error { e.error };
				
				while((*s) != '\0') {
					e = push_back(*s);
					if(e.error) return Error { e.error };
					s++;
				}
				
				return Error { nullptr };
			};
			
			ErrorOr<void> from(String& s) {
				ErrorOr<void> e = reserve(s.length());
				if(e.error) return Error { e.error };
				
				for(mu i = 0; i < s.size(); i++) {
					ErrorOr<char&> c = s.at(i);
					if(c.error) return Error { c.error };
					
					e = push_back(c.value());
					if(e.error) return Error { e.error };
				};
				
				return Error { nullptr };
			};
			
			ErrorOr<void> from(String *s) {
				ErrorOr<void> e = reserve(s->length());
				if(e.error) return Error { e.error };
				
				for(mu i = 0; i < s->size(); i++) {
					ErrorOr<char&> c = s->at(i);
					if(c.error) return Error { c.error };
					
					e = push_back(c.value());
					if(e.error) return Error { e.error };
				};
				
				return Error { nullptr };
			};
			
			mu length() { return size(); };
			
			ErrorOr<u32> readUTF8(mu i) {
				u32 ret = 0x00000000;
				u8 char_length = 1;
				
				ErrorOr<char&> first_byte = at(i);
				if(first_byte.error) return Error { first_byte.error };
				
				// Check MSb.
				if(first_byte.value() & 0x80) {
					// If the MSb (1), it must be a 2-, 3-, 4-byte character
					// or just an invalid character.
					
					// If only the MSb is (1), then it may be corrupted, or a
					// non-UTF-8 character.
					if((first_byte.value() & 0b11000000) == 0b10000000) {
						return Error { "Invalid CONTINUE flag as first byte" };
					}
					
					if((first_byte.value() & 0b11100000) == 0b11000000) {
						char_length = 2;
						ret |= (u32)(first_byte.value() & 0b00011111);
					}
					
					if((first_byte.value() & 0b11110000) == 0b11100000) {
						char_length = 3;
						ret |= (u32)(first_byte.value() & 0b00001111);
					}
					
					if((first_byte.value() & 0b11111000) == 0b11110000) {
						char_length = 4;
						ret |= (u32)(first_byte.value() & 0b00000111);
					}
					
					for(u8 j = 1; j < char_length; j++) {
						ErrorOr<char&> curr_byte = at(i + j);
						if(curr_byte.error) return Error { curr_byte.error };
						
						if((curr_byte.value() & 0b11000000) != 0b10000000) {
							return Error {
								"UTF-8 reader was expecting a CONTINUE "
								"character"
							};
						}
						
						ret = ((ret << 6) | (curr_byte.value() & 0x7f));
					};
				} else {
					// If it's (0), it must be an ASCII character.
					ret = (u32)(first_byte.value() & 0x7f);
				}
				
				return ret;
			};
			
			ErrorOr<char *> c_str(void) {
				ErrorOr<char *> s = alloc_type<char>(length() + 1);
				if(s.error) return Error { s.error };
				
				char *sc = s.value();
				for(mu i = 0; i < length(); i++) {
					ErrorOr<char&> c = at(i);
					if(c.error) return Error { c.error };
					
					*sc = c.value();
					sc++;
				};
				
				return s.value();
			};
	};
};

#endif

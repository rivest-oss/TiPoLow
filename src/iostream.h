/*******************************************************************************
 *
 *	Project name:
 *		TiPoLow
 *	
 *	File name:
 *		iostream.h
 *	
 *	Description:
 *		Base I/O stream.
 *	
 *	Exposed: [TODO] Documentation.
 *
 ******************************************************************************/

#ifndef __tipolow__iostream__h__
#define __tipolow__iostream__h__

namespace TiPoLow {
	namespace ios {
		typedef enum {
			beg = 1,
			cur = 2,
			end = 3,
		} seekdir;
	};
	
	class BaseIOStream {
		public:
			virtual mu tellg(void) = 0;
			virtual void seekg(mu pos) = 0;
			virtual void seekg(mi off, ios::seekdir dir) = 0;
			
			virtual mu tellp(void) = 0;
			virtual void seekp(mu pos) = 0;
			virtual void seekp(mi off, ios::seekdir dir) = 0;
			
			virtual ErrorOr<void> read(u8 *s, mu n) = 0;
			virtual ErrorOr<void> write(u8 *s, mu n) = 0;
			
			void skipg(mi pos) { seekg(pos, ios::seekdir::cur); };
			void skipp(mi pos) { seekp(pos, ios::seekdir::cur); };
			
			ErrorOr<u8> readU8(void) {
				u8 buff[1];
				ErrorOr<void> e = read(buff, 1);
				if(e.error) return Error { e.error };
				return buff[0];
			};
			
			ErrorOr<u16> readU16BE(void) {
				u8 buff[2];
				ErrorOr<void> e = read(buff, 2);
				if(e.error) return Error { e.error };
				
				return (
					(((u16)buff[0]) << 8) |
					((u16)buff[1])
				);
			};
			
			ErrorOr<u32> readU32BE(void) {
				u8 buff[4];
				ErrorOr<void> e = read(buff, 4);
				if(e.error) return Error { e.error };
				
				return (
					(((u32)buff[0]) << 24) |
					(((u32)buff[1]) << 16) |
					(((u32)buff[2]) << 8) |
					((u32)buff[3])
				);
			};
			
			ErrorOr<u64> readU64BE(void) {
				u8 buff[8];
				ErrorOr<void> e = read(buff, 8);
				if(e.error) return Error { e.error };
				
				return (
					(((u64)buff[0]) << 56) |
					(((u64)buff[1]) << 48) |
					(((u64)buff[2]) << 40) |
					(((u64)buff[3]) << 32) |
					(((u64)buff[4]) << 24) |
					(((u64)buff[5]) << 16) |
					(((u64)buff[6]) << 8) |
					((u64)buff[7])
				);
			};
			
			ErrorOr<u16> readU16LE(void) {
				ErrorOr<u16> e = readU16BE();
				if(e.error) return e;
				
				u16 num = e.value();
				num = (
					(num << 8) |
					(num >> 8)
				);
				
				return num;
			};
			
			ErrorOr<u32> readU32LE(void) {
				ErrorOr<u32> e = readU32BE();
				if(e.error) return Error { e.error };
				
				u32 num = e.value();
				
				return (
					(num >> 24) |
					((num << 8) & 0xff0000) |
					((num >> 8) & 0xff00) |
					((num << 24) & 0xff000000)
				);
			};
			
			ErrorOr<u64> readU64LE(void) {
				ErrorOr<u64> e = readU64BE();
				if(e.error) return Error { e.error };
				
				u64 num = e.value();
				
				return (
					((num >> 56)	& 0x00000000000000ff) |
					((num >> 40)	& 0x000000000000ff00) |
					((num >> 24)	& 0x0000000000ff0000) |
					((num >> 8)		& 0x00000000ff000000) |
					((num << 8)		& 0x000000ff00000000) |
					((num << 24)	& 0x0000ff0000000000) |
					((num << 40)	& 0x00ff000000000000) |
					((num << 56)	& 0xff00000000000000)
				);
			};
			
			ErrorOr<i8> readI8(void) { return readU8(); };
			
			ErrorOr<i16> readI16BE(void) { return readU16BE(); };
			ErrorOr<i32> readI32BE(void) { return readU32BE(); };
			ErrorOr<i64> readI64BE(void) { return readU64BE(); };
			
			ErrorOr<i16> readI16LE(void) { return readU16LE(); };
			ErrorOr<i32> readI32LE(void) { return readU32LE(); };
			ErrorOr<i64> readI64LE(void) { return readU64LE(); };
			
			ErrorOr<void> writeU8(u8 value) {
				u8 buff[1] = {
					(u8)value
				};
				
				ErrorOr<void> e = write(buff, 1);
				if(e.error) return Error { e.error };
				return Error { nullptr };
			};
			
			ErrorOr<void> writeU16BE(u16 value) {
				u8 buff[2] = {
					(u8)((value >> 8) & 0xff),
					(u8)(value & 0xff)
				};
				
				ErrorOr<void> e = write(buff, 2);
				if(e.error) return Error { e.error };
				return Error { nullptr };
			};
			
			ErrorOr<void> writeU32BE(u32 value) {
				u8 buff[4] = {
					(u8)((value >> 24) & 0xff),
					(u8)((value >> 16) & 0xff),
					(u8)((value >> 8) & 0xff),
					(u8)(value & 0xff)
				};
				
				ErrorOr<void> e = write(buff, 4);
				if(e.error) return Error { e.error };
				return Error { nullptr };
			};
			
			ErrorOr<void> writeU64BE(u64 value) {
				u8 buff[8] = {
					(u8)((value >> 56) & 0xff),
					(u8)((value >> 48) & 0xff),
					(u8)((value >> 40) & 0xff),
					(u8)((value >> 32) & 0xff),
					(u8)((value >> 24) & 0xff),
					(u8)((value >> 16) & 0xff),
					(u8)((value >> 8) & 0xff),
					(u8)(value & 0xff)
				};
				
				ErrorOr<void> e = write(buff, 8);
				if(e.error) return Error { e.error };
				return Error { nullptr };
			};
			
			ErrorOr<void> writeU16LE(u16 value) {
				u8 buff[2] = {
					(u8)(value & 0xff),
					(u8)((value >> 8) & 0xff)
				};
				
				ErrorOr<void> e = write(buff, 2);
				if(e.error) return Error { e.error };
				return Error { nullptr };
			};
			
			ErrorOr<void> writeU32LE(u32 value) {
				u8 buff[4] = {
					(u8)(value & 0xff),
					(u8)((value >> 8) & 0xff),
					(u8)((value >> 16) & 0xff),
					(u8)((value >> 24) & 0xff)
				};
				
				ErrorOr<void> e = write(buff, 4);
				if(e.error) return Error { e.error };
				return Error { nullptr };
			};
			
			ErrorOr<void> writeU64LE(u64 value) {
				u8 buff[8] = {
					(u8)(value & 0xff),
					(u8)((value >> 8) & 0xff),
					(u8)((value >> 16) & 0xff),
					(u8)((value >> 24) & 0xff),
					(u8)((value >> 32) & 0xff),
					(u8)((value >> 40) & 0xff),
					(u8)((value >> 48) & 0xff),
					(u8)((value >> 56) & 0xff)
				};
				
				ErrorOr<void> e = write(buff, 8);
				if(e.error) return Error { e.error };
				return Error { nullptr };
			};
			
			ErrorOr<void> writeI8(i8 value) { return writeU8(value); };
			ErrorOr<void> writeI16LE(i16 value) { return writeU16LE(value); };
			ErrorOr<void> writeI32LE(i32 value) { return writeU32LE(value); };
			ErrorOr<void> writeI64LE(i64 value) { return writeU64LE(value); };
			
			ErrorOr<float> readF32(void) {
				ErrorOr<u32> raw_value = readU32BE();
				if(raw_value.error) return Error { raw_value.error };
				
				u32 value_u32 = raw_value.value();
				float value = *(float *)&value_u32;
				
				return value;
			};
			
			ErrorOr<float> readF64(void) {
				ErrorOr<u64> raw_value = readU64BE();
				if(raw_value.error) return Error { raw_value.error };
				
				u64 value_u64 = raw_value.value();
				double value = *(double *)&value_u64;
				
				return value;
			};
			
			ErrorOr<void> writeF32(float value) {
				u32 *raw_value = (u32 *)(&value);
				return writeU32BE(*raw_value);
			};
			
			ErrorOr<void> writeF64(float value) {
				u64 *raw_value = (u64 *)(&value);
				return writeU64BE(*raw_value);
			};
	};
};

#endif


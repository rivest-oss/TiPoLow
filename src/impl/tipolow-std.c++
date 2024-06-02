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

namespace TiPoLow {
	namespace FS {
		ErrorOr<void> BaseFileStream::open(u16 mode) {
			// [TODO]
		};
		
		ErrorOr<void> BaseFileStream::close(void) {
			// [TODO]
		};
		
		mu BaseFileStream::tellg(void) {
			// [TODO]
		};
		
		void BaseFileStream::seekg(mu pos) {
			// [TODO]
		};
		
		void BaseFileStream::seekg(mi off, seekdir dir) {
			// [TODO]
		};
		
		mu BaseFileStream::tellp(void) {
			// [TODO]
		};
		
		void BaseFileStream::seekp(mu pos) {
			// [TODO]
		};
		
		void BaseFileStream::seekp(mi off, seekdir dir) {
			// [TODO]
		};
		
		ErrorOr<void> BaseFileStream::read(u8 *s, mu n) {
			// [TODO]
		};
		
		ErrorOr<void> BaseFileStream::write(u8 *s, mu n) {
			// [TODO]
		};
		
		bool BaseFileStream::is_open(void) {
			// [TODO]
		};
		
		ErrorOr<void> BaseFileInterface::resolve_path(void) {
			// [TODO]
		};
		
		ErrorOr<void> BaseFileInterface::copy(BaseFileInterface& to) {
			// [TODO]
		};
		
		ErrorOr<void> BaseFileInterface::create_symlink(BaseFileInterface& to,
														bool dir
		) {
			// [TODO]
		};
		
		ErrorOr<void> BaseFileInterface::remove(void) {
			// [TODO]
		};
		
		ErrorOr<mu> BaseFileInterface::remove_all(void) {
			// [TODO]
		};
		
		ErrorOr<void> BaseFileInterface::move(BaseFileInterface& to) {
			// [TODO]
		};
		
		ErrorOr<void> BaseFileInterface::resize(mu size) {
			// [TODO]
		};
		
		ErrorOr<FileSystemSpace> BaseFileInterface::space(void) {
			// [TODO]
		};
		
		ErrorOr<FileStatus> BaseFileInterface::status(void) {
			// [TODO]
		};
		
		ErrorOr<const char *> BaseFileInterface::temp_directory_path(void) {
			// [TODO]
		};
	};
};

#endif

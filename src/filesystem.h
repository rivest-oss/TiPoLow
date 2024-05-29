/*******************************************************************************
 *
 *	Project name:
 *		TiPoLow
 *	
 *	File name:
 *		filesystem.h
 *	
 *	Description:
 *		Provides high-level functions and classes for file managing and R/W.
 *	
 *	Exposed:
 *		* [TODO]
 *
 ******************************************************************************/

#ifndef __tipolow__filesystem__h__
#define __tipolow__filesystem__h__

#include "iostream.h"

namespace TiPoLow {
	namespace ios {
		typedef enum _openmode {
			in = 1 << 0,
			out = 1 << 1,
		} openmode;
	};
	
	namespace FS {
		typedef enum _permissions {
			// [TODO]
		} permissions;
		
		typedef struct {
			bool	found		= false;
			
			bool	regular		= false;
			bool	directory	= false;
			bool	block		= false;
			bool	character	= false;
			bool	fifo		= false;
			bool	socket		= false;
			bool	symlink		= false;
			
			// https://en.cppreference.com/w/cpp/filesystem/file_size
			mu		file_size	= 0;
		} FileStatus;
		
		typedef struct {
			mu		capacity	= 0;
			mu		free		= 0;
			mu		available	= 0;
		} FileSystemSpace;
		
		class BaseFileInterface {
			public:
				const char *__p_path = nullptr;
				
				// Implementation specific thingy.
				void *__i_path = nullptr;
				FileStatus __i_status;
				
				BaseFileInterface(const char *path) { set_path(path); };
				BaseFileInterface(void) {};
				
				void set_path(const char *path) { __p_path = path; };
				
				// https://en.cppreference.com/w/cpp/filesystem/absolute
				ErrorOr<void> resolve_path(void);
				// https://en.cppreference.com/w/cpp/filesystem/copy
				ErrorOr<void> copy(BaseFileInterface& to);
				// https://en.cppreference.com/w/cpp/filesystem/create_symlink
				ErrorOr<void> create_symlink(BaseFileInterface& to, bool dir);
				// https://en.cppreference.com/w/cpp/filesystem/remove
				ErrorOr<void> remove(void);
				ErrorOr<mu> remove_all(void);
				// https://en.cppreference.com/w/cpp/filesystem/rename
				ErrorOr<void> move(BaseFileInterface& to);
				// https://en.cppreference.com/w/cpp/filesystem/resize_file
				ErrorOr<void> resize(mu size);
				// https://en.cppreference.com/w/cpp/filesystem/space
				ErrorOr<FileSystemSpace> space(void);
				// https://en.cppreference.com/w/cpp/filesystem/status
				ErrorOr<FileStatus> status(void);
				// https://en.cppreference.com/w/cpp/filesystem/
				// temp_directory_path
				ErrorOr<const char *> temp_directory_path(void);
		};
	};
};

#endif


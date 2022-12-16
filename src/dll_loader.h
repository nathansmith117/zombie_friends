#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef _WIN32
#include <Windows.h>
#include <Lmcons.h>
#else
#include <unistd.h>
#include <dlfcn.h>
#endif


namespace DllLoader {
#ifdef _WIN32
	typedef HINSTANCE HANDLE;
#else
	typedef void * HANDLE;
#endif

	typedef void * ADDRESS;

	HANDLE load(const char * file_path);
	ADDRESS get_address(HANDLE handle, const char * symbol);
	int close(HANDLE handle);
};

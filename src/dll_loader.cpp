#include "dll_loader.h"

namespace DllLoader {
	HANDLE load(const char * file_path) {
		if (file_path == NULL)
			return NULL;

#ifdef _WIN32
		return LoadLibraryA((LPCSTR)file_path);
#else
		return dlopen(file_path, RTLD_NOW | RTLD_GLOBAL);
#endif
	}

	ADDRESS get_address(HANDLE handle, const char * symbol) {
		if (handle == NULL || symbol == NULL)
			return NULL;

#ifdef _WIN32
		return (ADDRESS)GetProcAddress(handle, (LPCSTR)symbol)
#else
		return (ADDRESS)dlsym(handle, symbol);
#endif
	}

	int close(HANDLE handle) {
		if (handle == NULL)
			return -1;

#ifdef _WIN32
		return (FreeLibrary(handle) != 0) ? 0 : -1;
#else
		return dlclose(handle);
#endif
	}
};

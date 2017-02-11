#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__MINGW64__)
    // Do windows stuff here
    #define PLATFORM_WINDOWS
#elif defined(__APPLE__)
	// Do Mac stuff here
	#define PLATFORM_MAC
#elif defined(__linux__)
	// Do linux stuff here
	#define PLATFORM_LINUX
#else
	#error "Unknown platform"
#endif

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__MINGW64__)
    // Do windows stuff here
#elif defined(__APPLE__)
	// Do Mac stuff here
#elif defined(__linux__)
	// Do linux stuff here
#else
	#error "Unknown platform"
#endif

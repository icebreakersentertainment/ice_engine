Hercules
--------

[![Build Status](https://travis-ci.org/jarrettchisholm/hercules.png)](https://travis-ci.org/jarrettchisholm/hercules)
[![Build status](https://ci.appveyor.com/api/projects/status/ardx8mj0aa7e9fxu/branch/master)](https://ci.appveyor.com/project/jarrettchisholm/hercules/branch/master)

**Note**: This project is incredibly young, and isn't in any kind of production ready state. 

To clone:

    git clone https://github.com/jarrettchisholm/hercules.git

Documentation: [http://game-engine-hercules.readthedocs.io/en/latest/](http://game-engine-hercules.readthedocs.io/en/latest/)

Get/build prerequisites:

    python setup.py

To build on Linux:

    mkdir build
    cd build
    cmake ..
    make

To test on Linux:

    mkdir build
    cd build
    cmake -D HERCULES_BUILD_AS_LIBRARY=1 -D HERCULES_GRAPHICS_BACKEND=noop -D HERCULES_BUILD_TESTS=1 ..
    make
    ctest

To build on Windows:

    mkdir build
    cd build
    
    # Workaround for `Release` directory being created as a file in Windows
    mkdir build/Release
    
    cmake -G "Visual Studio 14" ..
    msbuild /p:Configuration=Release hercules.sln

To test on Windows:

    mkdir build
    
    # Workaround for `Release` directory being created as a file in Windows
    mkdir build/Release
    mkdir build/tests
    mkdir build/tests/Release
    
    cmake -G "Visual Studio 14" -D BUILD_AS_LIBRARY=1 -D HERCULES_GRAPHICS_BACKEND=noop -D HERCULES_BUILD_TESTS=1 ..
    msbuild /p:Configuration=Release hercules.sln
    ctest

Hercules
--------

[![Build Status](https://travis-ci.org/jarrettchisholm/hercules.png)](https://travis-ci.org/jarrettchisholm/hercules)
[![Build status](https://ci.appveyor.com/api/projects/status/ardx8mj0aa7e9fxu/branch/master)](https://ci.appveyor.com/project/jarrettchisholm/hercules/branch/master)

**Note**: This project is incredibly young, and isn't in any kind of production ready state. 

To clone:

    git clone https://github.com/jarrettchisholm/hercules.git

Get/build prerequisites:

    python setup.py

To build on Linux:

    mkdir build
    cd build
    cmake ..
    make

To build on Windows:

    mkdir build
    cd build
    cmake -G "Visual Studio 14" ..
    msbuild /p:Configuration=Release hercules.sln

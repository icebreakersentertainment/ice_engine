Hercules
--------

[![Build Status](https://travis-ci.org/jarrettchisholm/hercules.png)](https://travis-ci.org/jarrettchisholm/hercules)

To clone:

    git clone ssh://git@module9games.com/opt/git/hercules.git

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

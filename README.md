Ice Engine
--------

[![Build Status](https://travis-ci.com/icebreakersentertainment/ice_engine.svg?branch=master)](https://travis-ci.com/icebreakersentertainment/ice_engine)
[![Build status](https://ci.appveyor.com/api/projects/status/2tqewfhy60mxv429/branch/master?svg=true)](https://ci.appveyor.com/project/icebreakersentertainment/ice-engine/branch/master)
[![Documentation Status](https://readthedocs.org/projects/ice-engine/badge/?version=latest)](http://ice-engine.readthedocs.io/en/latest/?badge=latest)

**Note**: This project is incredibly young, and isn't in any kind of production ready state.

Documentation is available at [http://ice-engine.readthedocs.io/en/latest/](http://ice-engine.readthedocs.io/en/latest/).

To clone:

    git clone https://github.com/icebreakersentertainment/ice_engine.git

Get/build prerequisites:

    mkdir build
    cd build

    conan profile new default --detect
    conan profile update settings.compiler.libcxx=libstdc++11 default

    conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan

    conan create ../conan/ctpl icebreakersentertainment/stable
    conan create ../conan/entityx icebreakersentertainment/stable
    conan create ../conan/angelscript icebreakersentertainment/stable
    conan create ../conan/freeimage icebreakersentertainment/stable
    conan create ../conan/celero icebreakersentertainment/stable

    conan install ..

To build on Linux:

    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="conan_paths.cmake" ..
    make

To test on Linux:

    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="conan_paths.cmake" -DICEENGINE_BUILD_TESTS=1 -DICEENGINE_BUILD_AS_LIBRARY=1 ..
    make
    ctest

To build on Windows:

    mkdir build
    cd build

    cmake -G "Visual Studio 16 2019" -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="conan_paths.cmake" ..
    msbuild /p:Configuration=Release ice_engine.sln

To test on Windows:

    mkdir build
    cd build

    cmake -G "Visual Studio 16 2019" -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="conan_paths.cmake" -DICEENGINE_BUILD_TESTS=1 -DICEENGINE_BUILD_AS_LIBRARY=1 ..
    msbuild /p:Configuration=Release ice_engine.sln
    ctest

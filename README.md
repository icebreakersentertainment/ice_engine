Hercules
--------

To clone:

    git clone ssh://git@module9games.com/opt/git/hercules.git

Build prerequisites:

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

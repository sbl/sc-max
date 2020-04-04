# bruits

Sound generating devices.

Author: Stephen Lumenta (stephen.lumenta@gmail.com)

### UGens

- Wellen: Tides2
- Zopf: Plaits

### Requirements

- CMake >= 3.5
- SuperCollider source code

### Building

Clone the project, then use CMake to configure and build it. If you start from a directory that
contains the directory where you cloned SuperCollider, the following commands will work.

    git clone https://github.com/sbl/bruits
    cd bruits
    mkdir build
    cd build

Then, depending on your toolchain:

    # Linux + make
    cmake .. -DCMAKE_BUILD_TYPE=Release
    make -j
    make install

    # macOS + Xcode
    cmake .. -GXcode
    cmake --build . --config Release
    cmake --build . --config Release --target install

    # Windows + VS 2017
    cmake .. -G"Visual Studio 15 2017 Win64"
    cmake --build . --config Release
    cmake --build . --config Release --target install

You may want to manually specify the install location in the first step to point it at your
SuperCollider extensions directory: add the option `-DCMAKE_INSTALL_PREFIX=/path/to/extensions`.

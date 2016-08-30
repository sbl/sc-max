# sc-max
[![Build Status](https://travis-ci.org/sbl/sc-max.svg?branch=master)](https://travis-ci.org/sbl/sc-max)

Some supercollider ugens ported to max-msp (v>6.19 - 32 + 64 bit) the focus lies on the noisier ones.
Ports by Stephen Lumenta.

- gendys
- dusts
- noises
- lfnoises

All credits are due to the original authors (James McCartney et alt.). these
are merely ports to max/msp and might be helpful.
Helptext is taken directly from the original supercollider helpfiles. If you
haven't done so, check out [supercollider](http://supercollider.github.io/) as
well. It is a really nice language for sound.

Use the issue tracker for bug reports or other issues.

## Installation:

- latest version [mac](https://github.com/sbl/sc-max/releases/download/1.0.0/sc-max-1.0.0.zip)

Drop the sc-max folder somewhere in your max-search-path for projects.

## Older versions:

If you're looking for an older unmaintained (32 bit only) version:

- [0.2.1 mac UB](http://github.com/downloads/sbl/sc-max/sc-max-0.2.1.zip)

## Compilation Prerequisites

To build the externals in this package you will need some form of compiler support on your system. 

* On the Mac this means Xcode (you can get from the App Store for free). 
* On Windows this most likely means some version of Visual Studio (the free versions should work fine).

You will also need to install [CMake](https://cmake.org/download/).

## Building

0. Get the code from Github, or download a zip and unpack it into a folder.
1. In the Terminal or Console app of your choice, change directories (cd) into the folder you created in step 0.
2. `mkdir build` to create a folder with your various build files
3. `cd build` to put yourself into that folder
4. Now you can generate the projects for your choosen build environment:

### Mac

You can build on the command line using Makefiles, or you can generate an Xcode project and use the GUI to build.

* Xcode: Run `cmake -G Xcode ..` and then run `cmake --build .` or open the Xcode project from this "build" folder and use the GUI.
* Make: Run `cmake ..` and then run `cmake --build .` or `make`.  Note that the Xcode project is preferrable because it is able substitute values for e.g. the Info.plist files in your builds.

### Windows

The exact command line you use will depend on what version of Visual Studio you have installed.  You can run `cmake --help` to get a list of the options available.  Assuming some version of Visual Studio 2013, the commands to generate the projects will look like this:

* 32 bit: `cmake -G "Visual Studio 12" ..`
* 64 bit: `cmake -G "Visual Studio 12 Win64" -DWIN64:Bool=True ..`

Having generated the projects, you can now build by opening the .sln file in the build folder with the Visual Studio app (just double-click the .sln file) or you can build on the command line like this:

`cmake --build . --config Release`

## LICENSE

All SuperCollider code and sc-max are licensed under the gpl
http://www.gnu.org/licenses/gpl.html

# Game Programming in C++ Code
This repository contains the source code for *Game Programming in C++* by Sanjay Madhav.

The source code for the chapters is released under the BSD 3-clause
license. See LICENSE for more detail. Note that this license does not apply to
the code in the External directory. Each External project is licensed separately.

# William (BJ) Blair Changes
I'm following the book on Linux (Ubuntu 20.04, gcc 9.3) using Makefiles. Assumes SDL2
is installed (sudo apt-get install libsdl2-dev). Simply type 'make' in the chapter
directory to build.

# Building the Code
Each chapter's code is tested and works on both Microsoft Windows and Apple macOS.

To compile on Windows, install Microsoft Visual Studio 2017 Community
(https://www.visualstudio.com/downloads/). During installation, select the
"Game Development in C++" workflow. In each Chapter directory, there is a
corresponding ChapterXX-windows.sln file to open.

To compile on macOS, install Xcode from the App Store. Each chapter has
a corresponding ChapterXX-mac.xcodeproj file.

Code for Chapter 7 and beyond uses the FMOD API for audio. This requires
a separate installation from (https://www.fmod.com/download). Download
and install version 1.09.x of the FMOD Studio API (newer versions are untested).
On Windows, install FMOD to the default directory. On Mac, copy the contents
of the FMOD package into External/FMOD.

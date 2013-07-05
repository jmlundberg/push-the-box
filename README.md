Simple 3D reincarnation of Sokoban running in
[Google Chrome Native Client](https://developers.google.com/native-client/)
on top of [Magnum](https://github.com/mosra/magnum) OpenGL engine. Your target
is to **push** all the boxes to their destination places.

![Screenshot](https://github.com/nuclearwhales/push-the-box/raw/master/screenshot.png)

(Slightly outdated) **playable version is available** on http://nuclearwhales.github.io/push-the-box.
Please be patient, it will took a while before it loads -- the dependency
libraries have ~10 MB in total. If you encounter any problems, please let us
know :-)

Browser requirements
--------------------

You need Google Chrome 25 or higher with working Native Client and OpenGL.
Native Client is disabled by default and only apps from Chrome Web Store are
allowed to run. This can be solved either by enabling Native Client in
[chrome://flags](chrome://flags) or running chrome from terminal with
`--enable-nacl` option.

You can check that OpenGL is working on this [native client example](http://gonativeclient.appspot.com/dev/demos/sdk_examples/fullscreen_tumbler/fullscreen_tumbler.html).
It should display a cube which can be rotated using mouse. Some GPUs are
blacklisted, you can try to bypass it with `--disable-gpu-blacklist` option.

How to play
-----------

Click on the image and the game will start in fullscreen. Clicking again will
pause the game and leave the fullscreen. Use your **mouse to look around**
and press **up arrow** or **W key** to move forward or push any box. If you
screw something up, you can restart the level with **R key**. When you
successfully complete the level, next level will be loaded. There are
currently 11 playable levels.

Why there is no...
------------------

Still work in progress :-)

Compiling and running the game
------------------------------

You can play the game online on the link provided above, but you can also
compile and run it on your own. You can run it either natively using SDL2 or in
NaCl itself (if everything needed by current version is ported there). In case
of NaCl you need these things:

 - [Native Client SDK](https://developers.google.com/native-client/beta/sdk/download).
   Version used here is `pepper_25`, i.e. unzip it and run `./naclsdk update pepper_25`.
 - [CMake](http://www.cmake.org) 2.8.8 or newer
 - [Corrade](https://github.com/mosra/corrade) and
   [Magnum](https://github.com/mosra/magnum) libraries compiled for NaCl. You
   can get the binaries [here](https://github.com/downloads/mosra/corrade/corrade-nacl-20121129-4577467.zip)
   and [here](https://github.com/downloads/mosra/magnum/magnum-nacl-20121129-029a480.zip).
   Unzip them and put the files in root of
   your NaCl SDK, e.g. `/home/johndoe/nacl_sdk/pepper_22`.
 - Corrade compiled for your system (needed for resource compiler). You can
   either compile it manually from https://github.com/mosra/corrade or use
   packages from [here](http://mosra.cz/blog/corrade.php) (outdated, but
   should be sufficient).

Get the toolchains submodule, if you don't have it already:

    cd /path/to/push-the-box
    git submodule init
    git sumbodule update

Don't forget to adapt `NACL_PREFIX` variable in `generic/NaCl-glibc-x86-32.cmake`
and `generic/NaCl-glibc-x86-64.cmake` to path where your SDK is installed,
e.g. `/home/johndoe/nacl_sdk/pepper_25`. You may need to adapt also
`NACL_TOOLCHAIN_PATH` so CMake is able to find the compiler.

Next create build directory for desired architecture (e.g. x86-64) and run
CMake. The toolchains needs access to its platform file, so be sure to
properly set absolute `CMAKE_MODULE_PATH` to `toolchains/modules/` directory
containing `Platform/NaCl.cmake`.

    mkdir -p build-nacl-x86-64
    cd build-nacl-x86-64
    cmake .. \
        -DCMAKE_MODULE_PATH="/path/to/push-the-box/toolchains/modules" \
        -DCMAKE_TOOLCHAIN_FILE="../toolchains/generic/NaCl-glibc-x86-64.cmake" \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/path/to/webserver/directory
    make

If you specified `CMAKE_INSTALL_PREFIX`, running `make install` will install
the game with all additional files and libraries to given directory in your
webserver and you can now run it from Chrome. Enjoy :-)

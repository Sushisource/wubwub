#SSMP (Working title)

Media Player

##Requirements
* [Qt framework](http://qt.nokia.com/downloads)
* [Taglib](https://github.com/taglib/taglib)
    compiled with MP4 support (-DWITH_MP4=ON)
* [irrKlang](http://www.ambiera.com/irrklang/)

##Building on a Mac
You'll somehow need a newer version of GCC that supports C++11. Clang wasn't working
as of this readme. You'll also need 32-bit QT binaries, as irrKlang does not support
64 bits.

KevinOS Notes:
==============

Compilation:
------------

+ `$ ./compile.sh` overwrites the filesystem of `/dev/sdb` -- be careful!

+ `$ qemu-system-i386 os-image` can be used for quicker testing. 

+ Testing is done with Oracle Virtualbox, QEMU, and a Lenovo T430. Development done on Ubuntu 12.04 with a standard GNU toolchain (GCC, LD, etc).

Features:
---------

KevinOS currently loads a 32bit C kernel in Protected Mode.  It detects all interrupt calls and has preliminary PIT-handling code.  Keypress detection for a standard laptop keyboard is complete and the OS boots into a _very_ basic shell.  Pseudorandom number generation implemented the same as the C standard document. A custom file system (KFS) is under development.

Unlicense:
----------

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>

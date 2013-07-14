KevinOS Notes:
==============

Compilation:
------------

+ `$ ./compile.sh` overwrites the filesystem of `/dev/sdb` -- be careful!

+ `$ qemu-system-i386 os-image` can be used for quicker testing. 

+ Testing is done with Oracle Virtualbox, QEMU, and a Lenovo T430. Development done on Ubuntu 12.04 with a standard GNU toolchain (GCC, LD, etc).

Features:
---------

KevinOS currently loads a 32bit C kernel in Protected Mode.  It detects all interrupt calls and has preliminary PIT-handling code.  Keypress detection for a standard laptop keyboard is complete and the OS boots into a _very_ basic shell.  Pseudorandom number generation implemented the same as the C standard document.

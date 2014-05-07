#!/bin/bash

buildstatus='unsuccessful'
use_new_bootloader=true
function finish {
    echo "Build was $buildstatus!"
}
trap finish EXIT
set -e

echo 'starting compilation'
rm os-image || true

echo 'running assembler'
nasm kernel_entry.asm -f elf -o kernel_entry.o
nasm int.asm -f elf -o int.o

echo 'building KFS'
./KFS_builder.py
nasm KFS_gen.asm -f bin -o KFS.bin
ld -melf_i386 -r -b binary -o KFS.o KFS.bin

echo "compiling kernel"
gcc -ffreestanding -m32 -c kernel.c -o kernel.o
ld -melf_i386 -o kernel.bin -Ttext 0x200 kernel_entry.o int.o kernel.o KFS.o --oformat binary --entry main
dd if=kernel.bin count=100 of=padded_kernel.bin conv=sync &> /dev/null && sync

echo "assembling bootloader"
if [ "$use_new_bootloader" = true ]; then
    nasm bootloader_stage1.asm -f bin -o stage1.bin
    nasm bootloader_stage2.asm -f bin -o stage2.bin
    cat stage1.bin stage2.bin > boot_sect.bin
else
    nasm boot_sect2.asm -f bin -o boot_sect.bin
fi

cat boot_sect.bin padded_kernel.bin > os-image

echo 'checking sector count'
actual=$(ls -l | grep "os-image" | cut -d' ' -f 7)

if [ "$use_new_bootloader" = true ]; then
    loaded=$(grep "NUM_SECTORS equ" < bootloader_stage2.asm | cut -d' ' -f 3)
    actual=$((actual/512-2))
else
    loaded=$(grep "NUM_SECTORS equ" < boot_sect2.asm | cut -d' ' -f 3)
    actual=$((actual/512-1))
fi


if [ "$loaded" -ne $((actual)) ]; then
    echo "In bootloader: $loaded"
    echo "Filesize: $actual"
    exit 1
fi

echo 'building floppy image'
rm floppy.img || true
dd if=/dev/zero of=floppy.img bs=512 count=2880 &> /dev/null && sync
dd if=os-image of=floppy.img conv=notrunc &> /dev/null && sync
if [ "$1" == "write" ]
then
	sudo dd if=floppy.img of=/dev/sdb conv=notrunc && sync
else
	qemu-system-i386 floppy.img -soundhw pcspk &
fi
buildstatus="successful"
rm *.o
#rm *.bin

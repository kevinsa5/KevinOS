#!/bin/bash
echo ''
echo 'starting compilation'
rm os-image
nasm boot_sect.asm -f bin -o boot_sect.bin
nasm kernel_entry.asm -f elf -o kernel_entry.o
nasm int.asm -f elf -o int.o
nasm KFS_creator.asm -f bin -o KFS.bin
ld -melf_i386 -r -b binary -o KFS.o KFS.bin

gcc -ffreestanding -m32 -c kernel.c -o kernel.o
ld -melf_i386 -o kernel.bin -Ttext 0x200 kernel_entry.o int.o kernel.o KFS.o --oformat binary
dd if=kernel.bin count=100 of=padded_kernel.bin conv=sync && sync
cat boot_sect.bin padded_kernel.bin > os-image
./sector_checker.py $(ls -l | grep "os-image")

qemu-system-i386 os-image

rm floppy.img
dd if=/dev/zero of=floppy.img bs=512 count=2880 && sync
dd if=os-image of=floppy.img conv=notrunc && sync
sudo dd if=floppy.img of=/dev/sdb conv=notrunc && sync

#sudo virtualbox
#qemu-system-i386 floppy.img
#rm *.o
#rm *.bin

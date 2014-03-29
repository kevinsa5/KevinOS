#!/bin/bash
echo 'starting compilation'
rm os-image
echo 'running assembler'
nasm boot_sect2.asm -f bin -o boot_sect.bin
nasm kernel_entry.asm -f elf -o kernel_entry.o
nasm int.asm -f elf -o int.o
echo 'building KFS'
./KFS_builder.py
nasm KFS_gen.asm -f bin -o KFS.bin
ld -melf_i386 -r -b binary -o KFS.o KFS.bin
echo "compiling kernel..."
gcc -ffreestanding -m32 -c kernel.c -o kernel.o
ld -melf_i386 -o kernel.bin -Ttext 0x200 kernel_entry.o int.o kernel.o KFS.o --oformat binary --entry main
dd if=kernel.bin count=100 of=padded_kernel.bin conv=sync &> /dev/null && sync
cat boot_sect.bin padded_kernel.bin > os-image
echo 'checking sector count'
./sector_checker.py $(ls -l | grep "os-image")
if [ $? -ne 0 ]
then
	exit 1
fi

echo 'building image'
rm floppy.img
dd if=/dev/zero of=floppy.img bs=512 count=2880 &> /dev/null && sync
dd if=os-image of=floppy.img conv=notrunc &> /dev/null && sync
#sudo dd if=floppy.img of=/dev/sdb conv=notrunc && sync

#sudo virtualbox
qemu-system-i386 floppy.img -soundhw pcspk
#rm *.o
#rm *.bin

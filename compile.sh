#!/bin/bash

buildstatus='unsuccessful'
use_new_bootloader=true
function finish {
    echo "Build was $buildstatus!"
}
trap finish EXIT
set -e

echo 'starting compilation'
rm -f os-image || true

echo 'running assembler'
nasm kernel_entry.asm -f elf -o kernel_entry.o
nasm int.asm -f elf -o int.o

echo 'building KFS'
./KFS_builder.py
nasm KFS_gen.asm -f bin -o KFS.bin
ld -melf_i386 -r -b binary -o KFS.o KFS.bin

echo "compiling kernel"
gcc -ffreestanding -m32 -c kernel.c -o kernel.o
ld -melf_i386 -o kernel.bin -Ttext 0x8000 kernel_entry.o int.o kernel.o KFS.o --oformat binary --entry main

sectorcount=100
size=$(wc -c < kernel.bin)
if ((size<$(($sectorcount*512)))) ; then
	dd if=kernel.bin count=$sectorcount of=padded_kernel.bin conv=sync &> /dev/null && sync
else
	echo "kernel image is truncated, you must increase the padding count!"
	exit 1
fi

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
actual=$(ls -l | grep "os-image" | awk '{print $5}')

if [ "$use_new_bootloader" = true ]; then
    loaded=$(grep "NUM_SECTORS equ" < bootloader_stage2.asm |awk '{print $3}')
    actual=$((actual/512-1))
else
    loaded=$(grep "NUM_SECTORS equ" < boot_sect2.asm | awk '{print $3}')
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
	echo "You will overwrite /dev/sdb: $(ls -l /dev/disk/by-id | grep sdb | cut -d ' ' -f 11) "
	read -p "Are you sure? (y/n)" -n 1 -r
	echo 
	if [[ $REPLY =~ ^[Yy]$ ]]
	then
		sudo dd if=floppy.img of=/dev/sdb conv=notrunc && sync
	else
		buildstatus="canceled"
		exit 2
	fi
else
	qemu-system-i386 floppy.img -soundhw pcspk &
fi

# increment buildData.c values
cp buildData.c buildData-temp.c
awk '/int buildID = [0-9]+;/ { printf "int buildID = %d;\n", $4+1 };!/int buildID = [0-9]+;/{print}' < buildData-temp.c > buildData.c
rm buildData-temp.c

buildstatus="successful"
rm *.o
rm *.bin

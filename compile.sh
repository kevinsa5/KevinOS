#!/bin/bash

buildstatus='unsuccessful'
function finish {
    echo "Build was $buildstatus!"
}
trap finish EXIT
set -e

rm -f os-image || true

echo 'Running assembler'
nasm kernel_entry.asm -f elf -o kernel_entry.o
nasm int.asm -f elf -o int.o

echo 'Building KFS'
./KFS_builder.py
nasm KFS_gen.asm -f bin -o KFS.bin
ld -melf_i386 -r -b binary -o KFS.o KFS.bin

echo "Compiling kernel files:"
printf "Compiling malloc.c... "
gcc -ffreestanding -m32 -c malloc.c -o malloc.o
printf "ok\n"
printf "Compiling shell_commands.c... "
gcc -ffreestanding -m32 -c shell_commands.c -o shell_commands.o
printf "ok\n"
printf "Compiling util.c... "
gcc -ffreestanding -m32 -c util.c -o util.o
printf "ok\n"
printf "Compiling kernel.c... "
gcc -ffreestanding -m32 -c kernel.c -o kernel.o
printf "ok\n"
printf "Compiling driver.c... "
gcc -ffreestanding -m32 -c driver.c -o driver.o
printf "ok\n"
printf "Compiling editor.c... "
gcc -ffreestanding -m32 -c editor.c -o editor.o
printf "ok\n"
printf "Compiling buildData.c... "
gcc -ffreestanding -m32 -c buildData.c -o buildData.o
printf "ok\n"
printf "Linking kernel files... "
ld -melf_i386 -o kernel.bin -Ttext 0x8000 kernel_entry.o int.o malloc.o shell_commands.o util.o driver.o editor.o buildData.o kernel.o KFS.o --oformat binary --entry main
printf "ok\n"

sectorcount=100
size=$(wc -c < kernel.bin)
if ((size<$(($sectorcount*512)))) ; then
	echo "Creating kernel image"
	dd if=kernel.bin count=$sectorcount of=padded_kernel.bin conv=sync &> /dev/null && sync
else
	echo "kernel image is truncated, you must increase the padding count!"
	exit 1
fi

printf "Assembling bootloader... "
nasm bootloader_stage1.asm -f bin -o stage1.bin
nasm bootloader_stage2.asm -f bin -o stage2.bin
cat stage1.bin stage2.bin > boot_sect.bin
cat boot_sect.bin padded_kernel.bin > os-image
printf "ok\n"

printf "Checking sector counts... "
actual=$(ls -l | grep "os-image" | awk '{print $5}')
actual=$((actual/512-1))

loaded=$(grep "NUM_SECTORS equ" < bootloader_stage2.asm |awk '{print $3}')

if [ "$loaded" -ne $((actual)) ]; then
	echo ""
    echo "In bootloader: $loaded"
    echo "Filesize: $actual"
    exit 1
else
	echo "Sector count ok"
fi

echo 'Building floppy image'
rm floppy.img || true
dd if=/dev/zero of=floppy.img bs=512 count=2880 &> /dev/null && sync
dd if=os-image of=floppy.img conv=notrunc &> /dev/null && sync
if [ "$1" == "write" ]; then
	echo "You will overwrite /dev/sdb:" 
	ls -l /dev/disk/by-id | grep sdb | awk '{print $9}'
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

echo "Build #$(grep 'buildID = ' < buildData.c | awk '{ print $4; }' | rev | cut -c 2- | rev)"

buildstatus="successful"
rm *.o
rm *.bin

echo "SLOC:$(wc -l bootloader_stage1.asm bootloader_stage2.asm boot.sh buildData.c compile.sh driver.c driver.h editor.c editor.h IDT.c int.asm kernel.c kernel_entry.asm kernel.h KFS_builder.py KFS_template.asm malloc.c malloc.h shell_commands.c shell_commands.h sh_exec.c sh_lisp.c util.c util.h | tail -n 1)"

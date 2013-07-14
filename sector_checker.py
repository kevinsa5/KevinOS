#!/usr/bin/python
# arg1: output of $ls -l | grep "os-image"
import sys

if len(sys.argv) < 2:
	print "no argument supplied to sector_checker.py"
	exit(1)

size = sys.argv[5]

num = None
with open("boot_sect.asm") as txt:
	for line in txt:
		if ";Change" in line:
			index = line.index(";Change")
			num = eval(line[index-2:index])
			print "loading %s sectors in boot_sect.asm" % (num)
		
if num == None:
	print "\n\n\n"
	print "COULD NOT FIND NUMBER OF SECTORS LOADED IN boot_sect.asm!"
	print "\n"
elif int(size)/512 != num + 1:
	print "\n\n\n"
	print "You need to change the number of sectors loading in boot_sect.asm (line 41?)"
	print "\n\n\n"

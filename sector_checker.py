#!/usr/bin/python
# arg1: output of $ls -l | grep "os-image"
import sys

filename = "boot_sect2.asm"

if len(sys.argv) < 2:
	print "no argument supplied to sector_checker.py"
	exit(1)

size = int(sys.argv[5])

num = None
lineNum = 0
with open(filename) as txt:
	for line in txt:
		lineNum += 1
		if "NUM_SECTORS" in line:
			num = int(line.split(" equ ")[1])
			print "loading %s sectors in %s" % (num, filename)
			break
		
if num == None:
	print "COULD NOT FIND NUMBER OF SECTORS LOADED IN %s!" % (filename)
	exit(1)
elif int(size)/512 != num + 1:
	print "You need to change the number of sectors loading in %s (line %s, change to %s sectors?)" %(filename,lineNum,int(size)/512-1)
	exit(2)
exit(0)

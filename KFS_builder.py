#!/usr/bin/python
import os
files = os.listdir("./files")
contents = {}
if len(files) > 94: 
	print "Too many files! Max number is 94, but {0} are present.".format(len(files))
	exit()
for i in files:
	if len(i) > 10:
		print "Filename is too long! Can only be ten characters, is currently {0} :{1}".format(len(i),i)
		exit()
with open("KFS_template.asm","r") as template, open("KFS_gen.asm","w") as out:
	for line in template:
		if "insert file data here" in line:
			sector = 0
			offset = 0
			for entry in files:
				#if entry.endswith(".bin"):
				#	with open(".files/{0}".format(entry), "rb") as temp:
				#		
				#	
				#	continue
				with open("./files/{0}".format(entry), "r") as temp:
					#contents[entry] = temp.read().rstrip().encode('string_escape')
					contents[entry] = temp.read().rstrip().replace('\n','\\n').replace('\t','\\t')
				paddingZeroes = ",0" * (10-len(entry))
				out.write("db '{0}'{1}\n".format(entry,paddingZeroes))
				out.write("db {0}\n".format(sector))
				hb = offset >> 8
				lb = offset - hb*256
				out.write("db {0},{1}\n".format(hb,lb))
				filesize = os.path.getsize("./files/" + entry)-1 #len(contents[entry])
				hhb = filesize >> 16
				hb = (filesize-hhb*256*256) >>8
				lb = (filesize-hhb*256*256-hb*256)
				out.write("db {0},{1},{2}\n".format(hhb,hb,lb))
				offset += filesize
				if offset >= 512:
					sector += offset / 512
					offset %= 512
				#sector += filesize / 256
				out.write("\n")
			out.write("times ({0}*16) db 0".format(94-len(files)))
		elif "insert file contents here" in line:
			for entry in files:
				out.write("db `")
				out.write(contents[entry])
				out.write("`\n")
			
		else:
			out.write(line)

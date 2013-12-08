org 0x0000

;KFS Master Table
;Structure		Value		# Bytes each	# items		total size (bytes)
;Signature		KFS Begin		9	1			9
;KFS Info		Version,etc		16	1			16
;FileName					10	94			940
;Which sector					1	94			94
;offset from sector				2	94			188
;size (bytes)					3	94			282
;Signature		KFS End			7	1			7


db 'KFS Begin'		;Signature (9 bytes)

db 1			;KFS Info (16 bytes)
times 15 db 0

db 'test1.txt',0	; filename (10 bytes)
db 0			; sector (1 byte)
db 0,0			; offset (2 bytes)
db 0,0,13		; filesize (3 bytes)

db 'test2.txt',0
db 0
db 0,13
db 0,0,21

db 'test3.txt',0
db 0
db 0,34
db 0,0,10

times (91*16) db 0

db 'KFS End'		;Signature (7 bytes)

; Files:

db 'Hello, World!'
db 'This is another file!'
db 'file three'

times (512-13-21) db 0

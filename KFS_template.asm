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

#insert file data here


db 'KFS End'		;Signature (7 bytes)

; Files:

#insert file contents here

times (512-13-21) db 0

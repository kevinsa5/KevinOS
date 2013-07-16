org 0x0000

;KFS Master Table				
;Structure		Value		# Bytes each	# items		total size (bytes)
;Signature		KFS Begin		9				1				9
;KFS Info		Version,etc		16				1				16
;FileName						10				94				940
;Which segment					1				94				94
;offset from segment			2				94				188
;size (bytes)					3				94				282
;Signature		KFS End			7				1				7


db 'KFS Begin'		;Signature (9 bytes)

db 1	            ;KFS Info (16 bytes)
times 15 db 0

times 94*4 db '1234'		;Allocation Table (94 * 16 = 1504 bytes)

db 'KFS End'		;Signature (7 bytes)

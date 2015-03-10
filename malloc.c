/*
	http://wiki.osdev.org/Memory_Map_%28x86%29
	roughly 30 KiB of free memory from 0x0500 to 0x7BFF
	the stack grows down from 0x7c00, so we'll leave that alone.
	
	upper memory: 0x00100000 -> 0x00EFFFFF  (14 MiB)
	0x100000 : 0x28E390 allocation table (1631120 bytes, 1.55 MiB)
	0x28E391 : 0xEFFFFF heap (13048942 bytes, 12.44 MiB)
	
	heaptable is a usage table, where the nth bit in the
	table is on or off to show if the nth byte in the heapblock 
	is allocated or not
*/


#define TOTAL_HEAP_MEMORY (0xEFFFFF - 0x100000)
#define TOTAL_ALLOCATABLE_MEMORY (0xEFFFFF - 0x28E391)
#define TOTAL_TABLE_MEMORY (0x28E391 - 0x100000)

char* heapTable1 = (char*) 0x100000;
char* heapBlock1 = (char*) 0x28E391;
int bytesAllocated = 0;

void* malloc(int size){
	int i,j;
	int n_consecutive_bytes = 0;
	int block_start = 0;
	for(i = 0; i < TOTAL_ALLOCATABLE_MEMORY; i++){
		for(j = 0; j < 8; j++){
			if(!(heapTable1[i] & (1 << j))){
				n_consecutive_bytes++;
			} else {
				n_consecutive_bytes = 0;
				block_start = i*8 + j + 1;
			}
			if(n_consecutive_bytes == size){
				goto end;
			}
		}
	}
	end:
	if(i*8+j >= TOTAL_ALLOCATABLE_MEMORY){
		return (void*) 0;
	} else {
		bytesAllocated += size;
		i = block_start/8;
		j = block_start - i*8;
		while(i*8 + j < block_start + size){
			while(i*8 + j < block_start + size && j < 8){
				heapTable1[i] |= (1 << j);
				j++;
			}
			j = 0;
			i++;
		}
		return (void*) (heapBlock1 + block_start);
	}
}

void free(void* pointer, int size){
	char* ptr = (char*) pointer;
	if(ptr < heapBlock1 || heapBlock1 + TOTAL_ALLOCATABLE_MEMORY < ptr){
		ttprintln("Invalid pointer passed to free()");
		return;
	}
	int i, j, block_start;
	bytesAllocated -= size;
	block_start = ptr - heapBlock1;
	i = block_start/8;
	j = block_start - i*8;
	while(i*8 + j < block_start + size){
		while(i*8 + j < block_start + size && j < 8){
			heapTable1[i] &= ~(1 << j);
			j++;
		}
		j = 0;
		i++;
	}
}

void clearAllocationTable(){
	int i;
	for(i = 0; i < TOTAL_TABLE_MEMORY; i++){
		heapTable1[i] = 0;
	}
}

int getBytesAllocated(){
	return bytesAllocated;
}
int getAvailableMemory(){
	return TOTAL_ALLOCATABLE_MEMORY;
}

double heapUsage(){
	return (100.0*bytesAllocated)/TOTAL_ALLOCATABLE_MEMORY;
}

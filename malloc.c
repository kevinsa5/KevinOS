
// http://wiki.osdev.org/Memory_Map_%28x86%29
// roughly 30 KiB of free memory from 0x0500 to 0x7BFF
// enough for now, might create more blocks in the future.

/*
	0x0500 to 0x12AC (3500 bytes) is a usage table, where the nth bit in the
	table is on or off to show if the nth byte in the heap is allocated or not
*/

#define TOTAL_HEAP_MEMORY (0x7BFF - 0x0500)
#define TOTAL_ALLOCATABLE_MEMORY (0x7BFF - 0x12AD)

char* heapTable1 = (char*) 0x0500;
char* heapBlock1 = (char*) 0x12AD;
long bytesAllocated;

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

double heapUsage(){
	return (100.0*bytesAllocated)/TOTAL_ALLOCATABLE_MEMORY;
}

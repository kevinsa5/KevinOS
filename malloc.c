
// http://wiki.osdev.org/Memory_Map_%28x86%29
// roughly 30 KiB of free memory from 0x0500 to 0x7BFF
// enough for now, might create more blocks in the future.

#define TOTAL_HEAP_MEMORY (0x7BFF - 0x0500)

char* heapBlock1 = (char*) 0x500;
int mallocIndex = 0;

void* malloc(int size){
	mallocIndex += size;
	return (void*) (heapBlock1 + (mallocIndex - size));
}

void free(void* ptr){
	// empty, for now
}

double heapUsage(){
	return (100.0*mallocIndex)/TOTAL_HEAP_MEMORY;
}

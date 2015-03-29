#ifndef MALLOC_H
#define MALLOC_H

void* malloc(int);
void free(void*,int);
void clearAllocationTable();
int getBytesAllocated();
int getAvailableMemory();
double heapUsage();

#endif

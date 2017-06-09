#ifndef MEM_H
#define MEM_H

void Memory_Init();
void *malloc(int size);
void free(void *ptr);
int memcmp(const void * ptr1, const void * ptr2, size_t num);
void printChart();
void printFreeSpaces();
char *memoryUsage(int mode);
char *memorySpan();

typedef struct _memTab{
  int offset;
  int size;
} memTab;

#endif

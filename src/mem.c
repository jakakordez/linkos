#include <stddef.h>
#include <stdint.h>
#include "mem.h"
#include "cli.h"
#include "string.h"
#include "debug.h"

char *heapTop;
memTab *mtTop; // Table of free chunks
const int heapSize = 104857600; // 100 MiB

void Memory_Init(){
  int heapTopLocation;
  asm( "movl $heap_top, %0"
    : "=a" (heapTopLocation)
  );
  heapTop = (char *)heapTopLocation;

  int memoryTopLocation;
  asm( "movl $mt_top, %0"
    : "=a" (memoryTopLocation)
  );
  mtTop = (memTab *)memoryTopLocation;

  mtTop[0].offset = (int)heapTop;
  mtTop[0].size = heapSize;
  mtTop[1].size = 0;
}

void *malloc(int size){
  dprintf("malloc ( %d )", size);
  int i = 0;
  int pos = -1;
  // Search for smallest possible chunk with capacity >= size+4
  while(mtTop[i].size != 0){ // Scan entire table
    if((pos == -1 || mtTop[i].size < mtTop[pos].size) && mtTop[i].size >= size+4) pos = i;
    i++;
  }
  if(pos < 0) return NULL; // Chunk not found
  char *end = (char *)(mtTop[pos].offset + mtTop[pos].size - size);
  mtTop[pos].size -= size+4;
  *((int *)(end)-1) = size;
  dprintf(" -> %d\n", (int)end);
  return (void *)(end);
}

void free(void *ptr){
  dprintf("free ( %d )\n", (int)ptr);
  int i = 0;
  int pos = -1;
  while(1){ // Find last free space before our pointer
    if(mtTop[i].size == 0 || mtTop[i].offset > (int)ptr){
      pos = i-1;
      break;
    }
    i++;
  }

  int *location = (int *)(mtTop[pos].offset + mtTop[pos].size); // Location of first chunk
  while(1){
    if(((int)location)+(*location)+4 > (int)ptr) break; // Memory chunk containing this pointer
    else location = (int *)(((int)location)+(*location)+4); // Check next chunk (add chunk size to current position)
  }

  int j = 0;
  while(mtTop[j].size > 0) j++; // Index for new free space
  
  // Insert new free space
  mtTop[j].offset = (int)location;
  mtTop[j].size = (*location)+4;

  for(int k = 0; k < j+1; k++){
    for(int l = k+1; l < j+1; l++){
      if(mtTop[l].offset < mtTop[k].offset){ // Sort memory table by pointer value
        int t1 = mtTop[l].offset;
        int t2 = mtTop[l].size;
        mtTop[l].offset = mtTop[k].offset;
        mtTop[l].size = mtTop[k].size;
        mtTop[k].offset = t1;
        mtTop[k].size = t2;
      }
    }
  }
  mtTop[j+1].size = 0;
  mtTop[j+1].offset = 0;

  int k = 0; // Writing index
  int r = 0; // Reading index
  while(r < j+1){ // Loop until the new free table entry
    if(mtTop[r].size == 0) r++;
    else {
      if(mtTop[k].offset + mtTop[k].size == mtTop[r+1].offset) {
        while(mtTop[k].offset + mtTop[k].size == mtTop[r+1].offset){
          mtTop[k].size += mtTop[r+1].size;
          mtTop[r+1].size = 0;
          r++;
        }
      }
      else{
        mtTop[k].offset = mtTop[r].offset;
        mtTop[k].size = mtTop[r].size;
      }
      r++;
      k++;
    }
  }
  mtTop[k].size = 0;
  mtTop[k].offset = 0;
}

int memcmp(const void * s1, const void * s2, size_t n){
  unsigned char u1, u2;

  for ( ; n-- ; s1++, s2++) {
    u1 = * (unsigned char *) s1;
    u2 = * (unsigned char *) s2;
    if ( u1 != u2) {
      return (u1-u2);
    }
  }
  return 0;
}

char *memoryUsage(int mode){
  int j = 0;
  int fr = 0;
  while(mtTop[j].size > 0){
    fr += mtTop[j].size;
    j++;
  }
  if(mode) return printf("%pB / %pB (%d\\%)", (double)(heapSize-fr), (double)heapSize, ((heapSize-fr)*10000)/heapSize);
  return printf("%d B / %d B (%d\\%)", heapSize-fr, heapSize, ((heapSize-fr)*10000)/heapSize);
}

char *memorySpan(){
  return printf("%d - %d", heapTop, heapTop+heapSize);
}

void printChart(){
  int w = 40;
  int h = 20;
  int step = heapSize/(w*h);
  int t = 0;
  for(int i = 0; i < 20; i++){
    cli_setcursor(38, i);
    for(int j = 0; j < 40; j++){
      int loc = (int)heapTop+(((i*40)+j)*step);
      cli_setcolor(make_color(0, 0));
      if(loc >= mtTop[t].offset+mtTop[t].size){
        cli_setcolor(make_color(6, 6));
        t++;
      }
      cli_print(" ");
    }
  }
}

void printFreeSpaces(){
  int j = 0;
  while(mtTop[j].size != 0){
    cli_print(printf("%d %d\n", mtTop[j].offset, mtTop[j].size));
    j++;
  }
}

#include <stddef.h>
#include <stdint.h>
#include "mem.h"
#include "cli.h"
#include "string.h"

char *heapTop;
int *mtTop;
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
  mtTop = (int *)memoryTopLocation;

  mtTop[0] = (int)heapTop;
  mtTop[1] = heapSize;
  mtTop[3] = 0;
}

void *malloc(int size){
  int i = 0;
  int pos = -1;
  while(1){
    if(mtTop[i*2+1] == 0) break;
    if((pos == -1 || mtTop[i*2+1] < mtTop[pos*2+1]) && mtTop[i*2+1] >= size+4) pos = i;
    i++;
  }
  if(pos < 0) return NULL;
  char *end = (char *)(mtTop[pos*2]+mtTop[pos*2+1]-size);
  mtTop[pos*2+1] -= size+4;
  *((int *)(end)-1) = size;
  return (void *)(end);
}

void free(void *ptr){

  int i = 0;
  int pos = -1;
  while(1){ // Find last free space before our pointer
    if(mtTop[i*2+1] == 0 || mtTop[i*2] > (int)ptr){
      pos = i-1;
      break;
    }
    i++;
  }

  int *location = (int *)(mtTop[pos*2]+mtTop[pos*2+1]); // Location of first chunk
  while(1){
    //cli_print(printf("A. chunk %d (%d) \n", (int)location, *location));
    if(((int)location)+*location+4 > (int)ptr) break; // Memory chunk containing this pointer
    else location = (int *)(((int)location)+(*location)+4); // Check next chunk (add chunk size to current position)
  }

  int j = 0;
  while(1){
    if(mtTop[j*2+1] == 0) break; // Index for new free space
    j++;
  }
  // Insert new free space
  mtTop[j*2] = (int)location;
  mtTop[j*2+1] = (*location)+4;

  for(int k = 0; k < j+1; k++){
    for(int l = k+1; l < j+1; l++){
      if(mtTop[l*2] < mtTop[k*2]){ // Sort memory table by pointer value
        int t1 = mtTop[l*2];
        int t2 = mtTop[l*2+1];
        mtTop[l*2] = mtTop[k*2];
        mtTop[l*2+1] = mtTop[k*2+1];
        mtTop[k*2] = t1;
        mtTop[k*2+1] = t2;
      }
    }
  }

  int k = 0;
  int r = 0;
  while(1){
    if(r >= j+1) break;
    if(mtTop[r*2+1] == 0) r++;
    else if(mtTop[r*2]+mtTop[r*2+1] == mtTop[(r+1)*2]) {
      while(mtTop[r*2]+mtTop[r*2+1] == mtTop[(r+1)*2]){
        mtTop[k*2+1] += mtTop[(r+1)*2+1];
        mtTop[(k+1)*2+1] = 0;
        r++;
      }
    }
    else{
      mtTop[k*2] = mtTop[r*2];
      mtTop[k*2+1] = mtTop[r*2+1];
    }
    r++;
    k++;
  }
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
  while(1){
    if(mtTop[j*2+1] == 0) break;
    fr += mtTop[j*2+1];
    j++;
  }
  if(mode) return printf("%pB / %pB (%d\\%)", (double)(heapSize-fr), (double)heapSize, ((heapSize-fr)*10000)/heapSize);
  return printf("%d B / %d B (%d\\%)", heapSize-fr, heapSize, ((heapSize-fr)*10000)/heapSize);
}

char *memorySpan(){
  return printf("%d - %d", heapTop, heapTop+heapSize);
}

void printFreeSpaces(){
  int j = 0;
  while(mtTop[j*2+1] != 0){
    cli_print(printf("%d %d\n", mtTop[j*2], mtTop[j*2+1]));
    j++;
  }
}

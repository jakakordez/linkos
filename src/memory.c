#include <stddef.h>
#include <stdint.h>
#include "windows.h"
#include "keyboard.h"
#include "cli.h"
#include "string.h"
#include "mem.h"

char *memory_title = "Memory";
void Memory_Draw(window *this){
  cli_setcolor(make_color(COLOR_BLACK, COLOR_LIGHT_GREY));
  cli_fill();
  cli_padding(1);
  int *v = this->Data;
  int r = 0;
  cli_setcursor(0, r);
  cli_print("Memory usage:");
  cli_setcursor(0, ++r);
  cli_print(memoryUsage(*v));
  cli_setcursor(0, ++r);
  cli_print("Memory span:");
  cli_setcursor(0, ++r);
  cli_print(memorySpan());
  r++;
  cli_setcursor(0, ++r);
  cli_print("Free spaces table:");
  cli_setcursor(0, ++r);
  printFreeSpaces();
  printChart();
}

void Memory_KeyPress(window *this, char key){
  int *v = this->Data;
  if(!RELEASED(key)){
    if(key == ESC){
      if(*v == 1) *v = 0;
      else *v = 1;
    }
  }
}

void Memory_Dispose(window *this){
  free(this->Data);
}

window *NEW_Memory(){
  window *r = (window *)malloc(sizeof(window));
  r->Title = memory_title;
  r->CurrentState = MAXIMIZED;
  r->Data = (int *)malloc(sizeof(int));
  r->Draw = Memory_Draw;
  r->Dispose = Memory_Dispose;
  r->KeyPress = Memory_KeyPress;
  return r;
}

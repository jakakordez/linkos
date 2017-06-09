#include <stddef.h>
#include <stdint.h>
#include "../windows.h"
#include "../cli.h"
#include "../string.h"
#include "../mem.h"
#include "../keyboard.h"
#include "../debug.h"

char *programs_title = "Programs";
void Programs_Draw(window *this){
  cli_setcolor(make_color(COLOR_BLACK, COLOR_LIGHT_GREY));
  cli_fill();
  cli_padding(1);
  int *n= (int *)this->Data;
  for(int i = 0; i < programCount;i++){
    if(*n == i) cli_setcolor(make_color(COLOR_WHITE, COLOR_DARK_GREY));
    else cli_setcolor(make_color(COLOR_BLACK, COLOR_LIGHT_GREY));
    cli_setcursor(0, i);
    cli_print(list[i].title);
  }
}

void Programs_KeyPress(window *this, char key){
  int *n= (int *)this->Data;
  if(!RELEASED(key)){
    if(key == S || key == DownArrow) *n = (*n+1)%programCount;
    else if(key == W || key == UpArrow) *n = (*n+programCount-1)%programCount;
    if(key == Return) Windows_Open(list[*n].new());
  }
}

window *NEW_Programs(){
  //alert("Info", "Program started!");
  window *r = (window *)malloc(sizeof(window));
  r->Title = programs_title;
  r->Data = (int *)malloc(sizeof(int));
  r->CurrentState = MAXIMIZED;
  r->KeyPress = Programs_KeyPress;
  r->Draw = Programs_Draw;
  r->Dispose = NULL;
  return r;
}

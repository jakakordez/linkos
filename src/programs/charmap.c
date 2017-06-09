#include <stddef.h>
#include <stdint.h>
#include "../windows.h"
#include "../cli.h"
#include "../string.h"
#include "../mem.h"

char *charmap_title = "Charmap";
void Charmap_Draw(window *this){
  this->Title = charmap_title;
  cli_setcolor(make_color(COLOR_BLACK, COLOR_WHITE));
  cli_fill();
  cli_padding(1);
  cli_setcursor(0, 0);
  for(int i = 0; i < 128; i++){
    if((char)i != '\n') cli_print(printf("%c ", (char)i));
	}
}

window *NEW_Charmap(){
  window *r = (window *)malloc(sizeof(window));
  r->Title = charmap_title;
  r->CurrentState = MAXIMIZED;
  r->Draw = Charmap_Draw;
  r->KeyPress = NULL;
  r->Dispose = NULL;
  return r;
}

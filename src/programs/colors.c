#include <stddef.h>
#include <stdint.h>
#include "../windows.h"
#include "../cli.h"
#include "../string.h"
#include "../mem.h"

char *colors_title = "Colors";
void Colors_Draw(window *this){
  this->Title = colors_title;
  cli_setcolor(make_color(COLOR_BLACK, COLOR_WHITE));
  cli_fill();
  cli_padding(1);
  cli_setcursor(0, 0);
  for(int i = 0; i < 16; i++){
    for(int j = 0; j < 16; j++){
      cli_setcolor(make_color(i, j));
      cli_print("AA");
    }
    cli_print("\n");
	}
}

window *NEW_Colors(){
  window *r = (window *)malloc(sizeof(window));
  r->Title = colors_title;
  r->CurrentState = MAXIMIZED;
  r->Draw = Colors_Draw;
  return r;
}

#include <stddef.h>
#include <stdint.h>
#include "../windows.h"
#include "../cli.h"
#include "../string.h"
#include "../mem.h"
#include "../keyboard.h"
#include "../debug.h"

typedef struct _terminal_Locals{
  char *data;
  int position;
} terminal_Locals;

char *terminal_title = "Terminal";
void Terminal_Draw(window *this){
  terminal_Locals *l = this->Data;
  cli_setcolor(make_color(COLOR_WHITE, COLOR_BLACK));
  cli_fill();
  cli_setcursor(0, 0);
  cli_print(printf("/ $ %d %s_", l->data, l->data));
}
int Shift = 0;
void Terminal_KeyPress(window *this, char key){
  terminal_Locals *l = this->Data;
  //l->position += strcpy(printf("%d ", key), &(l->data[l->position]));
  if(!RELEASED(key)){
    if((Key)key == BackSpace) {
      l->position --;
      l->data[l->position] = '\0';
    }
    else if (PRINTABLE(key)){
      if(SHIFT_DOWN) l->data[l->position] = KeyMAPU[key&0x7F];
      else l->data[l->position] = KeyMAPL[key&0x7F];
      l->position ++;
    }
  }
  //cli_print(printf("-%d-", l->position));
}

void Terminal_Dispose(window *this){
  dprintf("Terminal_Dispose()\n");
  terminal_Locals *l = this->Data;
  free(l->data);
  free(this->Data);
}

window *NEW_Terminal(){
  dprintf("NEW_Terminal()\n");
  window *r = (window *)malloc(sizeof(window));
  r->Title = terminal_title;
  r->CurrentState = MAXIMIZED;
  terminal_Locals *locals = malloc(sizeof(terminal_Locals));

  locals->data = malloc(10000);
  locals->position = 0;
  r->Data = locals;
  r->Draw = Terminal_Draw;
  r->KeyPress = Terminal_KeyPress;
  r->Dispose = Terminal_Dispose;
  return r;
}

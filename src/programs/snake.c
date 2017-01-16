#include <stddef.h>
#include <stdint.h>
#include "../windows.h"
#include "../cli.h"
#include "../string.h"
#include "../mem.h"
#include "../keyboard.h"

typedef struct _vector{
  int x;
  int y;
} vector;

typedef struct _snake_Locals{
  vector *snk;
  int length;
  int head;
  int direction;
  vector candy;
} snake_Locals;

char *snake_title = "Snake";
void Snake_Draw(window *this){
  snake_Locals *l = this->Data;
  cli_setcolor(make_color(COLOR_WHITE, COLOR_BLACK));
  cli_fill();

  cli_setcursor(l->candy.x, l->candy.y);
  cli_print("@");

  for(int i = l->head; i >= l->head-l->length; i--){
    cli_setcursor(l->snk[i].x, l->snk[i].y);
    cli_print("*");
  }
}

void Snake_KeyPress(window *this, char key){
  snake_Locals *l = this->Data;
  if(!RELEASED(key)){
    switch(key){
      case LeftArrow:
        direction = (direction+270)%360;
      break;
      case RightArrow:
        direction = (direction+450)%360;
      break;
    }
  }
}

void Snake_Dispose(window *this){
  snake_Locals *l = this->Data;
  free(l->snk);
  free(this->Data);
}

window *NEW_Snake(){
  window *r = (window *)malloc(sizeof(window));
  r->Title = snake_title;
  r->CurrentState = MAXIMIZED;
  snake_Locals *locals = malloc(sizeof(snake_Locals));

  locals->snk = malloc(sizeof(vector)*200);
  locals->candy.x = 5;
  locals->candy.y = 4;
  locals->direction = 180;
  locals->head = 0;
  locals->length = 1;
  locals->snk[0].x = 1;
  locals->snk[0].y = 1;
  r->Data = locals;
  r->Draw = Snake_Draw;
  r->KeyPress = Snake_KeyPress;
  r->Dispose = Snake_Dispose;
  return r;
}

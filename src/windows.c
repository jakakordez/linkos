#include <stddef.h>
#include <stdint.h>
#include "cli.h"
#include "string.h"
#include "keyboard.h"
#include "mem.h"
#include "windows.h"
#include "debug.h"

#define windowCount 6
int openedWindows;
int focusedWindow;
window *windows[windowCount];
uint8_t WindowColor, TaskbarColor, SelectedTaskColor, SelectedWindowColor;
program list[programCount];

int alertOpen;
char alert_title[20];
char alert_message[30];

void alert(char *title, char *msg){
  strcpy(title, alert_title);
  strcpy(msg, alert_message);
  alertOpen = 1;
}

void Windows_Init(){
  for(int i = 0; i < windowCount; i++){
    windows[i] = NULL;
  }
  focusedWindow = 0;
  TaskbarColor = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	SelectedTaskColor = make_color(COLOR_WHITE, COLOR_BLACK);
	WindowColor = make_color(COLOR_BLACK, COLOR_LIGHT_GREY);
	SelectedWindowColor = make_color(COLOR_BLACK, COLOR_BLUE);

  list[0].title = terminal_title;
  list[0].new = NEW_Terminal;
  list[1].title = charmap_title;
  list[1].new = NEW_Charmap;
  list[2].title = memory_title;
  list[2].new = NEW_Memory;
  list[3].title = colors_title;
  list[3].new = NEW_Colors;
  //list[4].title = snake_title;
  //list[4].new = NEW_Snake;

  //alert_title = (char *)malloc(20*sizeof(char));
  //alert_message = (char *)malloc(30*sizeof(char));
}

void Windows_Open(window * new){
  dprintf("Windows_Open(window *%d)\n", (int)new);
  if(openedWindows == windowCount) return;
  focusedWindow = openedWindows;
  windows[openedWindows++] = new;
}

void Close_Window(window *w){
  if(w->Dispose != NULL) w->Dispose(w);
  free(w);
  int n = 0;
  for(int i = 0; i < windowCount; i++){
    if(windows[i] == w) {
      n = 1;
      windows[i] = NULL;
    }
    if(n && i < windowCount-1){
      windows[i] = windows[i+1];
    }
  }
  openedWindows--;
  focusedWindow = 0;
}

void DrawTaskbar(){
  cli_setcolor(TaskbarColor);
  cli_setcursor(0, 24);
  cli_print(" ");
	for(int i = 0; i < 6; i++){
		cli_setcolor(TaskbarColor);
		int pad = 12;
		if(windows[i] != NULL){
			if(i == focusedWindow) cli_setcolor(SelectedTaskColor);
			cli_print(windows[i]->Title);
			pad -= strlen(windows[i]->Title);
		}
		for(int j = pad; j > 0; j--){
			cli_print(" ");
		}
	}
  cli_print(" 12:45 ");
}

void DrawWindows(){
	if(windows[focusedWindow] != NULL){
    cli_setcolor(SelectedWindowColor);
    switch(windows[focusedWindow]->CurrentState){
      case MAXIMIZED:
        cli_setcursor(0, 0);
        cli_print(" ");
  			cli_print(windows[focusedWindow]->Title);
        cli_repeat(' ', 79-strlen(windows[focusedWindow]->Title));
      break;
      case LEFT_HALF:
        cli_setcursor(0, 0);
        cli_print(" ");
        cli_print(windows[focusedWindow]->Title);
        cli_repeat(' ', 39-strlen(windows[focusedWindow]->Title));
      break;
      case RIGHT_HALF:
        cli_setcursor(40, 0);
        cli_print(" ");
        cli_print(windows[focusedWindow]->Title);
        cli_repeat(' ', 39-strlen(windows[focusedWindow]->Title));
      break;
    }
    windowBox(windows[focusedWindow]);
    windows[focusedWindow]->Draw(windows[focusedWindow]);
	}
  if(alertOpen){
    cli_setbox((cli_box){.x=24, .y=8, .w=32, .h=6});
    cli_setcolor(make_color(COLOR_BLACK, COLOR_WHITE));
    cli_fill();
    int l = strlen(alert_title)/2;
    cli_border('-', '|');
    cli_setcursor(15-l, 0);
    cli_print(printf(" %s ", alert_title));
    l = strlen(alert_message)/2;
    cli_setcursor(15-l, 2);
    cli_print(printf(" %s ", alert_message));

    cli_setcursor(14, 4);
    cli_setcolor(make_color(COLOR_WHITE, COLOR_BLACK));
    cli_print(" OK ");
  }
}

int leftBorder(window *this){
  return (this->CurrentState == RIGHT_HALF)?40:0;
}
int rightBorder(window *this){
  return (this->CurrentState == LEFT_HALF)?80:40;
}

void windowBox(window * this){
  switch(this->CurrentState)
  {
    case RIGHT_HALF:
      cli_setbox((cli_box){.x=40, .y=1, .w=40, .h=23});
    break;
    case LEFT_HALF:
      cli_setbox((cli_box){.x=0, .y=1, .w=40, .h=23});
    break;
    default:
      cli_setbox((cli_box){.x=0, .y=1, .w=80, .h=23});
    break;
  }
}

void KeyPress(char key){
  if(alertOpen) {
    if(key == Return || key == ESC){
      alertOpen = 0;
    }
  }
  else if(ALT_DOWN){
    if(key == Tab){
      if(SHIFT_DOWN) focusedWindow = (focusedWindow+openedWindows-1)%openedWindows;
      else focusedWindow = (focusedWindow+1)%openedWindows;
    }
    else if(key == F4){
      Close_Window(windows[focusedWindow]);
    }
  }
  else{
    window *w = windows[focusedWindow];
    if(w->KeyPress != NULL) w->KeyPress(w, key);
  }
}

#ifndef WINDOWS
#define WINDOWS
#include "cli.h"
typedef enum _Window_State{
  MAXIMIZED,
  LEFT_HALF,
  RIGHT_HALF
} Window_State;

typedef struct _window{
  char *Title;
  Window_State CurrentState;
  void *Data;
  void (*Draw)(struct _window *);
  void (*KeyPress)(struct _window *, char key);
  void (*Dispose)(struct _window *);
} window;

int leftBorder(window *);
int rightBorder(window *);
void windowBox(window *);

void Windows_Init();
void Windows_Open(window *);
void DrawTaskbar();
void DrawWindows();
void KeyPress(char key);
void alert(char *title, char *msg);

typedef struct _program{
  char *title;
  window *(*new)(void);
} program;

#define programCount    4

window *NEW_Programs();
window *NEW_Terminal();
extern char *terminal_title;
window *NEW_Charmap();
extern char *charmap_title;
window *NEW_Memory();
extern char *memory_title;
window *NEW_Colors();
extern char *colors_title;
window *NEW_Snake();
extern char *snake_title;

extern program list[programCount];

#endif

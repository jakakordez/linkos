#ifndef CLI
#define CLI
#include <stddef.h>
#include <stdint.h>

/* Hardware text mode color constants. */
enum vga_color {
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
};

typedef struct _cli_box{
	int x;
	int y;
	int w;
	int h;
} cli_box;

void cli_initialize(void);
void cli_writestring(const char* data);
void cli_print(char *data);
void cli_putAt(char c, uint8_t color, size_t x, size_t y);
uint8_t make_color(enum vga_color fg, enum vga_color bg);
void cli_setcursor(size_t x, size_t y);
void cli_setcolor(uint8_t color);
void cli_repeat(char c, int n);
void cli_fill();
void cli_padding(int padding);
void cli_fullscreen();
void cli_setbox(cli_box b);
void cli_border(char main, char side);
#endif

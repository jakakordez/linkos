#include <stdarg.h>
#include "cli.h"
#include "string.h"

cli_box bbox;

uint8_t make_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

uint16_t make_vgaentry(char c, uint8_t color) {
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

int cli_row;
int cli_column;
uint8_t cli_color;
uint16_t* cli_buffer;

void cli_initialize() {
	cli_row = 0;
	cli_column = 0;
	cli_color = make_color(COLOR_BLUE, COLOR_GREEN);
	cli_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			cli_buffer[index] = make_vgaentry(' ', cli_color);
		}
	}
	cli_fullscreen();
}

void cli_setbox(cli_box b){
	bbox.x = b.x;
	bbox.y = b.y;
	bbox.h = b.h;
	bbox.w = b.w;
}

void cli_fullscreen(){
	bbox = (cli_box){ .x = 0, .y = 0, .w = 80, .h = 25 };
}

void cli_padding(int padding){
	bbox.x +=padding;
	bbox.y +=padding;
	bbox.h -=padding*2;
	bbox.w -=padding*2;
}

void cli_setcolor(uint8_t color) {
	cli_color = color;
}

void cli_putAt(char c, uint8_t color, size_t i, size_t j) {
	const size_t index = (bbox.y+j) * VGA_WIDTH + (bbox.x+i);
	cli_buffer[index] = make_vgaentry(c, color);
}

void cli_setcursor(size_t x, size_t y){
	cli_column = x;
	cli_row = y;
}

void cli_putchar(char c) {
	if(c == '\n'){
		cli_column = 0;
		cli_row++;
		return;
	}
	cli_putAt(c, cli_color, cli_column, cli_row);
	if (++cli_column == bbox.w) {
		cli_column = 0;
		if (++cli_row == bbox.h) {
			cli_row = 0;
		}
	}
}

void cli_fill(){
	cli_setcursor(0, 0);
	cli_repeat(' ', bbox.w*bbox.h);
}

void cli_repeat(char c, int n){
	for(int j = n; j > 0; j--) cli_putchar(c);
}

void cli_writestring(const char* data) {
	size_t datalen = strlen(data);
	for (size_t i = 0; i < datalen; i++)
		cli_putchar(data[i]);
}

void cli_border(char main, char side){
	cli_setcursor(0, 0);
	cli_repeat(main, bbox.w);
	cli_setcursor(0, bbox.h-1);
	cli_repeat(main, bbox.w);
	for(int i = 1; i < bbox.h-1; i++){
		cli_setcursor(0, i);
		cli_putchar(side);
		cli_setcursor(bbox.w-1, i);
		cli_putchar(side);
	}
}

void cli_print(char *data){
	cli_writestring(data);
}

#include <stddef.h>
#include <stdint.h>
#include "cli.h"
#include "keyboard.h"
#include "string.h"
#include "mem.h"
#include "windows.h"
#include "debug.h"

/* Check if the compiler thinks we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif

void kernel_main() {
	cli_initialize();
	Memory_Init();
	Windows_Init();
	Debug_Init();
	dprintf("Debugger started\n");
	Windows_Open(NEW_Programs());
	//Open_Window(NEW_Memory());
	//Open_Window(NEW_Terminal());
	//cli_print(printf("%d %d %f %f", 0, 5, 2.0, 1.0));
	//while(1);
	while(1){
		dprintf("Main while\n");
		cli_fullscreen();
		DrawTaskbar();
		DrawWindows();
		//cli_print(printf("   %d   ", malloc(1000)));
		dprintf("Keypress\n");
		KeyPress(Keyboard_Scan());
	}
}

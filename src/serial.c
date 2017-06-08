#include "serial.h"

void init_serial(int comPort) {
   outb(comPort + 1, 0x00);    // Disable all interrupts
   outb(comPort + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outb(comPort + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   outb(comPort + 1, 0x00);    //                  (hi byte)
   outb(comPort + 3, 0x03);    // 8 bits, no parity, one stop bit
   outb(comPort + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outb(comPort + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

int serial_received(int comPort) {
   return inb(comPort + 5) & 1;
}
 
char read_serial(int comPort) {
   while (serial_received(comPort) == 0);
   return inb(comPort);
}

int is_transmit_empty(int comPort) {
   return inb(comPort + 5) & 0x20;
}
 
void write_serial(int comPort, char a) {
   while (is_transmit_empty(comPort) == 0);
   outb(comPort,a);
}
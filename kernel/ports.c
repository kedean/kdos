#include "../kernel/ports.h"

uint8_t port_byte_in(uint16_t port){
	uint8_t result;
	__asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
	return result;
}

void port_byte_out(uint16_t port, uint8_t data){
	__asm__("out %%al, %%dx" : :"a" (data), "d" (port));
}

uint8_t port_word_in(uint16_t port){
	uint8_t result;
	__asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
	return result;
}

void port_word_out(uint16_t port, uint8_t data){
	__asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}

void io_wait(){
	__asm__ volatile ( "outb %%al, $0x80" : : "a"(0));
}

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

/* reinitialize the PIC controllers, giving them specified vector offsets
   rather than 8h and 70h, as configured by default */
 
#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */
 
#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */
 
/*
arguments:
	offset1 - vector offset for master PIC
		vectors on the master become offset1..offset1+7
	offset2 - same for slave PIC: offset2..offset2+7
*/
void PIC_remap(uint8_t offset1, uint8_t offset2)
{
	unsigned char a1, a2;
 
	a1 = port_byte_in(PIC1_DATA);                        // save masks
	a2 = port_byte_in(PIC2_DATA);
 
	port_byte_out(PIC1_COMMAND, ICW1_INIT+ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	port_byte_out(PIC2_COMMAND, ICW1_INIT+ICW1_ICW4);
	io_wait();
	port_byte_out(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	io_wait();
	port_byte_out(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	io_wait();
	port_byte_out(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	port_byte_out(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();
 
	port_byte_out(PIC1_DATA, ICW4_8086);
	io_wait();
	port_byte_out(PIC2_DATA, ICW4_8086);
	io_wait();
 
	port_byte_out(PIC1_DATA, a1);   // restore saved masks.
	port_byte_out(PIC2_DATA, a2);
}
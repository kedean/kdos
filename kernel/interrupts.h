#pragma once

typedef struct interrupt_data interrupt_data_s;
typedef enum known_interrupts known_interrupts_e;
typedef void (*callback_ptr)(interrupt_data_s*);
typedef struct idt_entry idt_entry_s;
typedef struct idt_ptr idt_ptr_s;

/*
 * The interrupts we support customizing, technically this should encompass all interrupts
 */
enum known_interrupts{
	INT_DOUBLE_FAULT = 8,
	INT_TIMER = 32,
	INT_KEYBOARD = 33,
	INT_CMOS = 40,
	INT_PAGE_FAULT = 14,
	INT_SYSCALL = 0x30
};

/*
 * This defines what the stack looks like after an ISR was running
 */
struct interrupt_data
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */
};

/*
 * An entry in the interrupt descriptor table
 */
struct idt_entry{
	unsigned short base_low;
	unsigned short segment;
	unsigned char always0;
	unsigned char flags;
	unsigned short base_high;
} __attribute__((packed));

/*
 * A pointer to an entry in the IDT, formatted for the CPU
 */
struct idt_ptr{
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));

/*
 * Turn on interrupts globally
 */
void interrupts_enable();
/*
 * Turn off interrupts globally
 */
void interrupts_disable();
/*
 *Create and initialize the platform-specific interrupt data, to be called before registering any interrupts
 */
void interrupts_init();
/*
 * Registers a callback to a specific interrupt number
 * The interrupt number must be a value from known_interrupts_e
 */
void interrupts_register_callback(known_interrupts_e int_num, callback_ptr callback);

/*
 * Boilerplate!
 */

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

extern void si128();

#define CODE_SEG 0x08

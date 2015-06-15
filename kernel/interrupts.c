#include "../kernel/system.h"
#include "../memory/memory.h"
#include "../io/screen_io.h"
#include "../kernel/ports.h"

/*
 *The IDT has 256 entries, the first 32 are hardware exceptions.
 */
idt_entry_s idt[256];

/*
 * This array will store function pointers for each interrupt callback routine
 */
callback_ptr callbacks[256];

/*
 * Sets an entry in the IDT
 */
static void idt_set_gate(uint8_t num, uint64_t base, uint16_t segment, uint8_t flags){
	idt_entry_s* entry = (idt_entry_s*) &(idt[num]);
	entry->base_high = (base >> 16) & 0xffff;
	entry->base_low = base & 0xffff;
	entry->segment = segment;
	entry->flags = flags;
	entry->always0 = 0;
}

static void irq_remap(void)
{
    port_byte_out(0x20, 0x11);
    port_byte_out(0xA0, 0x11);
    port_byte_out(0x21, 0x20);
    port_byte_out(0xA1, 0x28);
    port_byte_out(0x21, 0x04);
    port_byte_out(0xA1, 0x02);
    port_byte_out(0x21, 0x01);
    port_byte_out(0xA1, 0x01);
    port_byte_out(0x21, 0x00);
    port_byte_out(0xA1, 0x00);
}

/*
 * Set up the interrupt descriptor table
 */
static void idt_init()
{
	idt_ptr_s idtp;
	idtp.limit = (sizeof(idt_entry_s) * 256) - 1;
	idtp.base = (unsigned int) &idt;

	/*Clear it all out*/
	memset((char*) &idt, 0, sizeof(idt_entry_s)*256);

	/*load into memory*/
	/*calls the lidt instruction on the location of the new idt*/
	__asm__ __volatile__("lidt (%0)" : : "p"(&idtp));
}

/*
 * Boilerplate to initialize all isr and irq callbacks to point to their service routines
 */
static void install_basic_routines()
{
		//isr's
    idt_set_gate(0, (unsigned)isr0, CODE_SEG, 0x8E);
    idt_set_gate(1, (unsigned)isr1, CODE_SEG, 0x8E);
    idt_set_gate(2, (unsigned)isr2, CODE_SEG, 0x8E);
    idt_set_gate(3, (unsigned)isr3, CODE_SEG, 0x8E);
    idt_set_gate(4, (unsigned)isr4, CODE_SEG, 0x8E);
    idt_set_gate(5, (unsigned)isr5, CODE_SEG, 0x8E);
    idt_set_gate(6, (unsigned)isr6, CODE_SEG, 0x8E);
    idt_set_gate(7, (unsigned)isr7, CODE_SEG, 0x8E);

    idt_set_gate(8, (unsigned)isr8, CODE_SEG, 0x8E);
    idt_set_gate(9, (unsigned)isr9, CODE_SEG, 0x8E);
    idt_set_gate(10, (unsigned)isr10, CODE_SEG, 0x8E);
    idt_set_gate(11, (unsigned)isr11, CODE_SEG, 0x8E);
    idt_set_gate(12, (unsigned)isr12, CODE_SEG, 0x8E);
    idt_set_gate(13, (unsigned)isr13, CODE_SEG, 0x8E);
    idt_set_gate(14, (unsigned)isr14, CODE_SEG, 0x8E);
    idt_set_gate(15, (unsigned)isr15, CODE_SEG, 0x8E);

    idt_set_gate(16, (unsigned)isr16, CODE_SEG, 0x8E);
    idt_set_gate(17, (unsigned)isr17, CODE_SEG, 0x8E);
    idt_set_gate(18, (unsigned)isr18, CODE_SEG, 0x8E);
    idt_set_gate(19, (unsigned)isr19, CODE_SEG, 0x8E);
    idt_set_gate(20, (unsigned)isr20, CODE_SEG, 0x8E);
    idt_set_gate(21, (unsigned)isr21, CODE_SEG, 0x8E);
    idt_set_gate(22, (unsigned)isr22, CODE_SEG, 0x8E);
    idt_set_gate(23, (unsigned)isr23, CODE_SEG, 0x8E);

    idt_set_gate(24, (unsigned)isr24, CODE_SEG, 0x8E);
    idt_set_gate(25, (unsigned)isr25, CODE_SEG, 0x8E);
    idt_set_gate(26, (unsigned)isr26, CODE_SEG, 0x8E);
    idt_set_gate(27, (unsigned)isr27, CODE_SEG, 0x8E);
    idt_set_gate(28, (unsigned)isr28, CODE_SEG, 0x8E);
    idt_set_gate(29, (unsigned)isr29, CODE_SEG, 0x8E);
    idt_set_gate(30, (unsigned)isr30, CODE_SEG, 0x8E);
    idt_set_gate(31, (unsigned)isr31, CODE_SEG, 0x8E);

    //irq's
    idt_set_gate(32, (unsigned)irq0, CODE_SEG, 0x8E);
    idt_set_gate(33, (unsigned)irq1, CODE_SEG, 0x8E);
    idt_set_gate(34, (unsigned)irq2, CODE_SEG, 0x8E);
    idt_set_gate(35, (unsigned)irq3, CODE_SEG, 0x8E);
    idt_set_gate(36, (unsigned)irq4, CODE_SEG, 0x8E);
    idt_set_gate(37, (unsigned)irq5, CODE_SEG, 0x8E);
    idt_set_gate(38, (unsigned)irq6, CODE_SEG, 0x8E);
    idt_set_gate(39, (unsigned)irq7, CODE_SEG, 0x8E);

    idt_set_gate(40, (unsigned)irq8, CODE_SEG, 0x8E);
    idt_set_gate(41, (unsigned)irq9, CODE_SEG, 0x8E);
    idt_set_gate(42, (unsigned)irq10, CODE_SEG, 0x8E);
    idt_set_gate(43, (unsigned)irq11, CODE_SEG, 0x8E);
    idt_set_gate(44, (unsigned)irq12, CODE_SEG, 0x8E);
    idt_set_gate(45, (unsigned)irq13, CODE_SEG, 0x8E);
    idt_set_gate(46, (unsigned)irq14, CODE_SEG, 0x8E);
    idt_set_gate(47, (unsigned)irq15, CODE_SEG, 0x8E);

		//syscall
		idt_set_gate(INT_SYSCALL, (unsigned)si128, CODE_SEG, 0x8E);
}


/**
 * All isr's and irq's point here before passing on to their callbacks, if one exists
 */
void interrupt_handler(interrupt_data_s* r)
{
	/*
	 * The messages for known exceptions
	 */
	const char *exception_messages[] =
	{
	    "Division By Zero",
	    "Debug",
	    "Non Maskable Interrupt",
	    "Breakpoint",
	    "Into Detected Overflow",
	    "Out of Bounds",
	    "Invalid Opcode",
	    "No Coprocessor",

	    "Double Fault",
	    "Coprocessor Segment Overrun",
	    "Bad TSS",
	    "Segment Not Present",
	    "Stack Fault",
	    "General Protection Fault",
	    "Page Fault",
	    "Unknown Interrupt",

	    "Coprocessor Fault",
	    "Alignment Check",
	    "Machine Check",
	    "Reserved",
	    "Reserved",
	    "Reserved",
	    "Reserved",
	    "Reserved",

	    "Reserved",
	    "Reserved",
	    "Reserved",
	    "Reserved",
	    "Reserved",
	    "Reserved",
	    "Reserved",
	    "Reserved"
	};

	uint_t num_exceptions_known = sizeof(exception_messages) / sizeof(const char *);

	if(callbacks[r->int_no] != 0){
		__asm__ __volatile__("nop" : : : "%ecx");
		__asm__ __volatile__("nop" : : : "%ebx");
		port_byte_out(0x20, 0x20);
		callbacks[r->int_no](r);
	} else if(r->int_no == INT_TIMER){
		//this means the timer fired before we actually set it up, this is ok!
	} else if (r->int_no < num_exceptions_known){
  	printf("\n-- %s Exception: %d. System Halted! --\n", exception_messages[r->int_no], r->int_no);
  	halt();
  } else{
  	printf("\n-- Unknown Exception #%u --\n", r->int_no);
  	halt();
  }
}

void interrupts_register_callback(known_interrupts_e int_num, callback_ptr callback){
	callbacks[int_num] = callback;
}

void interrupts_enable(){
	__asm__ __volatile__ ("sti");
}

void interrupts_disable(){
	__asm__ __volatile__ ("cli");
}

void interrupts_init(){
	irq_remap();
	PIC_remap(0x20, 0x28);
	idt_init();
	install_basic_routines();
	memset(callbacks, 0, sizeof(callbacks));
}

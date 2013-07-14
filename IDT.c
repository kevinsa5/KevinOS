extern void int_general();		// Default interrupt routine
extern void int_0();
extern void int_1();
extern void int_2();
extern void int_3();
extern void int_4();
extern void int_5();
extern void int_6();
extern void int_7();
extern void int_8();
extern void int_9();
extern void int_10();
extern void int_11();
extern void int_12();
extern void int_13();
extern void int_14();
extern void int_15();
extern void int_16();
extern void int_17();
extern void int_18();
extern void int_19();
extern void int_20();
extern void int_21();
extern void int_22();
extern void int_23();
extern void int_24();
extern void int_25();
extern void int_26();
extern void int_27();
extern void int_28();
extern void int_29();
extern void int_30();
extern void int_31();
extern void int_32();
extern void int_33();			// Keyboard interrupt
extern void int_34();
extern void int_35();
extern void int_36();
extern void int_37();
extern void int_38();
extern void int_39();
extern void int_40();
extern void int_41();
extern void int_42();
extern void int_43();
extern void int_44();
extern void int_45();
extern void int_46();
extern void int_47();

struct idtstruct {
	unsigned short offset1;
	unsigned short selector;
	unsigned char unused;
	unsigned char flag;
	unsigned short offset2;
} __attribute__((packed));		// Packed to avoid padding

struct idtpointer {
	unsigned short size;
	unsigned int offset;	
} __attribute__((packed));		// Packed to avoid padding


static struct idtstruct idttable[256];	// Define the idt struct

void remappic();
void isr_general();
void isr_0();
void isr_1();
void isr_2();
void isr_3();
void isr_4();
void isr_5();
void isr_6();
void isr_7();
void isr_8();
void isr_9();
void isr_10();
void isr_11();
void isr_12();
void isr_13();
void isr_14();
void isr_15();
void isr_16();
void isr_17();
void isr_18();
void isr_19();
void isr_20();
void isr_21();
void isr_22();
void isr_23();
void isr_24();
void isr_25();
void isr_26();
void isr_27();
void isr_28();
void isr_29();
void isr_30();
void isr_31();
void isr_32();
void isr_33();
void isr_34();
void isr_35();
void isr_36();
void isr_37();
void isr_38();
void isr_39();
void isr_40();
void isr_41();
void isr_42();
void isr_43();
void isr_44();
void isr_45();
void isr_46();
void isr_47();
void idt_entry(unsigned int entry, void* offset, unsigned short selector, unsigned char flag);

void idt_init()
{
	unsigned int i;
	remappic();
	struct idtpointer idt;			// Contains the struct being loaded by lidt

	idt_entry(0, &int_0, 0x08, 0x8E);	// Flag indicates an interrupt gate
	idt_entry(1, &int_1, 0x08, 0x8E);
	idt_entry(2, &int_2, 0x08, 0x8E);
	idt_entry(3, &int_3, 0x08, 0x8E);
	idt_entry(4, &int_4, 0x08, 0x8E);
	idt_entry(5, &int_5, 0x08, 0x8E);
	idt_entry(6, &int_6, 0x08, 0x8E);
	idt_entry(7, &int_7, 0x08, 0x8E);
	idt_entry(8, &int_8, 0x08, 0x8E);
	idt_entry(9, &int_9, 0x08, 0x8E);
	idt_entry(10, &int_10, 0x08, 0x8E);
	idt_entry(11, &int_11, 0x08, 0x8E);
	idt_entry(12, &int_12, 0x08, 0x8E);
	idt_entry(13, &int_13, 0x08, 0x8E);
	idt_entry(14, &int_14, 0x08, 0x8E);
	idt_entry(15, &int_15, 0x08, 0x8E);
	idt_entry(16, &int_16, 0x08, 0x8E);
	idt_entry(17, &int_17, 0x08, 0x8E);
	idt_entry(18, &int_18, 0x08, 0x8E);
	idt_entry(19, &int_19, 0x08, 0x8E);
	idt_entry(20, &int_20, 0x08, 0x8E);
	idt_entry(21, &int_21, 0x08, 0x8E);
	idt_entry(22, &int_22, 0x08, 0x8E);
	idt_entry(23, &int_23, 0x08, 0x8E);
	idt_entry(24, &int_24, 0x08, 0x8E);
	idt_entry(25, &int_25, 0x08, 0x8E);
	idt_entry(26, &int_26, 0x08, 0x8E);
	idt_entry(27, &int_27, 0x08, 0x8E);
	idt_entry(28, &int_28, 0x08, 0x8E);
	idt_entry(29, &int_29, 0x08, 0x8E);
	idt_entry(30, &int_30, 0x08, 0x8E);
	idt_entry(31, &int_31, 0x08, 0x8E);
	idt_entry(32, &int_32, 0x08, 0x8E);
	idt_entry(33, &int_33, 0x08, 0x8E);
	idt_entry(34, &int_34, 0x08, 0x8E);
	idt_entry(35, &int_35, 0x08, 0x8E);
	idt_entry(36, &int_36, 0x08, 0x8E);
	idt_entry(37, &int_37, 0x08, 0x8E);
	idt_entry(38, &int_38, 0x08, 0x8E);
	idt_entry(39, &int_39, 0x08, 0x8E);
	idt_entry(40, &int_40, 0x08, 0x8E);
	idt_entry(41, &int_41, 0x08, 0x8E);
	idt_entry(42, &int_42, 0x08, 0x8E);
	idt_entry(43, &int_43, 0x08, 0x8E);
	idt_entry(44, &int_44, 0x08, 0x8E);
	idt_entry(45, &int_45, 0x08, 0x8E);
	idt_entry(46, &int_46, 0x08, 0x8E);
	idt_entry(47, &int_47, 0x08, 0x8E);

	for(i = 48; i < 256; i++)				// Fill in the remaining entries with a standard isr
	{
		idt_entry(i, &int_general, 0x08, 0x8E);
	}

	idt.size = sizeof(idttable) - 1;		// Size of the IDT
	idt.offset = (unsigned int)&idttable;		// Pointer to the IDT

	asm("lidt (%0)" : : "p"(&idt));			// Load the IDT struct
	asm("sti" : :);					// Activate interrupts
}

void idt_entry(unsigned int entry, void* offset, unsigned short selector, unsigned char flag)
{
	unsigned int offsetinteger = (unsigned int)offset;	// Store pointer into an integer for later processing
	idttable[entry].offset1 = offsetinteger & 0xFFFF;		
	idttable[entry].selector = selector;
	idttable[entry].unused = 0;
	idttable[entry].flag = flag;
	idttable[entry].offset2 = (offsetinteger >> 16) & 0xFFFF;
}

void isr_general(){
	writeByteToPort(0x20, 0x20);
	ttprintln("isr_general was called");
}
void isr_0(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_0 : Divide Error was called");
}
void isr_1(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_1 : Debug Exceptions was called");
}
void isr_2(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_2 : Intel Reserved was called");
}
void isr_3(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_3 : Breakpoint was called");
}
void isr_4(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_4 : Overflow was called");
}
void isr_5(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_5 : Bounds Check was called");
}
void isr_6(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_6 : Invalid Opcode was called");
}
void isr_7(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_7 : Coprocessor Not Available was called");
}
void isr_8(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_8 : Double Fault was called");
}
void isr_9(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_9 : Coprocessor Segment Overrun was called");
}
void isr_10(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_10 : Invalid TSS was called");
}
void isr_11(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_11 : Segment Not Present was called");
}
void isr_12(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_12 : Stack Exception was called");
}
void isr_13(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_13 : General Protection Exception (Triple Fault) was called");
}
void isr_14(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_14 Page Fault was called");
}
void isr_15(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_15 Intel Reserved was called");
}
void isr_16(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_16 Coprocessor Error was called");
}
void isr_17(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_17 was called");
}
void isr_18(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_18 was called");
}
void isr_19(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_19 was called");
}
void isr_20(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_20 was called");
}
void isr_21(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_21 was called");
}
void isr_22(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_22 was called");
}
void isr_23(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_23 was called");
}
void isr_24(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_24 was called");
}
void isr_25(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_25 was called");
}
void isr_26(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_26 was called");
}
void isr_27(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_27 was called");
}
void isr_28(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_28 was called");
}
void isr_29(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_29 was called");
}
void isr_30(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_30 was called");
}
void isr_31(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_31 was called");
}

void isr_32(){
	writeByteToPort(0x20,0x20);
	pitCall();
	//ttprintln("isr_32 (IRQ 0: Programmable Interrupt Timer) was called");
}

void isr_33(){
	//keyboard
	unsigned char scancode = readByteFromPort(0x60);
	keyPressed(scancode);
	writeByteToPort(0x20, 0x20);
}
void isr_34(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_34 (IRQ 2: Cascade [used by PICS]) was called");
}
void isr_35(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_35 (IRQ 3: COM2) was called");
}
void isr_36(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_36 (IRQ 4: COM1) was called");
}
void isr_37(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_37 (IRQ 5: LPT2) was called");
}
void isr_38(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_38 (IRQ 6: Floppy Disk) was called");
}
void isr_39(){
	writeByteToPort(0x20,0x20);
	ttprintln("isr_39 (IRQ 7: LPT1 / spurious interrupt) was called");
}
void isr_40(){
	writeByteToPort(0xA0,0x20);
	writeByteToPort(0x20,0x20);
	ttprintln("isr_40 (IRQ 8: CMOS real-time clock) was called");
}
void isr_41(){
	writeByteToPort(0xA0,0x20);
	writeByteToPort(0x20,0x20);
	ttprintln("isr_41 (IRQ 9: Free for peripherals) was called");
}
void isr_42(){
	writeByteToPort(0xA0,0x20);
	writeByteToPort(0x20,0x20);
	ttprintln("isr_42 (IRQ 10: Free for peripherals) was called");
}
void isr_43(){
	writeByteToPort(0xA0,0x20);
	writeByteToPort(0x20,0x20);
	ttprintln("isr_43 (IRQ 11: Free for peripherals) was called");
}
void isr_44(){
	writeByteToPort(0xA0,0x20);
	writeByteToPort(0x20,0x20);
	ttprintln("isr_44 (IRQ 12: PS2 Mouse) was called");
}
void isr_45(){
	writeByteToPort(0xA0,0x20);
	writeByteToPort(0x20,0x20);
	ttprintln("isr_45 (IRQ 13: FPU / Coprocessor / Inter-processor) was called");
}
void isr_46(){
	writeByteToPort(0xA0,0x20);
	writeByteToPort(0x20,0x20);
	ttprintln("isr_46 (IRQ 14: Primary ATA Hard Disk) was called");
}
void isr_47(){
	writeByteToPort(0xA0,0x20);
	writeByteToPort(0x20,0x20);
	ttprintln("isr_47 (IRQ 15: Secondary ATA Hard Disk) was called");
}


void remappic(){
	writeByteToPort(0x20, 0x11);		// Initilisation instruction
	writeByteToPort(0xA0, 0x11);
		
	writeByteToPort(0x21, 0x20);		// Map the first 8 interrupts to 0x20
	writeByteToPort(0xA1, 0x28);		// Map 8 - 15 interrupts to 0x28

	writeByteToPort(0x21, 0x04);		// Tell the pic how its connected 
	writeByteToPort(0xA1, 0x02);		
	
	writeByteToPort(0x21, 0x01);		// Tell the mode it is operating in
	writeByteToPort(0xA1, 0x01);

	//writeByteToPort(0x21, 0xfd);
	//writeByteToPort(0xA1, 0xff);
	writeByteToPort(0x21, 0x0);
	writeByteToPort(0xA1, 0x0);
}


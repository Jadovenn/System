/**
 * isr.h - Interface for high level interrupt service routines
 * 	   cpu register structur
 * System sources under license MIT
 */

#ifndef ISR_H_
#define ISR_H_

#include <stdint.h>

typedef struct Cpu_registers
{
	uint32_t ds;                           // datasegment selector
	uint32_t edi, esi, ebp, esp;           // pusha
	uint32_t ebx, edx, ecx, eax;           // pusha
	uint32_t interr_nb, err_code;          // interrupt nb and error
	uint32_t eip, cs, eflags, useresp, ss; // pushed by the processor
} Cpu_registers_t;

typedef void (*isr_t)(Cpu_registers_t);

void  Cpu_dump_registers(Cpu_registers_t regs);
int   Cpu_register_interrupt_handler(uint8_t, isr_t);
isr_t Cpu_get_interrupt_handler(uint32_t key);
int   Cpu_idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

#define IRQ_0  32
#define IRQ_1  33
#define IRQ_2  34
#define IRQ_3  35
#define IRQ_4  36
#define IRQ_5  37
#define IRQ_6  38
#define IRQ_7  39
#define IRQ_8  40
#define IRQ_9  41
#define IRQ_10 42
#define IRQ_11 43
#define IRQ_12 44
#define IRQ_13 45
#define IRQ_14 46
#define IRQ_15 47

#endif // ISR_H_

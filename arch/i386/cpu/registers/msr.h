/**
 * msr.h - Model Specific Registers
 * System sources under license MIT
 */

#ifndef _MSR_H
#define _MSR_H

#include <stdint.h>

#define IA32_APIC_BASE_MSR 0x1B

void Cpu_read_msr(uint32_t msr, uint32_t* hi, uint32_t* low);
void Cpu_write_msr(uint32_t msr, uint32_t hi, uint32_t low);

#endif // _MSR_H

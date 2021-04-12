/**
 * msr.c - Model Specific Registers
 * System sources under license MIT
 */

#include "cpu/registers/msr.h"

void Cpu_read_msr(uint32_t msr, uint32_t* hi, uint32_t* lo) { __asm__("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr)); }

void Cpu_write_msr(uint32_t msr, uint32_t hi, uint32_t lo) { __asm__("wrmsr" : : "a"(lo), "d"(hi), "c"(msr)); }

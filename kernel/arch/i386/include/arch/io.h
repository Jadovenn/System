/**
 * io.h - x86 input/output PIC registers
 * System sources under license MIT
 */

#ifndef IO_H_
#define IO_H_

// Monitor Related I/O ports
#define VGA_SCREEN_CTRL 0x3d4
#define VGA_DATA_REG    0x3d5

// PIC 8259 Related I/O ports and command
// 8259 Programmable Interrupt Controller
#define PIC1         0x20 // i/o base address for master PIC
#define PIC2         0xa0 // i/o base for slave PIC
#define PIC1_COMMAND PIC1
#define PIC1_DATA    (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA    (PIC2 + 1)
#define PIC_EOI      0x20
#define ICW1_INIT    0x10 // init mode
#define ICW1_ICW4    0x01
#define ICW4_8086    0x01 // MCS-80/85

// Programable Interval Timer PIT
#define PIT_COMMAND 0x43
#define PIT_DATA    0x40
#define PIT_REFRESH 0x36

#endif // IO_H_

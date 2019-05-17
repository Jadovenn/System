/**
 * Programable Interupt Controller 8259
 */

#ifndef PIC_H_
# define PIC_H_

void	PIC_disable();
void	PIC_send_EOI(unsigned char irq);

#endif // PIC_H_


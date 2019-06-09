/**
 * ports.h - x86 r/w ports API
 * System sources under license MIT
 */

#ifndef PORTS_H_
# define PORTS_H_

//	read a byte from the specified port
unsigned char	port_read_byte(unsigned short port);

//	write a byte to the specified port
void 		port_write_byte(unsigned short port, unsigned char data);

//	read a word from the specified port
unsigned short	port_read_word(unsigned short port);

//	write a short to the specified port
void 		port_write_word(unsigned short port, unsigned short data);

//	wait for an i/o operation to complete
void		io_irq_wait(void);

#endif // PORTS_H_


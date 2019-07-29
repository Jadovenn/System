/**
 * string.h - C library string
 * System sources under license MIT
 */

#ifndef STRING_H_
# define STRING_H_

#include <stddef.h>

/**
 * @details copy n bytes from src to dest.
 * @param dest {void*} - pointer to destination memory
 * @param src  {const void*} - pointer to source memory
 * @param size {unsigned long} - number of byte to copy
 * @return {void*} - original value of dest
 */
void	*memcpy(void *dest, const void *src, unsigned long size);

/**
 * @details set n bytes with values byte to dest
 * @warning the memory area must be at least size long
 * @param dest {void*} - pointer to destination memory
 * @param byte {int} - value of the byte, (converter in unsigned char)
 * @param size {unsigned long} - number of byte to set
 */
void	*memset(void *dest, int byte, unsigned long size);
void	*memccpy(void *restrict s1, const void *restrict s2, int c, size_t n);

size_t	strlen(const char *s);
char	*strstr(const char *s1, const char *s2);

#endif // STRING_H_


/**
 * string.h - C library string
 * System sources under license MIT
 */

#ifndef STRING_H_
#define STRING_H_

#include <stddef.h>

/**
 * @details copy n bytes from src to dest.
 * @param dest {void*} - pointer to destination memory
 * @param src  {const void*} - pointer to source memory
 * @param size {unsigned long} - number of byte to copy
 * @return {void*} - original value of dest
 */
void* memcpy(void* dest, const void* src, unsigned long size);

/**
 * @details set n bytes with values byte to dest
 * @warning the memory area must be at least size long
 * @param dest {void*} - pointer to destination memory
 * @param byte {int} - value of the byte, (converter in unsigned char)
 * @param size {unsigned long} - number of byte to set
 */
void* memset(void* dest, int byte, unsigned long size);

/**
 * @details - shall copy bytes from memory area s2 into s1,
 * 	stopping after the first occurrence of byte c or after
 * 	n bytes are copied.
 * @param s1 {void *restrict} - pointer to memory area
 * @param s2 {void *restrict} - pointer to another memory area
 * @param c  {int} - byte to stop at first occurence
 * @param n  {size_t} - max number of bytes to copy
 * @attention s1 and s2 shall not overlap, if it is, behavior is undefined
 */
void* memccpy(void* restrict s1, const void* restrict s2, int c, size_t n);

/**
 * @brief shall compute the number of bytes in the string
 * 	to which s points, not including the terminating NUL character.
 * @param s {const char *} - string
 * @return number of byte in string
 */
size_t strlen(const char* s);

/**
 * @brief shall locate the first occurrence in the string
 * 	pointed to by s1 of the sequence of bytes
 * @param s1 {const char *} - first string
 * @param s2 {const char *} - second string, memory area could overlap
 * @return {char *} - shall return a pointer to the located string
 * 	or a null pointer if the string is not found.
 * 	If s2 points to a string with zero length, the function shall return s1
 */
char* strstr(const char* s1, const char* s2);

#endif // STRING_H_

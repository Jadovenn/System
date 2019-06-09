/**
 * string.h - C library string
 * System sources under license MIT
 */

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



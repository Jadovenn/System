/**
 * memccpy.c - copy n byte before occurance
 * System sources under license MIT
 */

#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <bitset.h>

void	*memccpy(void *restrict target, const void *restrict source, int c, size_t n) {
	uint32_t *word_ptr_target = NULL;
	uint32_t *word_ptr_source = NULL;
	uint8_t  *byte_ptr_target = target;
	uint8_t	 *byte_ptr_source = source;
	uint8_t	 character = (uint8_t)c;
	unsigned idx = 0;
	void	 *occurence_ptr = NULL;

	while ((n - idx) % 4 != 0) { // Read first byte
		byte_ptr_target[idx] = byte_ptr_source[idx];
		if (byte_ptr_source[idx] == character) {
			occurence_ptr = byte_ptr_target + idx + 1;
			return occurence_ptr;
		}
		idx += 1;
	}
	word_ptr_target = byte_ptr_target + idx; // use aligned pointer
	word_ptr_source = byte_ptr_source + idx;
	for (; idx < n; idx += 4) { // read word by word
		uint32_t chunck = word_ptr_source[idx / 4];
		if (_has_byte(chunck, character)) { // if char is found, go byte by byte and return
			while (idx < n) { 
				byte_ptr_target[idx] = byte_ptr_source[idx];
				if (byte_ptr_source[idx] == character) {
					occurence_ptr = byte_ptr_target + idx + 1;
					return occurence_ptr;
				}
				idx += 1;
			}
		} else { // else go word by word
			word_ptr_target[idx / 4] = chunck;
		}
	}
	return NULL;
}


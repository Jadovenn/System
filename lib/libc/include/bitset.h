/**
 * bitset.h - Bitset arithmetics
 * System sources under license MIT
 */

#ifndef BITSET_H_
# define BITSET_H_

#ifdef __i386__

# define _has_zero(word)	((word - 0x01010101UL) & ((~word) & 0x80808080UL))
# define _has_byte(word, byte)  (_has_zero(((word) ^ ((~0UL)/255 * (byte)))))

#endif // __i386__

#endif // BITSET_H_


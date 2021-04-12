/**
 * bitset.h - Bitset arithmetics
 * System sources under license MIT
 */

#ifndef BITSET_H_
#define BITSET_H_

#ifdef __i386__

#define _has_zero(dword)       ((dword - 0x01010101UL) & ((~dword) & 0x80808080UL))
#define _has_byte(dword, byte) (_has_zero(((dword) ^ ((~0UL) / 255 * (byte)))))

#define _has_zero_16(word) ((word - (unsigned short)0x0101) & ((~word) & (unsigned short)0x8080))

#define _set_bit(dword, bit)   (dword |= (1 << bit))
#define _unset_bit(dword, bit) (dword &= ~(1 << bit))

#endif // __i386__

#endif // BITSET_H_

/*
 *    crc32.c - calculate crc32 checksums of data
 *
 *    Originally written in 2020 by Ayman El Didi and modified to suit the
 *    needs of this project.
 *
 *    To the extent possible under law, the author(s) have dedicated all
 *    domain worldwide. This software is distributed without any warranty.
 *
 *    You should have received a copy of the CC0 Public Domain Dedication along
 *    with this software. If not, see
 *    <http://creativecommons.org/publicdomain/zero/1.0/>.
 */

#include "global.h"
#include "crc32.h"

u32 crc32_for_byte(u32 byte) {
    const u32 polynomial = 0xEDB88320L;
    u32 result = byte;

    for (size_t i = 0; i < 8; i++) {
        /* IMPLEMENTATION: the code below always shifts result right by
         * 1, but only XORs it by the polynomial if we're on the lowest
         * bit.
         *
         * This is because 1 in binary is 00000001, so ANDing the
         * result by 1 will always give 0 unless the lowest bit is set.
         * And since XOR by zero does nothing, the other half only
         * occurs when we're on the lowest bit.
         *
         * I didn't leave the above implementation in, despite being
         * faster on my machine since it is a more complex operation
         * which may be slower on less sophisticated processors. It can
         * be added in in place of the loop code below.
         */

        result = (result >> 1) ^ (result & 1) * polynomial;

        /* Here is the code I replaced with the branch I tried to
         * remove:
        if (result & 1) {
            result = (result >> 1) ^ polynomial;
            continue;
        }
        result >>= 1;
         */
    }
    return result;
}

u32 crc32(const void *input, size_t size) {
    const u8 *current = input;
    u32 result = 0xFFFFFFFF;

    for (size_t i = 0; i < size; i++) {
        result ^= current[i];
        result = crc32_for_byte(result);
    }

    return ~result;
}

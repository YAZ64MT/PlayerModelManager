/*
 *    crc32.h - calculate crc32 checksums of data
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

// This implementation has been modified to use the typedefs in 

#include "ultra64.h"

/* Calculate crc32 checksum of size_t size bytes of const void *input and
 * return the result.
 *
 * No lookup tables are used, its simply a byte by byte crc32 algorithm.
 */

u32 crc32(const void *input, size_t size);

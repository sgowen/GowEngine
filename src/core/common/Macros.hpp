//
//  Macros.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#define UNUSED(x) (void)(x)
#define BUFFER_OFFSET(i) ((void*)(i))

/// Bit Flag Utils
#define SET_BIT(val, bit, set) val = (val & ~bit) | (set ? bit : 0)
#define IS_BIT_SET(val, bit) (val & bit)

/// Number Utils
#define GOW_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define GOW_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define CLAMP(x, lower, upper) (GOW_MIN(upper, GOW_MAX(x, lower)))

/// Handy for determining num bits required given a specific value
#define NBITS2(n)  ((n&2)?1:0)
#define NBITS4(n)  ((n&(0xC))?(2+NBITS2(n>>2)):(NBITS2(n)))
#define NBITS8(n)  ((n&0xF0)?(4+NBITS4(n>>4)):(NBITS4(n)))
#define NBITS16(n) ((n&0xFF00)?(8+NBITS8(n>>8)):(NBITS8(n)))
#define NBITS32(n) ((n&0xFFFF0000)?(16+NBITS16(n>>16)):(NBITS16(n)))
#define NBITS(n)   (n==0?0:NBITS32(n)+1)

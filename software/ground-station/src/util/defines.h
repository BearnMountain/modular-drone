#ifndef	DEFINES_H_ 
#define	DEFINES_H_ 

#include <stdint.h>

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;
typedef int32_t  i32;
typedef int16_t  i16;
typedef int8_t   i8;
typedef float	 f32;
typedef double 	 f64;

template <typename T>
constexpr T CLAMP(T v, T lo, T hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

#endif

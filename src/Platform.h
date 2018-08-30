
#ifndef __CU_PLATFORM_H_
#define __CU_PLATFORM_H_

#define CU_USE_EXTERNAL_TYPES

#ifdef CU_USE_EXTERNAL_TYPES
    #include <stdint.h>
#elif

    typedef signed char             int8_t;
    typedef short int               int16_t;
    typedef int                     int32_t;
    typedef long long int           int64_t;

    typedef unsigned char           uint8_t;
    typedef unsigned short int      uint16_t;
    typedef unsigned int            uint32_t;
    typedef unsigned long long int  uint64_t;

#endif


#if defined(_WIN32) || defined(_MSC_VER)

    #define INLINE inline
    #define FORCEINLINE __forceinline

#elif defined(__GNUC__) || defined(__clang__)

    #define INLINE inline
    #define FORCEINLINE __attribute__((always_inline))

#endif

#ifndef RESTRICT
	#define RESTRICT __restrict
#endif

#endif /* __CU_PLATFORM_H_ */

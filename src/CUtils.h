
#ifndef __CU_CUTILS_H_
#define __CU_CUTILS_H_

#ifdef __CU_UTILS_DEBUG
    #define ERROR(x) printf(x)
#else
    #define ERROR(x) do {} while(0)
#endif

#endif /* CUTILS_H_ */

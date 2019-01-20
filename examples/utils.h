#ifndef __Z_UTILS_H__
#define __Z_UTILS_H__

#include <time.h>
#include <stdint.h>
#include <sys/time.h>


static uint64_t
z_us(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);

    uint64_t ts = (tv.tv_sec) * 1000000 + (tv.tv_usec);

    return ts;
}


#endif // __Z_UTILS_H__

#ifndef IP_UTILS_H
#define IP_UTILS_H
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    union
    {
        uint32_t ipv4;
        uint8_t ipv6;
    } address;
    int callCount;
    bool isIpv6;
    uint8_t flags;
} ipEntry;

typedef struct
{
    ipEntry *data;
    int size;
    int capacity;
} dynArray;

void dynArray_free(dynArray *arr);
int addIp(dynArray *arr, const char *ip);

#endif /*IP_UTILS_H*/

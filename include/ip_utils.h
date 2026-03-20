#ifndef IP_UTILS_H
#define IP_UTILS_H
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    union
    {
        uint32_t ipv4;
        uint8_t ipv6[16];
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
int dynArray_init(dynArray *arr);
int dynArray_push(dynArray *arr, ipEntry entry);
int addIp(dynArray *arr, const char *ip);

#ifdef __cplusplus
}
#endif
#endif /*IP_UTILS_H*/

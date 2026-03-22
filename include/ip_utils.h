#ifndef IP_UTILS_H
#define IP_UTILS_H
#include <netinet/in.h>
#include <stdint.h>
#include <stdbool.h>
#include <microhttpd.h>
typedef struct
{
    union
    {
        struct in_addr ipv4;
        struct in6_addr ipv6;
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
int addIp(dynArray *arr, const union MHD_ConnectionInfo *info);
//int addIp(dynArray *arr, const char *ip);

#endif /*IP_UTILS_H*/

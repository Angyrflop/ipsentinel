// Copyright (c) goes to Jan Oliver Quant
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdint.h>
#include <openssl/evp.h>
#include <sys/socket.h>
#include "config_ip.h"
#include "ip_utils.h" 

enum IpFlags{
        IP_FLAG_NONE = 0,
        IP_FLAG_INTERNAL = 1 << 0,      /*1*/
        IP_FLAG_WHITELISTED = 1 << 1,   /*2*/
        IP_FLAG_BLACKLISTED = 1 << 2,   /*4*/
        IP_FLAG_MALICIOUS = 1 << 3,     /*8*/
        IP_FLAG_SCANNER = 1 << 4,       /*16*/
};

static void ipEntry_init(ipEntry *entry)
{
        entry->isIpv6 = false;
        entry->flags = IP_FLAG_NONE;
        entry->callCount = 0;
}

int dynArray_init(dynArray *arr)
{
        arr->capacity = START_CAPACITY;
        arr->size = 0;
        ipEntry *tmp = (ipEntry *)malloc(arr->capacity * sizeof(ipEntry));
        if (tmp == NULL) {
                printf("[MEMORY] Unable to start array\n");
                return -1;
        }
        arr->data = tmp;
        return 0;
}

int dynArray_push(dynArray *arr, ipEntry entry)
{
        if (arr->size == arr->capacity) {
                arr->capacity *= 2;
                ipEntry *tmp = (ipEntry *)realloc(arr->data, arr->capacity * sizeof(ipEntry));
                if (tmp == NULL) {
                        printf("[MEMORY] Not expanding dynArray. Blocking incoming requests\n");
                        return -1;
                }
                arr->data = tmp;
        }
        arr->data[arr->size] = entry;
        arr->size++;
        return 0;
}

void dynArray_free(dynArray *arr)
{
        free(arr->data);
        arr->data = NULL;
        arr->size = 0;
        arr->capacity = 0;
}
       /*For testing again, will improve*/
void checkCallCount(ipEntry *entry)
{
        if (entry->callCount >= MAX_CALLCOUNT)
                entry->flags |= IP_FLAG_SCANNER | IP_FLAG_BLACKLISTED; /*20*/
}

int addIp(dynArray *arr, const char *ip)
{
    ipEntry entry;
    ipEntry_init(&entry);
    if (inet_pton(AF_INET, ip, &entry.address.ipv4) == 1) { 
        dynArray_push(arr, entry);
        return 0;
    }
    if (inet_pton(AF_INET6, ip, &entry.address.ipv6) == 1) {
        entry.isIpv6 = true;
        dynArray_push(arr, entry);
        return 0;
    }
    return -1;
}


// Copyright (c) goes to Jan Oliver Quant
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdint.h>
#include "config_ip.h"

enum IpFlags{
        IP_FLAG_NONE = 0,
        IP_FLAG_INTERNAL = 1 << 0,      /*1*/
        IP_FLAG_WHITELISTED = 1 << 1,   /*2*/
        IP_FLAG_BLACKLISTED = 1 << 2,   /*4*/
        IP_FLAG_MALICIOUS = 1 << 3,     /*8*/
        IP_FLAG_SCANNER = 1 << 4,       /*16*/
};

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
} ipHandler;    /*24 bytes*/

struct dynArray
{
        ipHandler *data;
        int size;
        int capacity;
};

void IpHandler_init(ipHandler *h)
{
        h->isIpv6 = false;
        h->flags = IP_FLAG_NONE;
        h->callCount = 0;
}

void dynArray_init(struct dynArray *arr)
{
        arr->capacity = START_CAPACITY;
        arr->size = 0;
        ipHandler *tmp = (ipHandler *)malloc(arr->capacity * sizeof(ipHandler));
        if (tmp == NULL) {
                printf("[MEMORY] Unable to start array");
                return;
        }
        arr->data = tmp;
}

void dynArray_push(struct dynArray *arr, ipHandler entry)
{

        if (arr->size == arr->capacity) {
                arr->capacity *= 2;
                ipHandler *tmp = (ipHandler *)realloc(arr->data, arr->capacity * sizeof(ipHandler));
                if (tmp == NULL) {
                        printf("[MEMORY] Not expanding dynArray. Blocking incoming requests\n");
                        return;
                }
                arr->data = tmp;
        }
        arr->data[arr->size] = entry;
        arr->size++;
}

void dynArray_free(struct dynArray *arr)
{
        free(arr->data);
        arr->data = NULL;
        arr->size = 0;
        arr->capacity = 0;
}
        /*For testing again, will improve*/
void checkCallCount(ipHandler *h)
{
        if (h->callCount >= MAX_CALLCOUNT)
                h->flags = IP_FLAG_SCANNER | IP_FLAG_BLACKLISTED; /*20*/
}

int main()
{       
        /*
        * ALL TEMPORARY!!!
        * The main function here will be replaced, it is currently just used for TESTING
        */
        const char testIpv4[] = "191.128.1.1";
        ipHandler entry1;
        IpHandler_init(&entry1);
        inet_pton(AF_INET, testIpv4, &entry1.address.ipv4);
        entry1.callCount = 12;
        checkCallCount(&entry1);

        struct dynArray arr;
        dynArray_init(&arr);
        dynArray_push(&arr, entry1);

        for (int i = 0; i < arr.size; i++) {
                printf("entry[%d] flags: %d\n", i, arr.data[i].flags);
        }

        // char str6[INET6_ADDRSTRLEN];
        // char tmp[INET6_ADDRSTRLEN];
        // inet_ntop(AF_INET6, &entryv6.address.ipv6, str6, INET6_ADDRSTRLEN);
        // inet_ntop(AF_INET, &entryv4.address.ipv4, tmp, INET_ADDRSTRLEN);

        // printf("White: %s\n", WHITELISTED_FILE_PATH);
        // printf("Ban: %s\n", BAN_FILE_PATH);

        dynArray_free(&arr);
        return 0;
}

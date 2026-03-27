// Copyright (c) goes to Jan Oliver Quant
#include "ip_utils.h"
#include "hashmap.h"
#include "checksum_handler.h"
#include <openssl/evp.h>
#include <sched.h>
#include <stddef.h>
#include <stdint.h>

int genChecksum(const hashmap_t *map, uint8_t *checksum, unsigned int *checksumLen)
{
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!ctx)
        return -1;
    if (!EVP_DigestInit_ex(ctx, EVP_sha256(), NULL)) {
        EVP_MD_CTX_free(ctx);
        return -1;
    }
    EVP_DigestUpdate(ctx, &map->size, sizeof(size_t));
    for (size_t i = 0; i < map->capacity; i++) {
        if (!map->slots[i].isOccupied) 
            continue;
        EVP_DigestUpdate(ctx, &map->slots[i], sizeof(ipEntry));
    }
    if (!EVP_DigestFinal_ex(ctx, checksum, checksumLen)) {
        EVP_MD_CTX_free(ctx);
        return -1;
    }
    EVP_MD_CTX_free(ctx);
    return 0;
}

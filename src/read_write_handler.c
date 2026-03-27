// Copyright (c) goes to Jan Oliver Quant
/*Make it more readable it sucks so hard rn ;C*/
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <openssl/evp.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "config_ip.h"
#include "ip_utils.h"
#include "read_write_handler.h"
#include "checksum_handler.h"
#include "hashmap.h"

int writeIPFile(const hashmap_t *map)
{
    uint8_t checksum[EVP_MAX_MD_SIZE];
    unsigned int checksumLen;
    if (genChecksum(map, checksum, &checksumLen) == -1)
        return -1;
    FILE *out = fopen(IP_FILE_PATH, "wb");
    if (out == NULL)
        return -1;
    fwrite(&MAGIC, sizeof(MAGIC), 1, out);
    fwrite(&map->size, sizeof(size_t), 1, out);
    for (size_t i = 0; i < map->capacity; i++) {
        if (!map->slots[i].isOccupied)
            continue;
        fwrite(&map->slots[i], sizeof(ipEntry), 1, out);
    }
    fwrite(checksum, checksumLen, 1, out);
    fclose(out);
    return 0;
}

int readIPFile(hashmap_t *map)
{
    uint8_t fileChecksum[EVP_MAX_MD_SIZE];
    uint8_t ComputedChecksum[EVP_MAX_MD_SIZE];
    unsigned int computedChecksumLen;
    size_t count;
    FILE *in = fopen(IP_FILE_PATH, "rb");
    if (in == NULL)
        return -1;
    uint64_t rMagic;
    fread(&rMagic, sizeof(MAGIC), 1, in);
    if (rMagic != MAGIC) {
        fclose(in);
        return -1;
    }
    fread(&count, sizeof(size_t), 1, in);
    for (size_t i = 0; i < count; i++) {
        ipEntry entry;
        fread(&entry, sizeof(ipEntry), 1, in);
        hashmap_insert(map, entry);
    }
    int checksumLen = EVP_MD_size(EVP_sha256());
    fread(&fileChecksum, checksumLen, 1, in);
    fclose(in);
    if (genChecksum(map, ComputedChecksum, &computedChecksumLen) == -1)
        return -1;
    if (memcmp(fileChecksum, ComputedChecksum, computedChecksumLen) != 0)
        return -1;
    return 0;
}


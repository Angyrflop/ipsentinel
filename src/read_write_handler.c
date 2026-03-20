// Copyright (c) goes to Jan Oliver Quant
/*Make it more readable it sucks so hard rn ;C*/
#include <stdint.h>
#include <stdio.h>
#include <openssl/evp.h>
#include <string.h>
#include "config_ip.h"
#include "ip_utils.h"
#include "read_write_handler.h"
#include "checksum_handler.h"

int writeFile(dynArray *arr)
{
    uint8_t checksum[EVP_MAX_MD_SIZE];
    unsigned int checksumLen;
    if (genChecksum(arr, checksum, &checksumLen) == -1)
        return -1;
    FILE *out = fopen(IP_FILE_PATH, "wb");
    if (out == NULL)
        return -1;
    fwrite(&MAGIC, sizeof(MAGIC), 1, out);
    fwrite(&arr->size, sizeof(int), 1, out);
    fwrite(arr->data, sizeof(ipEntry), arr->size, out);
    fwrite(checksum, checksumLen, 1, out);
    fclose(out);
    return 0;
}

int readFile(dynArray *arr)
{
    uint8_t fileChecksum[EVP_MAX_MD_SIZE];
    uint8_t ComputedChecksum[EVP_MAX_MD_SIZE];
    unsigned int computedChecksumLen;
    int count;
    FILE *in = fopen(IP_FILE_PATH, "rb");
    if (in == NULL)
        return -1;
    uint64_t rMagic;
    fread(&rMagic, sizeof(MAGIC), 1, in);
    if (rMagic != MAGIC) {
        fclose(in);
        return -1;
    }
    fread(&count, sizeof(int), 1, in);
    for (int i = 0; i < count; i++) {
        ipEntry entry;
        fread(&entry, sizeof(ipEntry), 1, in);
        dynArray_push(arr, entry);
    }
    int checksumLen = EVP_MD_size(EVP_sha256());
    fread(&fileChecksum, checksumLen, 1, in);
    fclose(in);
    if (genChecksum(arr, ComputedChecksum, &computedChecksumLen) == -1)
        return -1;
    if (memcmp(fileChecksum, ComputedChecksum, computedChecksumLen) != 0)
        return -1;
    return 0;
}


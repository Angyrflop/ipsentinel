// Copyright (c) goes to Jan Oliver Quant
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "url.h"

static const urlEntry url_table[] = {
    { "/", "index.html" },
    { "/style.css", "style.css" },
    { "/script.js", "script.js" },
    { "/not_found", "Not_found_404.html" },
    { "/not_found_style", "404_style.css" },
    { NULL, NULL }
};

const char *get_filename_from_url(const char *url)
{
    for (size_t i = 0; url_table[i].url != NULL; i++) {
        if (strcmp(url_table[i].url, url) == 0)
            return url_table[i].file;
    }
    return "Not_found_404.html";
}

char *read_file(const char *filepath)
{
    FILE *in = fopen(filepath, "rb");
    if (!in)
        return NULL;

    fseek(in, 0, SEEK_END);
    long size = ftell(in);
    rewind(in);

    char *buf = malloc(size + 1);
    if (!buf) {
        fclose(in);
        return NULL;
    }

    if (fread(buf, 1, size, in) != (size_t)size) {
        free(buf);
        fclose(in);
        return NULL;
    }

    buf[size] = '\0';
    fclose(in);
    return buf;
}



// Copyright (c) goes to Jan Oliver Quant
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mime.h"

static const MimeEntry mime_table[] = {
    { "html", "text/html; charset=utf-8" },
    { "css", "text/css" },
    { "js", "application/javascript" },
    { "json", "application/json" },
    { "jpeg", "image/jpeg" },
    { "ico", "image/vnd.microsoft.icon" },
    { NULL, NULL }
};

const char *mime_from_ext(const char *ext)
{
    for (size_t i = 0; mime_table[i].ext != NULL; i++) {
        if (strcmp(mime_table[i].ext, ext) == 0)
            return mime_table[i].type;
    }
    return "application/octet-stream";
}

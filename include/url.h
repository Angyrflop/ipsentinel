//Copyright (c) goes to Jan Oliver Quant
#ifndef URL_H
#define URL_H

typedef struct {
    const char *url;
    const char *file;
} urlEntry;

const char *get_filename_from_url(const char *url);
char *read_file(const char *filepath);

#endif /*URL_H*/

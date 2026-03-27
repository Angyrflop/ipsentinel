#ifndef MIME_H
#define MIME_H

typedef struct {
    char *ext;
    char *type;
} MimeEntry;

const char *mime_from_ext(const char *ext);

#endif /*MIME_H*/

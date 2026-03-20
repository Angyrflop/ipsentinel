#ifndef READ_WRITE_HANDLER_H
#define READ_WRITE_HANDLER_H
    #include "ip_utils.h"
#ifdef __cplusplus
extern "C" {
#endif
    int writeFile(dynArray *arr);
    int readFile(dynArray *arr);
#ifdef __cplusplus
}
#endif
#endif /*READ_WRITE_HANDLER_H*/

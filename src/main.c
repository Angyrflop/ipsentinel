#include <microhttpd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "hashmap.h"
#include "ip_utils.h"
#include "read_write_handler.h"
#include "server_config.h"
#include "mime.h"
#include "url.h"

typedef struct {
    hashmap_t *map;
    char *index_html;
    char *style_css;
    char *script_js;
} app;

enum MHD_Result answer_to_connection (void *cls, struct MHD_Connection *connection,
                          const char *url,
                          const char *method, const char *version,
                          const char *upload_data,
                          size_t *upload_data_size, void **req_cls)
{
    char *page;
    struct MHD_Response *response;
    enum MHD_Result ret;
    const char *mimeType = mime_from_ext("html");
    app *ctx = (app *)cls;

    const union MHD_ConnectionInfo *info = MHD_get_connection_info(connection, MHD_CONNECTION_INFO_CLIENT_ADDRESS);

    if (strcmp(method, "GET") != 0)
        return MHD_NO;
    if (strcmp(url, "/") == 0) { 
        page = ctx->index_html;
        mimeType = mime_from_ext("html");
    } else if (strcmp(url, "/style.css") == 0) {
        page = ctx->style_css;
        mimeType = mime_from_ext("css");
    } else if (strcmp(url, "/script.js") == 0) {
        page = ctx->script_js;
        mimeType = mime_from_ext("js");
    } else {
        return MHD_NO;
    }

    addIp(ctx->map, info);

    response = MHD_create_response_from_buffer (strlen (page),
                                            (void*) page, MHD_RESPMEM_PERSISTENT);
    MHD_add_response_header(response, "Content-Type", mimeType);
    ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
    MHD_destroy_response (response);

    return ret;
}

int main() {
    hashmap_t map;
    hashmap_init(&map);
    
    if ( READ_IP_ON_STARTUP == true) {
        if (readIPFile(&map) != 0) {
            printf("[ STARTUP]: Failed to read ips.bin!!!\n");
            if ( ABORT_IF_FILE_IS_EMPTY == true)
                return -1;
        }
    } else {
        printf("[ STARTUP]: Not reading ips.bin\n");
    }
    

    app ctx;
    ctx.map = &map;
    ctx.index_html = read_file(get_filename_from_url("/"));
    ctx.style_css = read_file(get_filename_from_url("/style.css"));
    ctx.script_js = read_file(get_filename_from_url("/script.js"));

    struct MHD_Daemon *daemon;

    daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, PORT, NULL, NULL,
            &answer_to_connection, &ctx, MHD_OPTION_END);
    if (daemon == NULL) {
        free(ctx.index_html);
        free(ctx.style_css);
        free(ctx.script_js);
        hashmap_free(&map);
        return -1;
    }

    getchar();

    /*Testing, only prints isIpv6 bolean since im too tired to write the whole inet_pton thingy*/
    for (size_t i = 0; i < map.capacity; i++) {
        if (!map.slots[i].isOccupied)
            continue;
        printf("[%d]\n", i);
        printf("%s\n", map.slots[i].isIpv6 ? "true" : "false");
    }

    if (WRITE_IP_DURING_SHUTDOWN == true) {
        if (writeIPFile(&map) != 0) {
            printf("[SHUTDOWN]: Failed to write ips.bin!!!\n");
            return -1;
        }
    } else {
        printf("[SHUTDOWN]: Not writting ips.bin\n");
    }

    MHD_stop_daemon(daemon);
    free(ctx.script_js);
    free(ctx.style_css);
    free(ctx.index_html);
    hashmap_free(&map);
    return 0;
}

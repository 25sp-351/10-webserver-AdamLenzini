#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct headers {
    char* key;
    char* value;
    struct headers* next;
} Headers;

typedef struct hreq {
    char* method;
    char* path;
    char* version;
    Headers* headers;
    char* body;
    int body_length;

    void (*print)(struct hreq* req);
    void (*free)(struct hreq* req);
} Request;

#define MAX_BUF_LEN 1024

Request* request_read_from_fd(int fd);

Request* request_create();

bool read_request_line(int fd, Request* req);

char* read_http_line(int fd);

Headers* read_headers(int fd);

void request_print(Request* req);

void request_free(Request* req);
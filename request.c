#include "request.h"

Request* request_read_from_fd(int fd) {
    printf("Reading request from fd %d\n", fd);

    Request* req = request_create();

    if (read_request_line(fd, req) == false) {
        printf("Failed to read request line\n");
        req->free(req);
        return NULL;
    }

    req->headers = read_headers(fd);

    if (req->headers == NULL) {
        printf("Failed to read headers\n");
        req->free(req);
        return NULL;
    }

    if (read_body(req, fd) == false) {
        printf("Failed to read body\n");
        req->free(req);
        return NULL;
    }

    return req;
}

Request* request_create() {
    Request* req = malloc(sizeof(Request));
    req->method = NULL;
    req->path = NULL;
    req->version = NULL;
    req->headers = NULL;
    req->body = NULL;
    req->body_length = 0;

    req->print = request_print;
    req->free = request_free;

    return req;
}

bool read_request_line(int fd, Request* req) {
    char* line;

    while (1) {
        line = read_http_line(fd);
        if (line == NULL) {
            return false;
        }
        if (strlen(line) == 0) 
            break;
        free(line);
    }

    req->method = malloc(strlen(line) + 1);
    req->path = malloc(strlen(line) + 1);
    req->version = malloc(strlen(line) + 1);
    int length_parsed;
    int number_parsed;

    number_parsed = sscanf(line, "%s %s %s%n", req->method, req->path,
        req->version, &length_parsed);

    if (number_parsed != 3 || length_parsed != strlen(line)) {
        printf("Invalid request line: %s\n", line);
        free(line);
        return false;
    }

    if (strcmp(req->method, "GET") != 0 && strcmp(req->method, "POST") != 0) {
        printf("Invalid method: %s\n", req->method);
        free(line);
        return false;
    }

    return true;
}

char* read_http_line(int fd) {
    char* line = malloc(MAX_BUF_LEN);
    ssize_t bytes_read = read(fd, line, MAX_BUF_LEN - 1);

    if (bytes_read <= 0) {
        free(line);
        return NULL;
    }

    line[bytes_read] = '\0'; // Null-terminate the string

    return line;
}

bool read_body(Request* req, int fd) {
    req->body
    req->body_length = 0;

    char* body_length = get_header(req->headers, "Content-Length");

    if (body_length == NULL)
        return true;

    if (sscanf(body_length, "%d", &req->body_length) != 1) {
        printf("Failed to read content length: %s\n");
        return false;
    }

    if (req->body_length == 0)
        return true;

    req->body = malloc(req->body_length + 1);
    int bytes_read = read(fd, req->body, req->body_length);
    if (bytes_read != req->body_length) {
        printf("Failed to read body\n");
        free(req->body);
        req->body = NULL;
        return false;
    }
    return true;
}
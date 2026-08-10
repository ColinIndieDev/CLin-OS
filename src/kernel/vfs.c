#include "vfs.h"

#include "../utils/stdlib.h"
#include "../utils/string.h"

vfs_node_t *nodes = NULL;
size_t nodes_count = 0;
size_t file = 0;

vfs_node_t *get_file_array() {
    return nodes;
}

size_t get_file_array_size() {
    return nodes_count;
}

void vfs_init(size_t files) {
    nodes_count = files;
    nodes = malloc(sizeof(vfs_node_t) * files);
}

uint32_t file_read(vfs_node_t *node, uint32_t off, uint32_t size, uint8_t* buf) {
    if (!node || !node->buffer || off >= node->length) {
        return 0;
    }

    if (off + size > node->length) {
        size = node->length - off;
    }

    for (uint32_t i = 0; i < size; i++) {
        buf[i] = node->buffer[off + i];
    }

    return size;
}

uint32_t file_write(vfs_node_t *node, uint32_t off, uint32_t size, const uint8_t* buf) {
    if (!node || !node->buffer) {
        return 0;
    }

    if (off + size > node->capacity) {
        size = node->capacity - off;
    }

    for (uint32_t i = 0; i < size; i++) {
        node->buffer[off + i] = buf[i];
    }

    if (off + size > node->length) {
        node->length = off + size;
    }

    return size;
}

vfs_node_t* create_file(const char* name, uint32_t capacity) {
    vfs_node_t* node = &nodes[file++];
    if (!node) {
        return NULL;
    }

    node->buffer = (uint8_t*)malloc(capacity);
    if (!node->buffer) {
        free(node);
        return NULL;
    }

    strcpy(node->name, name);
    node->flags = VFS_FILE;
    node->length = 0;
    node->capacity = capacity;
    node->read = file_read;
    node->write = file_write;

    return node;
}

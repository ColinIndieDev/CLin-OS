#pragma once

#include <stdint.h>
#include <stddef.h>

struct vfs_node;

typedef uint32_t (*read_type_t)(struct vfs_node *node, uint32_t off, uint32_t size, uint8_t* buf);
typedef uint32_t (*write_type_t)(struct vfs_node *node, uint32_t off, uint32_t size, const uint8_t* buf);

enum {
    VFS_FILE = 0x1
};

typedef struct vfs_node {
    char name[128];
    uint32_t flags;
    uint32_t length;
    uint32_t capacity;
    uint8_t *buffer;

    read_type_t read;
    write_type_t write;
} vfs_node_t;

vfs_node_t *get_file_array();
size_t get_file_array_size();
void vfs_init(size_t files);
uint32_t file_read(vfs_node_t *node, uint32_t off, uint32_t size, uint8_t* buf);
uint32_t file_write(vfs_node_t *node, uint32_t off, uint32_t size, const uint8_t* buf);
vfs_node_t *create_file(const char *name, uint32_t capacity);

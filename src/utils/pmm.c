#include "pmm.h"

#define PAGE_SIZE 4096

static uint32_t *pmm_bitmap = 0;
static size_t pmm_max_blocks = 0;

static inline void bitmap_set(size_t bit) {
    pmm_bitmap[bit / 32] |= (1 << (bit % 32));
}
static inline void bitmap_clear(size_t bit) {
    pmm_bitmap[bit / 32] &= ~(1 << (bit % 32));
}
static inline int bitmap_test(size_t bit) {
    return (int)(pmm_bitmap[bit / 32] & (1 << (bit % 32)));
}

void pmm_init(uint64_t mem_size, uintptr_t kernel_end) {
    pmm_max_blocks = mem_size / PAGE_SIZE;
    pmm_bitmap = (uint32_t *)kernel_end;
    size_t bitmap_size = pmm_max_blocks / 8;
    for (size_t i = 0; i < bitmap_size / 4; i++) {
        pmm_bitmap[i] = 0xFFFFFFFF;
    }

    uintptr_t free_mem_start = kernel_end + bitmap_size;
    size_t start_block = free_mem_start / PAGE_SIZE;
    for (size_t i = start_block; i < pmm_max_blocks; i++) {
        bitmap_clear(i);
    }
}

void *pmm_alloc_pages(size_t count) {
    if (count == 0) {
        return NULL;
    }

    size_t free_needed = count;
    size_t start_block = 0;

    for (size_t i = 0; i < pmm_max_blocks; i++) {
        if (!bitmap_test(i)) {
            if (free_needed == count) {
                start_block = i;
            }
            free_needed--;

            if (free_needed == 0) {
                for (size_t j = start_block; j < start_block + count; j++) {
                    bitmap_set(j);
                }
                return (void *)((uint64_t)start_block * PAGE_SIZE);
            }
        } else {
            free_needed = count;
        }
    }

    return NULL;
}

void pmm_free_pages(void *ptr, size_t count) {
    uintptr_t addr = (uintptr_t)ptr;
    size_t start_block = addr / PAGE_SIZE;

    for (size_t i = start_block; i < start_block + count; i++) {
        bitmap_clear(i);
    }
}

int liballoc_lock() {
    return 0;
}
int liballoc_unlock() {
    return 0;
}

void *liballoc_alloc(int pages) {
    return pmm_alloc_pages(pages);
}

int liballoc_free(void *ptr, int pages) {
    pmm_free_pages(ptr, pages);
    return 0;
}

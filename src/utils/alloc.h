#pragma once

#include <stdint.h>

#include "liballoc.h"

void pmm_init(uint64_t mem_size, uintptr_t kernel_end);

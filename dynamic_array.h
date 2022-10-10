#ifndef AM_DYN_ARRAY
#define AM_DYN_ARRAY
#include <stdlib.h>
#include <limits.h>
#include <memory.h>
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#define DYN_ARRAY_EMPTY_START_SLOTS 2

typedef struct dyn_array_header {
    size_t size; // In bytes
    size_t capacity; // In bytes
} dyn_array_header;

#define dyn_array(type) type*
#define dyn_array_get_header(array) ((dyn_array_header*)((size_t*)(array) - 2))
#define dyn_array_get_size(array) dyn_array_get_header(array)->size
#define dyn_array_get_count(array) dyn_array_get_size(array)/sizeof((array)[0])
#define dyn_array_get_capacity(array) dyn_array_get_header(array)->capacity
#define dyn_array_push(array, value) (dyn_array_resize((void**)&array, sizeof((array)[0])), (array)[dyn_array_get_count(array)] = (value), dyn_array_get_header(array)->size += sizeof((array)[0]))
#define dyn_array_pop(array) dyn_array_get_header(array)->size -= sizeof((array)[0])
#define am_dyn_array_clear(array) (am_dyn_array_get_header(array)->size = 0)


//void **array -> pointer to array
void dyn_array_init(void **array, size_t value_size) {
    if (*array == NULL) {
        dyn_array_header *data = (dyn_array_header*)malloc(value_size + sizeof(dyn_array_header));
        data->capacity = value_size;
        data->size = 0;
        *array = ((size_t*)data + 2);
    };
};

void dyn_array_resize(void **array, size_t add_size) {
    dyn_array_header *header;
    size_t new_capacity = 0, alloc_size = 0;
    if (dyn_array_get_capacity(*array) < dyn_array_get_size(*array) + add_size) {
        new_capacity = 2 * dyn_array_get_capacity(*array);
        alloc_size = new_capacity + sizeof(dyn_array_header);
        header = (dyn_array_header*)realloc(dyn_array_get_header(*array), alloc_size);
        if (!header) {
            printf("[FAIL] Failed to allocate memory for array!\n");
            return;
        };
        header->capacity = new_capacity;
        *array = (size_t*)header + 2;
    };
};

void dyn_array_replace(void *array, void *values, size_t offset, size_t size) {
    assert(offset + size <= dyn_array_get_size(array));
    memcpy(array + offset, values, size);
};

void dyn_array_clear(void *array) {
    dyn_array_header *header = dyn_array_get_header(array);
    header->capacity = 0;
    header->size = 0;
    free((void*)header);
};

#endif

#ifndef DYN_ARRAY_H
#define DYN_ARRAY_H

//
// Dynamic array 
// 
// These functions support a dynamic array type I originally encountered from
// Per Vognsen so all credit goes to him for the idea (or whoever he got it from).
//
// Supports custom allocators through the use of defining macros for da_alloc, da_realloc,
// and da_free.  If these aren't defined then it will use the stdlib versions
//
// Usage:
// Declare a pointer to the type you want to put in the array and set it to NULL.
//     T* list = NULL;
// 
// Use the rest of the functions as you would expect.  Call da_delete() to free the memory.
//
// Structure:
//     int len <- start of allocation
//     int cap
//     T[cap]  <- return of da_init points here

#if !defined(da_alloc) || !defined(da_realloc) || !defined(da_free)
    #include <stdlib.h>
    #define da_alloc(size) malloc(size)
    #define da_realloc(ptr, size) realloc(ptr, size)
    #define da_free(ptr) free(ptr)
#endif

// returns da_len basically
#define _da_hdr(__da_list) \
    (((int*)(__da_list))[-2])


void* _da_resize(void* ptr, size_t elem_size, size_t new_len);


#define _da_init(__da_list, __size) \
    _da_resize(__da_list, sizeof(*__da_list), __size)


#define da_len(__da_list) \
    ((__da_list) ? (_da_hdr(__da_list)) : 0)


#define da_cap(__da_list) \
    ((__da_list) ? (*(&_da_hdr(__da_list)+1)) : 0)


#define da_append(__da_list, __item) \
    da_insert(__da_list, __item, da_len(__da_list))    


#define da_push(__da_list, __item) \
    da_append(__da_list, __item)


#define da_insert(__da_list, __item, __index) \
    if((__da_list)==NULL) (__da_list) = _da_init(__da_list, 16);     \
    else if (da_cap((__da_list)) == da_len((__da_list))) (__da_list) = _da_init((__da_list), da_cap((__da_list)) * 2);    \
    memcpy((__da_list) + (__index) + 1, (__da_list) + (__index), sizeof(*(__da_list)) * (da_len((__da_list)) - (__index)));  \
    (__da_list)[__index] = (__item); \
    _da_hdr((__da_list))++ // incrememnt len


#define da_pop(__da_list) \
    (__da_list)[(_da_hdr((__da_list))--) - 1]


// NOTE(Scott): this is an ordered, slower remove
#define da_remove(__da_list, __index) \
    assert(da_len((__da_list)) > __index + 1); \
    memcpy((__da_list) + (__index), (__da_list) + (__index) + 1, sizeof(*(__da_list)) * (da_len((__da_list)) - (__index)));  \
    (_da_hdr((__da_list))--);


// NOTE(Scott): this is an unordered, faster move
#define da_remove_unordered(__da_list, __index) \
    assert(da_len((__da_list)) > __index + 1);     \
    (__da_list)[(__index)] = (__da_list)[da_len((__da_list)) - 1], _da_hdr((__da_list))--


#define da_clear(__da_list) \
    if((__da_list)) _da_hdr((__da_list)) = 0


#define da_delete(__da_list) \
    if((__da_list)) da_free(&_da_hdr((__da_list)))


//
// Implementation
//
#ifdef DYN_ARRAY_IMPL

inline void* _da_resize(void* ptr, size_t elem_size, size_t new_len) {    
    if (ptr) {
        int* block = (int*)da_realloc(&_da_hdr(ptr), sizeof(int) * 2 + elem_size * new_len);
        *(block+1) = new_len;
        return (void*)(block + 2);
    }
    int* block = (int*)da_alloc( sizeof(int) * 2 + elem_size * new_len);
    *(block) = 0;
    *(block+1) = new_len;
    return (void*)(block + 2);
}

#endif  // DYN_ARRAY_IMPL

#endif  // DYN_ARRAY_H
#define DYN_ARRAY_IMPL
#include "dyn_array.h"

#include <stdio.h>
#include <assert.h>


struct MyType {
    int a;
    char c;
};

void print_list(int* list) {
    int len = da_len(list);
    for (int i=0; i<len; i++)
        printf("%d\n", list[i]);
}

void main(int argc, char** argv) {

    struct MyType* list = NULL;

    int len = da_len(list);
    assert(len == 0);
    int cap = da_cap(list);
    assert(cap == 0);

    struct MyType item1 = {.a = 42, .c = 'a'};
    da_append(list, item1);
    assert(da_len(list) == 1);
    assert(da_cap(list) > 0);

    struct MyType copy = list[0];
    assert(copy.a == 42);
    assert(copy.c == 'a');

    da_clear(list);
    assert(da_len(list) == 0);

    da_delete(list);

    int* int_list = NULL;
    for (int i=0; i<4; i++) {
        da_append(int_list, i);
    }
    assert(da_len(int_list) == 4);
    assert(da_cap(int_list) == 16);

    assert(int_list[2] == 2);
    da_insert(int_list, 42, 2);
    assert(int_list[2] == 42);
    assert(int_list[3] == 2);
    assert(da_len(int_list) == 5);
    assert(da_cap(int_list) == 16);

    int last = da_pop(int_list);
    assert(last == 3);
    assert(da_len(int_list) == 4);

    da_remove(int_list, 2);
    print_list(int_list);
    printf("len=%d\n", da_len(int_list));
    assert(da_len(int_list) == 3);
    assert(int_list[2] == 2);

    da_remove_unordered(int_list, 0);
    assert(da_len(int_list) == 2);
    assert(int_list[0] == 2);
    assert(int_list[1] == 1);

    printf("Passed\n");
}
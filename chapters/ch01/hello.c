#include "stdio.h"
#include "unistd.h"
#include "string.h"

/*
    Printf Table

    %d     - int
    %f     - double
    %c     - char
    %s     - String (Char Array)
    %x     - Unsigned integer in hex
    %p     - Pointer (Address)
*/

size_t add(size_t x, size_t y) {
    return x + y;
}

int main(int argc, char* argv[]) {
    // C Library function that handles the low level sys call for us
    printf("Hello World!!\n");

    // Direct Linux SysCall
    write(1, "Hello World!!\n", 14);

    const size_t ARR_SIZE = 10;
    int arr[10] = { 0 };

    for(int i = 0; i < ARR_SIZE; i++) {
        arr[i] = ARR_SIZE - i;
        printf("Index %d: %d\n", i + 1, arr[i]);
    }
}
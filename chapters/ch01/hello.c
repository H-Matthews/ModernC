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
    fprintf(stdout, "%d + %d = %ld\n", 5, 10, add(5, 10));

    char string[] = "HELLO WORLD";
    int wordSize = strlen(string);

    printf("String Bytes: %ld\n", sizeof(string));
    for(size_t i = 0; i < sizeof(string); i++) {
        if(string[i] != '\0') {
            write(1, &string[i], 1);
        }
    }
    write(1, "\n", 1);

    // Print the character to stdout one char at a time
    // fputc is buffered so this doesn't invoke a syscall per function call
    // size_t i = 0;
    // for(i = 0; i < wordSize; i++) {
    //     // write(1, string + i, 1);
    //     fputc(string[i], stdout);
    // }

    // if(string[i] == '\0') {
    //     fprintf(stdout, "\\0\n");
    // }
}
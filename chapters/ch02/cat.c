#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>

enum { MAX_BUFFER_SIZE = 4096 };

/**
 * @brief Streams the files contents to stdout
 * 
 * This function takes a file descriptor, reads it into a intermediary byte buffer,
 * then streams the bytes to stdout. Uses read, write POSIX calls
 * 
 * @param[in] fptr The file to be streamed
 */
void streamBytes(int fd);

/**
 * @brief Logs a error when a file descriptor cannot be obtained
 */
void logFileError(const char* fileName, const char* progName);

int main(int argc, char* argv[]) {
    const char* progName = argv[0];

    if(argc > 1) {
        for(int i = 1; i < argc; i++) {
            const char* fileName = argv[i];

            int fd = open(fileName, O_RDONLY);
            if(fd == -1) {
                logFileError(fileName, progName);
                continue;
            }

            streamBytes(fd);
            close(fd);
        }
    }
    else {
        streamBytes(STDIN_FILENO);
    }

    return 0;
}

void streamBytes(int fd) {
    char buffer[MAX_BUFFER_SIZE];
    ssize_t bytesRead = 0;

    while((bytesRead = read(fd, buffer, MAX_BUFFER_SIZE)) > 0) {
        ssize_t totalWritten = 0;

        // Ensure all of the bytes we read are written
        while(totalWritten < bytesRead) {
            ssize_t written =
                write(STDOUT_FILENO, buffer + totalWritten, bytesRead - totalWritten);
            if(written < 0) {
                perror("Write Error");
                break;
            }

            totalWritten += written;
        }
    }

    if(bytesRead < 0) {
        perror("Read Error");
    }
}

void logFileError(const char* fileName, const char* progName) {
    char errorMsg[200];
    snprintf(errorMsg, sizeof(errorMsg), "%s: %s", progName, fileName);

    perror(errorMsg);
}
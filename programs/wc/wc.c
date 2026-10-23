#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <unistd.h>
#include <fcntl.h>

enum { MAX_BUFFER_SIZE = 4096 };

#define SPACE 0x20
#define NEWLINE 0x0A
#define HORIZONTAL_TAB 0x09
#define VERTICAL_TAB 0x0B
#define FORM_FEED 0x0C
#define CARRIAGE_RETURN 0x0D

int32_t byteCount = 0;
int32_t lineCount = 0;
int32_t wordCount = 0;
int32_t charCount = 0;

int32_t tByteCount = 0;
int32_t tLineCount = 0;
int32_t tWordCount = 0;
int32_t tCharCount = 0;

/**
 * @brief Streams bytes from a file descriptor to stdout
 * 
 * This function takes a file descriptor, reads it into a intermediary byte buffer,
 * then processes the bytes one at a time
 * 
 * @param[in] fd - File Descriptor to stream bytes from
 */
void processBytes(int fd);

/**
 * @brief Logs a error when a file descriptor cannot be obtained
 * 
 * @param[in] fileName - Name of the file that errored
 * @param[in] progName - Name of the program
 */
void logFileError(const char* fileName, const char* progName);

/**
 * @brief Prints count information to stdout
 * 
 * @param[in] fileName - Name of the file that we are printing statistics for
 */
void printStatistics(const char* fileName);

/**
 * @brief Prints total count information to stdout
 */
void printTotals();

/**
 * @brief Resets counters back to zero
 */
void resetCounters();

/**
 * @brief Determines whether or not the byte is an ASCII character
 * 
 * ASCII characters always have their MSB set to 0, therefore, we check if the MSB is set
 */
bool isAsciiChar(uint8_t byte) {
    return (byte & 0x80) == 0;
}

/**
 * @brief Determine whether or not the byte is a white space character
 */
bool isWhiteSpace(uint8_t byte) {
    switch(byte) {
        case SPACE:
        case NEWLINE:
        case HORIZONTAL_TAB:
        case VERTICAL_TAB:
        case FORM_FEED:
        case CARRIAGE_RETURN: {
            return true;
        }
        default: return false;
    }
}

int main(int argc, char* argv[]) {
    const char* progName = argv[0];

    if(argc > 1) {
        int16_t filesProcessed = 0;

        for(int i = 1; i < argc; i++) {
            const char* fileName = argv[i];

            const int fd = open(fileName, O_RDONLY);
            if(fd == -1) {
                logFileError(fileName, progName);
                continue;
            }

            processBytes(fd);
            printStatistics(fileName);

            // Each file needs its own totals...
            // So, reset after each file is completed. The totals are maintained.
            resetCounters();

            filesProcessed++;

            close(fd);
        }

        if(filesProcessed > 1) {
            printTotals();
        }

    }
    else {
        processBytes(STDIN_FILENO);
        printStatistics(NULL);
    }
}

void processBytes(int fd) {
    uint8_t buffer[MAX_BUFFER_SIZE];
    ssize_t bytesRead = 0;
    bool inWord = false;

    while((bytesRead = read(fd, buffer, MAX_BUFFER_SIZE)) > 0) {
        byteCount += bytesRead;

        for(int i = 0; i < bytesRead; i++) {
            uint8_t byte = buffer[i];

            // Determine line count
            if(byte == NEWLINE) {
                lineCount++;
            }

            // Determine ASCII char count
            if(isAsciiChar(byte)) {
                charCount++;
            }

            // Determine WORD count
            if(isWhiteSpace(byte)) {
                inWord = false;
            }
            else if(inWord == false) {
                inWord = true;
                wordCount++;
            }
        }
    }

    // Maintain totals
    tByteCount += byteCount;
    tLineCount += lineCount;
    tWordCount += wordCount;
    tCharCount += charCount;

    if(bytesRead < 0) {
        perror("Read Error");
    }
}

void logFileError(const char* fileName, const char* progName) {
    char errorMsg[200];
    snprintf(errorMsg, sizeof(errorMsg), "%s: %s", progName, fileName);

    perror(errorMsg);
}

void printStatistics(const char* fileName) {
    if(fileName != NULL) {
        printf("  %d  %d  %d  %s\n", lineCount, wordCount, byteCount, fileName);
    }
    else {
        printf("  %d  %d  %d\n", lineCount, wordCount, byteCount);
    }
}

void printTotals() {
    printf("  %d  %d  %d  Totals\n", tLineCount, tWordCount, tByteCount);
}

void resetCounters() {
    byteCount = 0;
    lineCount = 0;
    wordCount = 0;
    charCount = 0;
}
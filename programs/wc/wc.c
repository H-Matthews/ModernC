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

typedef struct FileStats{
    uint32_t byteCount;
    uint32_t lineCount;
    uint32_t wordCount;
    uint32_t charCount;
} FileStats;

/**
 * @brief Streams bytes from a file descriptor to stdout
 * 
 * This function takes a file descriptor, reads it into a intermediary byte buffer,
 * then processes the bytes one at a time
 * 
 * @param[in] fd - File Descriptor to stream bytes from
 * @param[in] fileStats - Structure that maintains file statistics
 */
void processBytes(int fd, FileStats* fileStats);

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
void printStatistics(const char* fileName, const FileStats* fileStats);

/**
 * @brief Prints total count information to stdout
 */
void printTotals(const FileStats* totalStats);

/**
 * @brief Resets counters back to zero
 */
void resetCounters(FileStats* fileStats);

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

    FileStats stats = {0, 0, 0, 0};
    FileStats totalStats = {0, 0, 0, 0};

    if(argc > 1) {
        uint16_t filesProcessed = 0;

        for(int i = 1; i < argc; i++) {
            const char* fileName = argv[i];

            const int fd = open(fileName, O_RDONLY);
            if(fd == -1) {
                logFileError(fileName, progName);
                continue;
            }

            processBytes(fd, &stats);

            // Update file totals
            totalStats.byteCount += stats.byteCount;
            totalStats.charCount += stats.charCount;
            totalStats.wordCount += stats.wordCount;
            totalStats.lineCount += stats.lineCount;

            printStatistics(fileName, &stats);

            // Each file needs its own totals...
            // So, reset stats after each file is read. The totals are maintained.
            resetCounters(&stats);

            filesProcessed++;

            close(fd);
        }

        if(filesProcessed > 1) {
            printTotals(&totalStats);
        }

    }
    else {
        processBytes(STDIN_FILENO, &stats);
        printStatistics(NULL, &stats);
    }
}

void processBytes(int fd, FileStats* fileStats) {
    uint8_t buffer[MAX_BUFFER_SIZE];
    ssize_t bytesRead = 0;
    bool inWord = false;

    while((bytesRead = read(fd, buffer, MAX_BUFFER_SIZE)) > 0) {
        fileStats->byteCount += bytesRead;

        for(int i = 0; i < bytesRead; i++) {
            uint8_t byte = buffer[i];

            // Determine line count
            if(byte == NEWLINE) {
                fileStats->lineCount++;
            }

            // Determine ASCII char count
            if(isAsciiChar(byte)) {
                fileStats->charCount++;
            }

            // Determine WORD count
            if(isWhiteSpace(byte)) {
                inWord = false;
            }
            else if(inWord == false) {
                inWord = true;
                fileStats->wordCount++;
            }
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

void printStatistics(const char* fileName, const FileStats* fileStats) {
    if(fileName != NULL) {
        printf(" %d %d %d %s\n",
            fileStats->lineCount, fileStats->wordCount, fileStats->byteCount, fileName);
    }
    else {
        printf(" %d %d %d\n",
            fileStats->lineCount, fileStats->wordCount, fileStats->byteCount);
    }
}

void printTotals(const FileStats* totalStats) {
    printf(" %d %d %d Totals\n",
        totalStats->lineCount, totalStats->wordCount, totalStats->byteCount);
}

void resetCounters(FileStats* fileStats) {
    fileStats->byteCount = 0;
    fileStats->lineCount = 0;
    fileStats->wordCount = 0;
    fileStats->charCount = 0;
}
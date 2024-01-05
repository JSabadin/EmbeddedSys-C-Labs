#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <malloc.h>

#define TOTAL_PIXELS 640 * 480 * 3

int main(int argc, char *argv[]) {

    // Create a named pipe (FIFO)
    mkfifo("fifo1", 0644);

    char *buffer;
    int inputFile, outputFile;
    int bufferSize;
    bufferSize = TOTAL_PIXELS / 480;
    buffer = (char *) malloc(sizeof(char) * bufferSize);

    // Open the input and output files
    inputFile = open("/dev/video0", O_RDONLY);
    outputFile = open("fifo1", O_WRONLY);

    // Continuously read from the input file and write to the output file
    while (1) {
        read(inputFile, buffer, bufferSize);
        write(outputFile, buffer, bufferSize);
    }

    // Close the input and output files
    close(inputFile);
    close(outputFile);

    exit(0);
}




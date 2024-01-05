#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <malloc.h>
#include <unistd.h>


int main(int argc, char *argv[]) {

    int inputFile, outputFile, screenWidth = 1680, screenHeight = 1050;
    int bufferSize = 640 * 2;
    int screenOffset = screenWidth * 2 - bufferSize;
    char *inputBuffer, *outputBuffer, *bufferB;
    inputBuffer = (char *) malloc(sizeof(char) * bufferSize);

    inputFile = open("fifo2", O_RDONLY);
    outputFile = open("/dev/fb0", O_WRONLY);

    // Read from the input file and write to the output file
    while (1) {
        read(inputFile, inputBuffer, bufferSize);
        write(outputFile, inputBuffer, bufferSize);
        lseek(outputFile, screenOffset, SEEK_CUR); // Instead, it would appear stretched and distorted.

        // Reset the file position to the beginning when it reaches the end
        if ((lseek(outputFile, 0, SEEK_CUR)) >= (screenWidth * 2 * 480)) {
            lseek(outputFile, 0, SEEK_SET);
        }
    }

    // Close the input and output files
    close(inputFile);
    close(outputFile);
    return 0;
}

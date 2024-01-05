#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <malloc.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    // Create a named pipe (FIFO)
    mkfifo("fifo2", 0644);

    int inputFile, outputFile, index, screenWidth = 1680, screenHeight = 1050;
    int inputBufferSize = 640 * 3;
    int outputBufferSize = 640 * 2;
    char *inputBuffer, *outputBuffer, red, green, blue;

    inputBuffer = (char *) malloc(sizeof(char) * inputBufferSize);
    outputBuffer = (char *) malloc(sizeof(char) * outputBufferSize);

    inputFile = open("fifo1", O_RDONLY);
    outputFile = open("fifo2", O_WRONLY);

    // Continuously read from the input file, convert RGB to 16-bit color, and write to the output file
    while (1) {
        read(inputFile, inputBuffer, inputBufferSize);

        for (index = 0; index <= 640; index++) {
            red = inputBuffer[index * 3];
            green = inputBuffer[index * 3 + 1];
            blue = inputBuffer[index * 3 + 2];

            outputBuffer[index * 2 + 1] = (red & 0xF8) | ((green & 0xE0) >> 5);
            outputBuffer[index * 2] = ((green & 0x1C) << 3) | ((blue & 0xF8) >> 3);
        }

        write(outputFile, outputBuffer, outputBufferSize);
    }

    // Close the input and output files
     close(inputFile);
     close(outputFile);

    return 0;
}

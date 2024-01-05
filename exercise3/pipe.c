#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <malloc.h>

#define INPUT_SIZE 480 * 640 * 3

pid_t pid1, pid2;
int pipe1[2], pipe2[2];
int pipeInitStatus1, pipeInitStatus2;
int copyData();
int convertData();
int displayData();

int main(int argc, char *argv[]) {
    pipeInitStatus1 = pipe(pipe1);
    pipeInitStatus2 = pipe(pipe2);
    pid1 = fork();

    if (pid1 == 0) {
        copyData();
    } else {
        pid2 = fork();
        if (pid2 == 0) {
            convertData();
        } else {
            displayData();
        }
    }
    exit(0);
}

int copyData() {
    char *tempBuffer;
    int inputFile;
    int chunkSize;
    chunkSize = INPUT_SIZE / 480;
    tempBuffer = (char *) malloc(sizeof(char) * chunkSize);
    inputFile = open("/dev/video0", O_RDONLY);

    while (1) {
        read(inputFile, tempBuffer, chunkSize);
        write(pipe1[1], tempBuffer, chunkSize);
    }
}

int convertData() {
    int inputPipe, outputPipe, pixelIndex, screenWidth = 1680, screenHeight = 1050;
    int inputSize = 640 * 3;
    int outputSize = 640 * 2;
    char *inputBuffer, *outputBuffer, red, green, blue;
    inputBuffer = (char *) malloc(sizeof(char) * inputSize);
    outputBuffer = (char *) malloc(sizeof(char) * outputSize);

    while (1) {
        read(pipe1[0], inputBuffer, inputSize);
        for (pixelIndex = 0; pixelIndex <= 640; pixelIndex++) {
            red = inputBuffer[pixelIndex * 3];
            green = inputBuffer[pixelIndex * 3 + 1];
            blue = inputBuffer[pixelIndex * 3 + 2];

            outputBuffer[pixelIndex * 2 + 1] = (red & 0xF8) | ((green & 0xE0) >> 5);
            outputBuffer[pixelIndex * 2] = ((green & 0x1C) << 3) | ((blue & 0xF8) >> 3);
        }
        write(pipe2[1], outputBuffer, outputSize);
    }
}

int displayData() {
    int outputFile, screenWidth = 1680, screenHeight = 1050;
    int inputSize = 640 * 2;
    int screenOffset = screenWidth * 2 - 1280;
    char *buffer;
    buffer = (char *) malloc(sizeof(char) * inputSize);
    outputFile = open("/dev/fb0", O_WRONLY);

    while (1) {
        read(pipe2[0], buffer, inputSize);
        write(outputFile, buffer, inputSize);
        lseek(outputFile, screenOffset, SEEK_CUR);

        if ((lseek(outputFile, 0, SEEK_CUR)) >= (screenWidth * 2 * 480)) {
            lseek(outputFile, 0, SEEK_SET);
        }
    }
}

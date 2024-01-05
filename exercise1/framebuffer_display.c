#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <malloc.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    int input_fd, output_fd;
    int BUF_SIZE = 1680 * 1050 * 2;  // Set the buffer size based on screen resolution and color depth (16 bits per pixel)
    char *buffer;

    while (1) {

        // Allocate memory for the buffer
        buffer = (char *) malloc(sizeof(char) * BUF_SIZE);

        // Open the file containing the frame data and read it into the buffer
        input_fd = open("izhod.raw", O_RDONLY);
        read(input_fd, buffer, sizeof(char) * BUF_SIZE);

        // Open the framebuffer device for writing
        output_fd = open("/dev/fb0", O_WRONLY);

        // Write the buffer to the framebuffer device
        write(output_fd, buffer, BUF_SIZE);

        // Close the file descriptors
        if ((close(input_fd) == -1) || (close(output_fd) == -1)) {
            printf("%s: error closing files\n", argv[0]);
            exit(7);
        }

        // Sleep for 0.5 seconds before the next iteration
        sleep(0.5);
    }
    return 0;

}





















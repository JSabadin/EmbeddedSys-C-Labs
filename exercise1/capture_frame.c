#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <malloc.h>

#define FRAME_SIZE 921600 // Define the size of each frame to be read

int main(int argc, char *argv[]) {
    char *buffer; // Declare a pointer for the buffer that will hold the data read from /dev/video0
    int input_fd, output_fd; // Declare integer variables to hold the file descriptors for the input and output files
    ssize_t frame_size, bytes_read; // Declare variables to hold the size of each frame read and the number of bytes read
    frame_size = FRAME_SIZE; // Initialize the frame size to be read to the value of FRAME_SIZE constant
    input_fd = open("/dev/video0", O_RDONLY); // Open /dev/video0 for reading
    output_fd = creat("vhod.raw", 0644); // Create a new file called "vhod.raw" with write permissions for the owner

    buffer = malloc(frame_size); // Allocate memory for the buffer

    while (1) { // Loop infinitely
        while ((bytes_read = read(input_fd, buffer, frame_size)) != 0) { // Read data from /dev/video0 into the buffer
            lseek(output_fd, 0, SEEK_SET); // Set the file position of the output file to the beginning
            write(output_fd, buffer, bytes_read); // Write the data from the buffer to the output file
        }
        sleep(0.5); // Sleep for 0.5 seconds before reading the next frame
    }

    close(input_fd); // Close the input file (never happens)
    close(output_fd); // Close the output file

    exit(0); // Exit the program with status 0 
}


// scp capture_frame.c pi@192.168.91.55:~
// 0644 is equivalent to rw-r--r--, which means the owner can read and write to the file, while the group and others can only read it.
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <malloc.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    int input_fd, output_fd, display_width = 1680, display_height = 1050, idx;
    int input_buf_size = 640 * 480 * 3;
    int output_buf_size = display_width * display_height * 2;
    char *input_buf, *output_buf, *intermediate_buf, r, g, b;

    // Allocate memory for input and output buffers
    input_buf = (char *) malloc(sizeof(char) * input_buf_size);
    output_buf = (char *) malloc(sizeof(char) * output_buf_size);
    intermediate_buf = (char *) malloc(sizeof(char) * output_buf_size);

    // Open input and output files
    input_fd = open("vhod.raw", O_RDONLY);
    output_fd = creat("izhod.raw", 0644);

    while (1) {
        read(input_fd, input_buf, input_buf_size);
        // Convert RGB24 to RGB565
        lseek(input_fd, 0, SEEK_SET);
        for (int i = 0; i <= 480 * 640; i++) {
            r = input_buf[i * 3];
            g = input_buf[i * 3 + 1];
            b = input_buf[i * 3 + 2];

            intermediate_buf[i * 2 + 1] = (r & 0xF8) | ((g & 0xE0) >> 5);
            intermediate_buf[i * 2] = ((g & 0x1C) << 3) | ((b & 0xF8) >> 3);
        }

        // Set the pointer to beggining of file.
        lseek(output_fd, 0, SEEK_SET);
        idx = 0;
        for (int i = 0; i < 480; i++) {
            for (int j = (i * display_width * 2); j < (1280 + (i * display_width * 2)); j++) {
                output_buf[j] = intermediate_buf[idx];
                idx++;
            }
        }

        // Write the image to the output file
        write(output_fd, output_buf, output_buf_size);

        sleep(0.5);
    }

    // Clean up
    free(input_buf);
    free(output_buf);
    free(intermediate_buf);
    close(input_fd);
    close(output_fd);
    return 0;
}




//  // scp image_process.c pi@192.168.91.55:~

// mode "1680x1050"
//    geometry 1680 1050 1680 1050 16
//     timings 0 0 0 0 0 0 0
//     accel true
//    rgba 5/11,6/5,5/0,0/0
// endmode

// sudo apt-get update && sudo apt-get install fbset
// fbset -i

//    /home/pi/izhod.raw: the path and filename of the output file to be created.
//    O_WRONLY: open the file in write-only mode, allowing write operations but not read  //operations.
//   O_CREAT: if the file does not exist, create it.
//   0644: the file permissions to set if the file is created. In octal notation, 0644 means //that the owner has read and write permissions (6), while group members and others have only read
// permissions (4).

// Together, these arguments create a file descriptor for the output file that can be used to write the converted image data to the file.

// Note that the open function returns a file descriptor, which is an integer value that represents the file and can be used with other system calls like read and write. The file descriptor needs to
//  be closed when the program is done using the file by calling the close function with the file descriptor as an argument.

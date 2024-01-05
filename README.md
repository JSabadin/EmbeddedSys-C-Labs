# EmbeddedSys-C-Labs

## Overview
This repository hosts a collection of embedded systems programming exercises in C, focusing on system calls and various concepts like FIFO, PIPEs, semaphores, and TCP/UDP protocols for image streaming.


## Exercises

### 1. noSync Image Operations
This exercise focuses on implementing non-synchronous image processing operations. It demonstrates the consequeces of when the synchronization between processes such as capturing, updating, and displaying images is not used.

### 2. FIFO Image Capture, Update, and Display
This exercise uses a FIFO buffer to manage image data, demonstrating capture, update, and display functionalities in an embedded context.

### 3. PIPEs for Image Processing Tasks
Focuses on the implementation of PIPEs for inter-process communication, particularly for tasks related to image processing.

### 4. Semaphore-Based Image Operations
Here, semaphores are used to control access to shared resources in concurrent image processing scenarios.

### 5. TCP/UDP Image Streaming
Covers the use of TCP and UDP protocols for efficient image streaming from a camera, highlighting their respective advantages and use-cases.

## Requirements
- C compiler (e.g., GCC): To compile and run the C programs.
- Raspberry Pi: The hardware platform used for these exercises. Any model that supports external hardware interfacing and network connectivity will be suitable.
- Camera Module: A compatible camera module for the Raspberry Pi, used for the image capture exercises.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <malloc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/times.h>
#include <time.h>


#define NPOD 640*480*3
#define SCREEN_SIZE 1280

#define SEM_BERI24 0
#define SEM_PISI24 1
#define SEM_BERI16 2
#define SEM_PISI16 3







int main(int argc, char *argv[]) {
    struct tms cpu;
    int ticks, ctr=1;
    float t1, t2;

    unsigned short SemArray[4];
    struct sembuf Semaphore;

    char *shm16;
    char *shm24;


    int shm24_id = shmget(IPC_PRIVATE, 640*480*3, IPC_CREAT | 0644);
    int shm16_id = shmget(IPC_PRIVATE, 640*480*2, IPC_CREAT | 0644);

    
    shm24 = shmat(shm24_id, NULL, 0);
    shm16  = shmat(shm16_id, NULL, 0);


    int sem_id = semget(IPC_PRIVATE, 4, IPC_CREAT | 0644);

    SemArray[SEM_BERI24] = 0;// Write semaphore for input
    SemArray[SEM_PISI24] = 1;// Read semaphore for input
    SemArray[SEM_BERI16] = 1;
    SemArray[SEM_PISI16] = 0;

    semctl(sem_id, 0, SETALL, SemArray);      // Initialize semaphores
    /*The line semctl(sem_id, 0, SETALL, SemArray);
     sets the initial values for all the semaphores in the semaphore set with the identifier sem_id. T
     he SETALL command is used to indicate that all the semaphores in the set should be initialized with the values provided in the SemArray array.*/
    pid_t pid1 = fork();
    if (pid1 == 0) {
        // Child process 1: read from /dev/video0 and write to fifo24 pipe

        char *pom;
        int fi;
        int n_pod;
        n_pod = NPOD/480;;
        fi = open("/dev/video0", O_RDONLY);

        static clock_t t, start;
        start = clock();

        while (1) {
            
            // Semafor wait - write lock
            Semaphore.sem_num = SEM_BERI24;
            Semaphore.sem_op = -1;
            Semaphore.sem_flg = 0;
            semop(sem_id, &Semaphore, 1);

            read(fi, shm24, n_pod);

            // unlock read
            Semaphore.sem_num = SEM_PISI24;
            Semaphore.sem_op = 1;
            Semaphore.sem_flg = 0;
            semop(sem_id, &Semaphore, 1);


            if(ctr == 10000){
                t = clock() - t;
                double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
                printf("%f s za %d iteracij \n", time_taken, ctr);
                ctr = 0;
                t = clock();
            }
            ctr++;
        }


    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        // Child process 2: read from fifo24 pipe and write to fifo16 pipe
        int fi, a;
        int SIZEI = 640*3;
        int SIZEO = 640*2;
        char *bufI, *bufO, R, G, B;

        while(1) {

            // unlock read
            Semaphore.sem_num = SEM_PISI24;
            Semaphore.sem_op = -1;
            Semaphore.sem_flg = 0;
            semop(sem_id, &Semaphore, 1);

            Semaphore.sem_num = SEM_BERI16;
            Semaphore.sem_op = -1;
            Semaphore.sem_flg = 0;
            semop(sem_id, &Semaphore, 1);

            
            for(a = 0; a <= 640; a++) {
                R = shm24[a*3];
                G = shm24[a*3+1];
                B = shm24[a*3+2];
                
                shm16[a*2+1] = (R & 0xF8) | ((G & 0xE0)>>5);
                shm16[a*2] = ((G & 0x1C)<<3) | ((B & 0xF8)>>3);
            }

            Semaphore.sem_num = SEM_BERI24;
            Semaphore.sem_op = 1;
            Semaphore.sem_flg = 0;
            semop(sem_id, &Semaphore, 1);

            Semaphore.sem_num = SEM_PISI16;
            Semaphore.sem_op = 1;
            Semaphore.sem_flg = 0;
            semop(sem_id, &Semaphore, 1);

           
        }

    }

    else{
        // Parent process: read from fifo16 pipe and write to /dev/fb0
        int fo;
        int SIZEI = 640*2;
        char *buf, *bufO, *bufB;
        buf = (char *) malloc(sizeof(char) * SIZEI);

        int  screenWidth = 1680;
        int screenOffset = screenWidth * 2 - 1280;
        
        fo = open("/dev/fb0", O_WRONLY);




        
        while(1){

            // Semafor wait - write lock
            Semaphore.sem_num = SEM_PISI16;
            Semaphore.sem_op = -1;
            Semaphore.sem_flg = 0;
            semop(sem_id, &Semaphore, 1);

            write(fo, shm16, SIZEI);
            lseek(fo,screenOffset,SEEK_CUR);
            
            if((lseek(fo,0,SEEK_CUR)) >= (screenWidth * 2 * 480)){
                lseek(fo,0,SEEK_SET);
            }

            // unlock read
            Semaphore.sem_num = SEM_BERI16;
            Semaphore.sem_op = 1;
            Semaphore.sem_flg = 0;
            semop(sem_id, &Semaphore, 1);

    
        }
    }

    return 0;
}


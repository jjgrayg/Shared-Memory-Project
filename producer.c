/* Modified from: pshm_ucase_send.c

    Licensed under GNU General Public License v2 or later.

    Jarod Graygo
    producer.c
*/
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "shared_mem.h"

int main() {
    srand(time(NULL));
    char *shmpath = "TABLE";
    int producedCount = 0;

    /* Create shared memory object, if it does not exist, and map it
        into the caller's address space. */
    int fd = shm_open(shmpath, O_CREAT | O_EXCL | O_RDWR,
                        S_IRUSR | S_IWUSR);
    if (fd == -1)
        fd = shm_open(shmpath, O_RDWR, 0);
        if (fd == -1)
            errExit("shm_open");

    struct shmbuf *shmp = mmap(NULL, sizeof(*shmp),
                                PROT_READ | PROT_WRITE,
                                MAP_SHARED, fd, 0);
    if (shmp == MAP_FAILED)
        errExit("mmap");
    
    while(producedCount < 500){

        /* Create data to be stored in shared memory object. */

        int int_1 = rand() % 999;
        int int_2 = rand() % 999;

        /* Copy data into the shared memory object. */

        shmp->int1 = int_1;
        shmp->int2 = int_2;
        printf("PRODUCER: Wrote to shared memory in int1: %d\n", shmp->int1);
        printf("PRODUCER: Wrote to shared memory in int2: %d\n", shmp->int2);

        /* Tell peer that it can now access shared memory. */

        if (sem_post(&shmp->sem1) == -1)
            errExit("sem_post");

        /* Wait until peer says that it has finished accessing
            the shared memory. */

        if (sem_wait(&shmp->sem2) == -1)
            errExit("sem_wait");

        /* Write modified data in shared memory to standard output. */
        if (shmp->int1 == -1)
            printf("PRODUCER: Consumer has consumed int in int1. Current value: %d\n", shmp->int1);
        if (shmp->int2 == -1)
            printf("PRODUCER: Consumer has consumed int in int2. Current value: %d\n", shmp->int2);

        /* Increment producedCount */
        producedCount++;
    }

    exit(EXIT_SUCCESS);
}

/* Modified from: pshm_ucase_bounce.c

    Licensed under GNU General Public License v2 or later.

    Jarod Graygo
    consumer.c
*/
#include <ctype.h>
#include "shared_mem.h"

int main() {
    char *shmpath = "TABLE";
    int consumedCount = 0;

    /* Create shared memory object, if it does not exist, and set its size to the size
        of our structure. */

    int fd = shm_open(shmpath, O_CREAT | O_EXCL | O_RDWR,
                        S_IRUSR | S_IWUSR);
    if (fd == -1)
        fd = shm_open(shmpath, O_RDWR, 0);
        if (fd == -1)
            errExit("shm_open");

    if (ftruncate(fd, sizeof(struct shmbuf)) == -1)
        errExit("ftruncate");

    /* Map the object into the caller's address space. */

    struct shmbuf *shmp = mmap(NULL, sizeof(*shmp),
                                PROT_READ | PROT_WRITE,
                                MAP_SHARED, fd, 0);
    if (shmp == MAP_FAILED)
        errExit("mmap");

    /* Initialize semaphores as process-shared, with value 0. */

    if (sem_init(&shmp->sem1, 1, 0) == -1)
        errExit("sem_init-sem1");
    if (sem_init(&shmp->sem2, 1, 0) == -1)
        errExit("sem_init-sem2");

    while(consumedCount < 50) {
        /* Wait for 'sem1' to be posted by peer before touching
            shared memory. */

        if (sem_wait(&shmp->sem1) == -1)
            errExit("sem_wait");

        /* Print data in stored memory to console. */
        if (shmp->int1 != -1){
            printf("CONSUMER: Recieved integer in int1: %d\n", shmp->int1);
            shmp->int1 = -1;
        }
        if (shmp->int2 != -1){
            printf("CONSUMER: Recieved integer in int2: %d\n", shmp->int2);
            shmp->int2 = -1;
        }

        /* Post 'sem2' to tell the peer that it can now
            access the modified data in shared memory. */

        if (sem_post(&shmp->sem2) == -1)
            errExit("sem_post");

        /* Increment consumedCount */
        consumedCount++;
    }

    /* Unlink the shared memory object. Even if the peer process
        is still using the object, this is okay. The object will
        be removed only after all open references are closed. */

    shm_unlink(shmpath);

    exit(EXIT_SUCCESS);
}
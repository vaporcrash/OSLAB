#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// For semaphore operations sem_init,sem_wait,sem_post
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <sys/types.h>
#include<unistd.h>
extern int errno;
#define SIZE 10 /* size of the shared buffer */
#define VARSIZE 1 /* size of shared variable = 1 byte */
#define INPUTSIZE 20
#define SHMPERM 0666 /* shared memory permissions */
int segid; /* ID for shared memory buffer */
int empty_id;
int full_id;
int mutex_id;
char *buff;
char *input_string;
sem_t *empty;
sem_t *full;
sem_t *mutex;
int p = 0;
int main()
{
    int i = 0;
    pid_t temp_pid;
    segid = shmget(104, SIZE, IPC_CREAT | IPC_EXCL | SHMPERM );
    empty_id=shmget(105, sizeof(sem_t), IPC_CREAT | IPC_EXCL | SHMPERM);
    full_id=shmget(106, sizeof(sem_t), IPC_CREAT | IPC_EXCL | SHMPERM);
    mutex_id=shmget(107,sizeof(sem_t), IPC_CREAT | IPC_EXCL | SHMPERM);
    buff = shmat(segid, (char *)0, 0);
    empty = shmat(empty_id, (char *)0, 0);
    full = shmat(full_id, (char *)0, 0);
    mutex = shmat(mutex_id, (char *)0, 0);
    // Initializing Semaphores Empty, Full & Mutex
    sem_init(empty, 1, 10);
    sem_init(full, 1, 0);
    sem_init(mutex, 1, 1);
    printf("\nProducer Process Started\n");
    while (i < 10)
    {
        int val = random()%10;
        printf("\nProducer %d trying to acquire Semaphore Empty\n", getpid());
        sem_wait(empty);
        printf("\nProducer %d successfully acquired Semaphore Empty\n", getpid());
        printf("\nProducer %d trying to acquire Semaphore Mutex\n", getpid());
        sem_wait(mutex);
        printf("\nProducer %d successfully acquired Semaphore Mutex\n", getpid());
        buff[p] = (char)(val + 48);
        printf("\nProducer %d Produced Item [%d]\n", getpid(), val);
        i++;
        p++;
        printf("\nItems produced: %d\n", p);
        sem_post(mutex);
        printf("\nProducer %d released Semaphore Mutex\n", getpid());
        sem_post(full);
        printf("\nProducer %d released Semaphore Full\n", getpid());
        sleep(2);
    }
    shmdt(buff);
    shmdt(empty);
    shmdt(full);
    shmdt(mutex);
    printf("\nProducer Process Ended\n");
    return(0);
}

/*
Output:
Producer Process Started

Producer 3733 trying to acquire Semaphore Empty

Producer 3733 successfully acquired Semaphore Empty

Producer 3733 trying to acquire Semaphore Mutex

Producer 3733 successfully acquired Semaphore Mutex

Producer 3733 Produced Item [3]

Items produced: 1

Producer 3733 released Semaphore Mutex

Producer 3733 released Semaphore Full

Producer 3733 trying to acquire Semaphore Empty

Producer 3733 successfully acquired Semaphore Empty

Producer 3733 trying to acquire Semaphore Mutex

Producer 3733 successfully acquired Semaphore Mutex

Producer 3733 Produced Item [6]

Items produced: 2

Producer 3733 released Semaphore Mutex

Producer 3733 released Semaphore Full

Producer 3733 trying to acquire Semaphore Empty

Producer 3733 successfully acquired Semaphore Empty

Producer 3733 trying to acquire Semaphore Mutex

Producer 3733 successfully acquired Semaphore Mutex

Producer 3733 Produced Item [7]

Items produced: 3

Producer 3733 released Semaphore Mutex

Producer 3733 released Semaphore Full

Producer 3733 trying to acquire Semaphore Empty

Producer 3733 successfully acquired Semaphore Empty

Producer 3733 trying to acquire Semaphore Mutex

Producer 3733 successfully acquired Semaphore Mutex

Producer 3733 Produced Item [5]

Items produced: 4

Producer 3733 released Semaphore Mutex

Producer 3733 released Semaphore Full

Producer 3733 trying to acquire Semaphore Empty

Producer 3733 successfully acquired Semaphore Empty

Producer 3733 trying to acquire Semaphore Mutex

Producer 3733 successfully acquired Semaphore Mutex

Producer 3733 Produced Item [3]

Items produced: 5

Producer 3733 released Semaphore Mutex

Producer 3733 released Semaphore Full

Producer 3733 trying to acquire Semaphore Empty

Producer 3733 successfully acquired Semaphore Empty

Producer 3733 trying to acquire Semaphore Mutex

Producer 3733 successfully acquired Semaphore Mutex

Producer 3733 Produced Item [5]

Items produced: 6

Producer 3733 released Semaphore Mutex

Producer 3733 released Semaphore Full

Producer 3733 trying to acquire Semaphore Empty

Producer 3733 successfully acquired Semaphore Empty

Producer 3733 trying to acquire Semaphore Mutex

Producer 3733 successfully acquired Semaphore Mutex

Producer 3733 Produced Item [6]

Items produced: 7

Producer 3733 released Semaphore Mutex

Producer 3733 released Semaphore Full

Producer 3733 trying to acquire Semaphore Empty

Producer 3733 successfully acquired Semaphore Empty

Producer 3733 trying to acquire Semaphore Mutex

Producer 3733 successfully acquired Semaphore Mutex

Producer 3733 Produced Item [2]

Items produced: 8

Producer 3733 released Semaphore Mutex

Producer 3733 released Semaphore Full

Producer 3733 trying to acquire Semaphore Empty

Producer 3733 successfully acquired Semaphore Empty

Producer 3733 trying to acquire Semaphore Mutex

Producer 3733 successfully acquired Semaphore Mutex

Producer 3733 Produced Item [9]

Items produced: 9

Producer 3733 released Semaphore Mutex

Producer 3733 released Semaphore Full

Producer 3733 trying to acquire Semaphore Empty

Producer 3733 successfully acquired Semaphore Empty

Producer 3733 trying to acquire Semaphore Mutex

Producer 3733 successfully acquired Semaphore Mutex

Producer 3733 Produced Item [1]

Items produced: 10

Producer 3733 released Semaphore Mutex

Producer 3733 released Semaphore Full

Producer Process Ended
*/
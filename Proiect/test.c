#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "lockers.h"
#include <pthread.h>
#include <errno.h>

//ex1

#define MAX_RESOURCES 10
#define NR_THREADS1 10

int available_resources = MAX_RESOURCES;
mymutex mtx;

int decrease_count(int count)
{
    acquire(&mtx);
    if (available_resources < count)
    {
        release(&mtx);
        return -1;
    }
    else
    {
        available_resources -= count;
        printf("Got  %d  resources  %d  remaining\n", count, available_resources);
        release(&mtx);
        return 0;
    }
}

int increase_count(int count)
{
    acquire(&mtx);
    available_resources += count;
    printf("Released  %d  resources  %d  remaining\n", count, available_resources);
    release(&mtx);
    return 0;
}

void *tfun1(void *v)
{
    int needed_resources = rand() % 5 + 1;
    while (decrease_count(needed_resources) == -1)
        ;
    // do something
    sleep(1);
    increase_count(needed_resources);

    return NULL;
}

int ex1()
{
    pthread_t thr[NR_THREADS1];

    srand(time(NULL));

    initialize(&mtx);

    printf("MAX_RESOURCES=%d\n", MAX_RESOURCES);

    for (int i = 0; i < NR_THREADS1; i++)
    {
        if (pthread_create(&thr[i], NULL, tfun1, NULL))
        {
            perror(NULL);
            return errno;
        }
    }

    for (int i = 0; i < NR_THREADS1; i++)
    {
        if (pthread_join(thr[i], NULL))
        {
            perror(NULL);
            return errno;
        }
    }

    return 0;
}

//ex2

#define NR_THREADS2 5
#define MAX_INCREMENT 1000

int count = 0;
pthread_t thr[NR_THREADS2];
mymutex mtx;
mysemaphore sem;

void barrier_point()
{
    acquire(&mtx);
    count++;
    if (count == NR_THREADS2)
    {
        int k;
        for (k = 0; k < NR_THREADS2; k++)
        {
            signal(&sem);
        }
    }
    release(&mtx);
    wait(&sem);
}

void *tfun2(void *v)
{
    int *tid = (int *)v;
    printf("%d reached the barrier\n", *tid);
    barrier_point();
    printf("%d passed the barrier\n", *tid);
    free(tid);
    return NULL;
}

int ex2()
{
    int i = 0;
    printf("NTHRS=%d\n", NR_THREADS2);
    initialize(&mtx);
    initialize_sem(&sem, 0);

    for (i = 0; i < NR_THREADS2; i++)
    {
        int *x = (int *)malloc(sizeof(int));
        *x = i;
        if (pthread_create(&thr[i], NULL, tfun2, x))
        {
            perror(NULL);
            return errno;
        }
    }
    for (i = 0; i < NR_THREADS2; i++)
    {
        if (pthread_join(thr[i], NULL))
        {
            perror(NULL);
            return errno;
        }
    }

    return 0;
}

// ex3

#define NR_THREADS3 4

myRW_lock rw_lock;

void *writer(void *args)
{
    int *writer_id = (int *)args;

    printf("Writer %d is trying to write\n", *writer_id);

    writer_lock(&rw_lock);

    printf("Writer %d is writing...\n", *writer_id);

    writer_unlock(&rw_lock);

    printf("Writer %d is leaving\n", *writer_id);

    free(writer_id);

    return NULL;
}

void *reader(void *args)
{
    int *reader_id = (int *)args;

    printf("Reader %d is trying to read\n", *reader_id);

    reader_lock(&rw_lock);

    printf("Reader %d is reading...\n", *reader_id);
    usleep(2);

    reader_unlock(&rw_lock);

    printf("Reader %d is leaving\n", *reader_id);

    free(reader_id);

    return NULL;
}

void ex3()
{

    pthread_t writers_threads[NR_THREADS3], readers_threads[NR_THREADS3];

    initialize_RWlock(&rw_lock);

    for(int i = 0; i < NR_THREADS3; i++)
    {
        int *writer_id = (int *)malloc(sizeof(int));
        *writer_id = i + 1;

        int *reader_id = (int *)malloc(sizeof(int));
        *reader_id = i + 1;


        pthread_create(&writers_threads[i], NULL, writer, writer_id);
        pthread_create(&readers_threads[i], NULL, reader, reader_id);
    }

    for(int i = 0; i < NR_THREADS3; i++)
    {
        pthread_join(writers_threads[i], NULL);
        pthread_join(readers_threads[i], NULL);
    }
}

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        if (argv[1][0] == '1' && argv[1][1] == '\0')
        {
            ex1();
        }
        else if (argv[1][0] == '2' && argv[1][1] == '\0')
        {
            ex2();
        }
        else if (argv[1][0] == '3' && argv[1][1] == '\0')
        {
            ex3();
        }
    }
}
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>

#define MAX_RESOURCES 10
#define NR_THREADS 10

int available_resources = MAX_RESOURCES;
pthread_mutex_t mtx;

int decrease_count(int count)
{
    pthread_mutex_lock(&mtx);
    if (available_resources < count)
    {
        pthread_mutex_unlock(&mtx);
        return -1;
    }
    else
    {
        available_resources -= count;
        printf("Got  %d  resources  %d  remaining\n", count, available_resources);
        pthread_mutex_unlock(&mtx);
        return 0;
    }
}

int increase_count(int count)
{
    pthread_mutex_lock(&mtx);
    available_resources += count;
    printf("Released  %d  resources  %d  remaining\n", count, available_resources);
    pthread_mutex_unlock(&mtx);
    return 0;
}

void *tfun(void *v)
{
    int needed_resources = rand() % 5 + 1;
    while (decrease_count(needed_resources) == -1)
        ;
    // do something
    sleep(1);
    increase_count(needed_resources);

    return NULL;
}

int main()
{
    pthread_t thr[NR_THREADS];

    srand(time(NULL));

    if (pthread_mutex_init(&mtx, NULL))
    {
        perror(NULL);
        return errno;
    }

    printf("MAX_RESOURCES=%d\n", MAX_RESOURCES);

    for (int i = 0; i < NR_THREADS; i++)
    {
        if (pthread_create(&thr[i], NULL, tfun, NULL))
        {
            perror(NULL);
            return errno;
        }
    }

    for (int i = 0; i < NR_THREADS; i++)
    {
        if (pthread_join(thr[i], NULL))
        {
            perror(NULL);
            return errno;
        }
    }

    pthread_mutex_destroy(&mtx);
    return 0;
}

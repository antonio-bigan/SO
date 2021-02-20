#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <semaphore.h>

#define NR_THREADS 5

pthread_mutex_t mtx;
sem_t sem;
int count;

void init(int N)
{
    count = N;
    if (sem_init(&sem, 0, 0))
    {
        perror(NULL);
        exit(errno);
    }
}

void barrier_point()
{
    static int ct = 0;

    pthread_mutex_lock(&mtx);
    ct++;
    if (ct == count)
    {
        for (int i = 0; i < count; i++)
        {
            if (sem_post(&sem))
            {
                perror(NULL);
                exit(errno);
            }
        }
    }
    pthread_mutex_unlock(&mtx);

    if (sem_wait(&sem))
    {
        perror(NULL);
        exit(errno);
    }
}

void *tfun(void *v)
{
    int *tid = v;
    printf("%d reached the barrier\n", *tid);
    barrier_point();
    printf("%d passed the barrier\n", *tid);

    free(tid);
    return NULL;
}

int main()
{
    pthread_t thr[NR_THREADS];

    if (pthread_mutex_init(&mtx, NULL))
    {
        perror(NULL);
        return errno;
    }

    init(NR_THREADS);

    printf("NR_THREADS=%d\n", NR_THREADS);

    for (int i = 0; i < NR_THREADS; i++)
    {
        int *param = malloc(sizeof(int));
        *param = i;
        if (pthread_create(&thr[i], NULL, tfun, param))
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
    sem_destroy(&sem);
    return 0;
}

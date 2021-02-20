#include "lockers.h"
#include <string.h>
#include <stdlib.h>

void acquire(mymutex *mutex)
{
    while (__sync_val_compare_and_swap(&mutex->available, 0, 1) != 0)
        ;
}

void release(mymutex *mutex)
{
    mutex->available = 0;
}

void initialize(mymutex *mutex)
{
    mutex->available = 0;
}

int wait(mysemaphore *s)
{
    acquire(&s->mut);
    while (atomic_load(&s->val) <= 0)
        ;
    atomic_fetch_sub(&s->val, 1);
    release(&s->mut);
    return 0;
}

int signal(mysemaphore *s)
{
    return atomic_fetch_add(&s->val, 1);
}

void initialize_sem(mysemaphore *s, int val)
{
    atomic_init(&s->val, val);
}

void initialize_RWlock(myRW_lock *rw_lock)
{
    initialize_sem(&rw_lock->x, 1);
    initialize_sem(&rw_lock->y, 1);
    rw_lock->readers_count = 0;
}

void reader_lock(myRW_lock *rw_lock)
{
    wait(&rw_lock->x);
    rw_lock->readers_count++;

    if (rw_lock->readers_count == 1)
        wait(&rw_lock->y);

    signal(&rw_lock->x);
}

void reader_unlock(myRW_lock *rw_lock)
{
    wait(&rw_lock->x);
    rw_lock->readers_count--;
    if (rw_lock->readers_count == 0)
        signal(&rw_lock->y);
    signal(&rw_lock->x);
}

void writer_lock(myRW_lock *rw_lock)
{
    wait(&rw_lock->y);
}

void writer_unlock(myRW_lock *rw_lock)
{
    signal(&rw_lock->y);
}
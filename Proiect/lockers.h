#ifndef __LOCKERS_H__
#define __LOCKERS_H__
#include <stdatomic.h>

typedef struct
{
    int available;
} mymutex;

void acquire(mymutex *mutex);
void release(mymutex *mutex);
void initialize(mymutex *mutex);

typedef struct
{
    int val;
    mymutex mut;
} mysemaphore;

int wait(mysemaphore *s);
int signal(mysemaphore *s);
void initialize_sem(mysemaphore *s, int val);

typedef struct
{
    mysemaphore x;
    mysemaphore y;
    int readers_count;
} myRW_lock;

void initialize_RWlock(myRW_lock *rw_lock);
void reader_lock(myRW_lock *rw_lock);
void reader_unlock(myRW_lock *rw_lock);
void writer_lock(myRW_lock *rw_lock);
void writer_unlock(myRW_lock *rw_lock);

#endif
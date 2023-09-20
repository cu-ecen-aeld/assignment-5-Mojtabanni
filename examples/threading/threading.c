#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)


pthread_mutex_t my_mutex = PTHREAD_MUTEX_INITIALIZER;

void* threadfunc(void* thread_param)
{
    struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    usleep (thread_func_args->obtain_ms*1000);
    pthread_mutex_lock(thread_func_args->mutex);
    usleep (thread_func_args->release_ms*1000);
    pthread_mutex_unlock(thread_func_args->mutex);
    thread_func_args->thread_complete_success = true;
    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    //struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    return thread_func_args;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
    struct thread_data *data;
    int ret;

    data = calloc(1, sizeof(struct thread_data));
    if (data == NULL){
        perror("calloc");
        return false;
    }

    data->mutex = mutex;
    data->obtain_ms = wait_to_obtain_ms;
    data->release_ms = wait_to_release_ms;

    
    ret = pthread_create(thread, NULL, threadfunc, (void *) data);
    if (ret) {
        errno = ret;
        perror ("pthread_create");
        return false;
    }
    
    
    return true;
}

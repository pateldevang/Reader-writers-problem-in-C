#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>

// Mutex declared
sem_t wrt;
sem_t mutex;

int readcount=0;
int i;

//MARK: - Readers function
void *reader(void *arg)
{
    // Sem wait added for semophore waiting if not added then the process will be in deadlock.
    sem_wait(&mutex);
    readcount++;
    if(readcount==1)
        sem_wait(&wrt);
    sem_post(&mutex);
    printf("reader is being performed - %d\n",readcount);
    sem_wait(&mutex);
    readcount--;
    if(readcount==0)
        sem_post(&wrt);
    sem_post(&mutex);
}

//MARK - Writers function
void *writer(void *arg)
{
    sem_wait(&wrt);
    printf("writing is being performed \n");
    sem_post(&wrt);
}

//MARK: - Main function
int main()
{
    // Initialize the pthread_t objects from the pthread.h library
    pthread_t tid1,tid2;
    // initialize the mutex for the the above declared
    sem_init(&mutex,0,1);
    sem_init(&wrt,0,1);
    //Create pthread for writers
    pthread_create(&tid1,NULL,writer,NULL);
    // Create pthread for readers
    for(i=0;i<4;i++)
    {
        pthread_create(&tid2,NULL,reader,NULL);
    }
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    // Destroying the mutex after the using it if not required.
    sem_destroy(&mutex);
    sem_destroy(&wrt);
    return 0;
}

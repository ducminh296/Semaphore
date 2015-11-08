/* shared_memory.c
**
** Implement method to deal with shared shared_memory
**
**/
#include "shared_memory.h"

#define KEY 1234 /* shared memory key */
static int shmid; /* ID of shared memory */
static void *shared_memory = (void*)0; /* shared_memory */

/* define a queue */
static struct queue_t *queue;
/*
** This method initilize the queue before using 
*/
void init_queue()
{
	queue->in = 0;
	queue->out = 0;
	queue->k = 100;
}
/*
** This method add a new buffer into the queue
** 
*/
void append(struct buffer_t buffer)
{

	queue->array[queue->in] = buffer;
	queue->in = (queue->in + 1) % queue->k;
}
/*
** This method take a buffer from the queue
*/
struct buffer_t take()
{
	struct buffer_t w;
	w = queue->array[queue->out];
	queue->out = (queue->out + 1) % queue->k;
	return w;
}
/*
** This method get a shared memory associated with KEY
**
*/
void get_shm()
{
	shmid = shmget((key_t)KEY, sizeof(struct queue_t), 0666 | IPC_CREAT);
	if (shmid == -1)
	{
		fprintf(stderr, "%d: Failed to get a shared memory", getpid());
		exit(EXIT_FAILURE);
	}
}
/*
** This method attach the shared memory to the address space of
** of the process
*/
void attach_shm()
{
	shared_memory = shmat(shmid, (void *)0, 0);
	if (shared_memory == (void *)-1) {
        fprintf(stderr, "%d: shared memory attach failed.\n", getpid());
        exit(EXIT_FAILURE);
    }
    printf("%d: Memory attached at %X.\n",getpid(), (int)shared_memory);
    /* cast memory to queue type */
    queue = (struct queue_t *)shared_memory;
}
/*
** This method dettach the shared memory
**
*/
void detach_shm()
{
	if (shmdt(shared_memory) == -1) {
        fprintf(stderr, "%d: shared memory detach failed.\n",getpid());
        exit(EXIT_FAILURE);
    }
}
/*
** This method delete the shared_memory
**
*/
void delete_shm()
{
	if (shmctl(shmid, IPC_RMID, 0) == -1) {
        fprintf(stderr, "delete shared memory failed\n");
        exit(EXIT_FAILURE);
    }
}

/*
** This method store number of bytes read from the producer into shm
**
*/
void number_of_bytes_to_shm(int num)
{
	queue->num_bytes = num;
}
/*
** This method store the file size into the shm
**
*/
void file_size_to_shm(int size)
{
	queue->file_size = size;
}
/*
** This method return the number of bytes read from the producer into shm
**
*/
int get_number_of_bytes()
{
	return queue->num_bytes;
}
/*
** This method return the input file size
**
*/
int get_file_size()
{
	return queue->file_size;
}

/* shared_memory.h
**
** Define method to deal with shared_memory
** The shared_memory is a queue which has 100 buffers
** Each buffer contain 128 bytes
**
*/
#include <fcntl.h> 
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/shm.h>

/* define a buffer with 128 elements */
struct buffer_t{
    int len;
    char string[128];
};
/* Define a queue containing 100 buffers */
struct queue_t{
	int in;
	int out;
	int k;
	struct buffer_t array[100];
	int num_bytes;//number of bytes read
	int file_size;//file size
};

/* method to deal with the queue */
void init_queue();//init the queue
void append(struct buffer_t buffer);//Add new buffer into the queue
struct buffer_t take();//Take buffer from the queue

/* method to deal with shared memory */
void get_shm();//Get the shared memory
void attach_shm();//attach shared memory to address space of process
void detach_shm();//detach shared memory from address space of process
void delete_shm();//delete shared memory

/* menthod to deal with some statistic info */
void number_of_bytes_to_shm(int num);//store number of bytes read into shm
int get_number_of_bytes();//return the number of bytes read into shm
void file_size_to_shm(int size);//store the file size into shm
int get_file_size();//return the input file size





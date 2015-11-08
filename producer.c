/* producer.c
** Author: Minh Mai  - 100845949
**
** This file create a producer process to read file and put it into shared memory
**
** In the terminal: ./producer
**
*/

#include <fcntl.h> 
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include "semaphore.h"
#include "shared_memory.h"

#define INPUT "input.txt"
int main(int argc, char *argv[])
{
	int file;//file descriptor
	struct buffer_t buffer; //a buffer of 128 characters
	struct stat st;//for reading file size
	char input_buffer[BUFSIZ]; //the input buffer
	int total_bytes=0;// total of bytes in the input buffer
	int bytes = 0;//bytes from one read operation
	int count = 0;

	get_sem();//init the semaphores
	set_semvalue();//set values for semaphores
	get_shm();//init shared memory
	attach_shm();//attach shared memory
	file = open(INPUT,O_RDONLY);//open file for reading
	stat(INPUT, &st); //obtain the size of the input file
	//store the size of input file to shm
	wait_s();
	file_size_to_shm(st.st_size); //critical section
	init_queue();//initialize the queue
	signal_s();
	signal_n();
	do
	{
		/* read text file into buffer */
		bytes = read(file, input_buffer, BUFSIZ);
		total_bytes += bytes;
		if (bytes == -1)
		{
			fprintf(stderr,"%d: Fail to read file.",getpid());
			exit(EXIT_FAILURE);
		}
		while (count<bytes)
		{
			if (bytes - count >= 128)
				buffer.len = 128;
			else
				buffer.len = bytes - count;
			strncpy(buffer.string,input_buffer + count,buffer.len);
			wait_e();
			wait_s();
			append(buffer);
			signal_s();
			signal_n();
			count += buffer.len;
			sleep(2); //sleep for 2 seconds to slow down the producer
		}
	} while (bytes != 0);
	printf("%d: Total number of bytes written into shared_memory is: %d.\n",getpid(),total_bytes);
	/* Store total_bytes to shared memory */
	wait_s();
	number_of_bytes_to_shm(total_bytes);
	signal_s();
	signal_n();
	detach_shm();//detach the shared memory
	close(file);//close file
}
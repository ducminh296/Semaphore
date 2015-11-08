/* consumer.c
** Author: Minh Mai  - 100845949
**
** This file create a consumer process to read buffer from shared memory
** and write to a file
**
** In the terminal: ./consumer
**
*/
#include <fcntl.h> 
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "semaphore.h"
#include "shared_memory.h"

#define OUTPUT "ouput.txt"
int main(int argc, char *argv[])
{
	int file;//file descriptor
	int number_of_bytes;//number of bytes the producer write into shm
	int total_write_bytes = 0;//total of bytes write into file
	int input_file_size;//size of the input file
	int write_bytes = 0; //number of bytes write into file
	struct buffer_t buffer; //a buffer of 128 characters

	get_sem();//init the semaphores
	get_shm();//init shared memory
	attach_shm();//attach shared memory
	file = open(OUTPUT, O_WRONLY|O_CREAT);//open file for writting

	/* obtain the input file size from the shared memory */
	wait_n();
	wait_s();
	input_file_size = get_file_size();
	signal_s();
	while (total_write_bytes < input_file_size)
	{
		/* Get a buffer from the shared memory */
		wait_n();
		wait_s();
		buffer = take();
		signal_s();
		signal_e();
		write_bytes = write(file, buffer.string, buffer.len);
		if (write_bytes == -1)
		{
			fprintf(stderr,"%d: Fail to write buffer to file.\n",getpid());
			exit(EXIT_FAILURE);
		}
		if (write_bytes < buffer.len)
		{
			fprintf(stderr, "%d: Number of bytes write to file not equal buffer len.\n",getpid());
		}
		total_write_bytes +=buffer.len;
	}
	/* Get the total number of bytes the producer written into shared memory */
	wait_n();
	wait_s();
	number_of_bytes = get_number_of_bytes();
	signal_s();
	/* Output */
	printf("%d: Total number of bytes read from shared memory: %d.\n", getpid(), total_write_bytes);
	printf("%d: Bytes count from the producer: %d.\n",getpid(),number_of_bytes);
	if (total_write_bytes == number_of_bytes)
		printf("%d: Bytes count is identical to the one printed from the producer.\n",getpid());
	else
		printf("%d: Bytes count is not identical to the one printed from the producer.\n",getpid());

	del_semvalue();//delete the semaphore
	detach_shm();//detach the shared memory
	delete_shm();//delete the shared memory
	close(file);//close file
}
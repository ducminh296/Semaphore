/* semaphore.h
**
** Define semaphore method to synchronize the producer with consumer
**
*/
#include <sys/sem.h>
#include <fcntl.h> 
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
void get_sem(void); // get a semaphore; sucess return 0; error return -1
void set_semvalue(void); //set value for the semaphore
void del_semvalue(void); //delete the semaphore

/* Semaphore s */
void wait_s(void);
void signal_s(void);
/* Semaphore n */
void wait_n(void);
void signal_n(void);
/* Semaphore e */
void wait_e(void);
void signal_e(void);


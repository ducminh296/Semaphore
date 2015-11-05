/* semaphore.h
**
** Define semaphore method to synchronize the producer with consumer
**
*/
#include <sys/sem.h>

int create_sem(void); // create a semaphore; sucess return 0; error return -1
int set_semvalue(int val); //set value for the semaphore
void del_semvalue(void); //delete the semaphore
int wait(void); // call wait in semaphore
int signal(void); // call signal in sephmaphore


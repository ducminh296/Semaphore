/* semaphore.c
**
** Implement the semaphore
**
**/

#include "semaphore.h"

static int sem_s = 0; // semaphore s
static int sem_n = 0; // semaphore n
static int sem_e = 0; // semaphore e
static void semaphore_wait(int sem_id); // call wait in semaphore
static void semaphore_signal(int sem_id); // call signal in semaphore
/* The function create a semaphore */
void get_sem(void)
{
    /* Create semaphore s */
	sem_s = semget((key_t)1234, 1, 0666 | IPC_CREAT);
    if (sem_s == -1) {
        fprintf(stderr, "%d: Failed to initialize semaphore\n", getpid());
        exit(EXIT_FAILURE);
    }
    /* Create semaphore n */
    sem_n = semget((key_t)4567, 1, 0666 | IPC_CREAT);
    if (sem_n == -1) {
        fprintf(stderr, "%d: Failed to initialize semaphore\n", getpid());
        exit(EXIT_FAILURE);
    }
    /* Create semaphore e */
    sem_e = semget((key_t)7890, 1, 0666 | IPC_CREAT);
    if (sem_e == -1) {
        fprintf(stderr, "%d: Failed to initialize semaphore\n", getpid());
        exit(EXIT_FAILURE);
    }
}
/* The function set_semvalue initializes the semaphore using the SETVAL command in a
 semctl call. We need to do this before we can use the semaphore. */

void set_semvalue(void)
{
    union semun sem_union;
    /* set semaphore s = 1 */
    sem_union.val = 1;
    if (semctl(sem_s, 0, SETVAL, sem_union) == -1)
    {
        fprintf(stderr, "%d: Cannot set value for semaphore s.\n", getpid());
        exit(EXIT_FAILURE);
    }
    /* set semaphore n = 0 */
    sem_union.val = 0;
    if (semctl(sem_n, 0, SETVAL, sem_union) == -1)
    {
        fprintf(stderr, "%d: Cannot set value for semaphore n.\n", getpid());
        exit(EXIT_FAILURE);
    }
    /* set semaphore e = 100 */
    sem_union.val = 100;
    if (semctl(sem_e, 0, SETVAL, sem_union) == -1)
    {
        fprintf(stderr, "%d: Cannot set value for semaphore e.\n", getpid());
        exit(EXIT_FAILURE);
    }
}

/* The del_semvalue function has almost the same form, except the call to semctl uses
 the command IPC_RMID to remove the semaphore's ID. */

void del_semvalue(void)
{
    union semun sem_union;
    /* Delete semaphore s */
    if (semctl(sem_s, 0, IPC_RMID, sem_union) == -1)
    {
        fprintf(stderr, "%d: Failed to delete semaphore.\n", getpid());
        exit(EXIT_FAILURE);
    }
    /* Delete semaphore n */
    if (semctl(sem_n, 0, IPC_RMID, sem_union) == -1)
    {
        fprintf(stderr, "%d: Failed to delete semaphore.\n", getpid());
        exit(EXIT_FAILURE);
    }
    /* Delete semaphore e */
    if (semctl(sem_e, 0, IPC_RMID, sem_union) == -1)
    {
        fprintf(stderr, "%d: Failed to delete semaphore.\n", getpid());
        exit(EXIT_FAILURE);
    }
}
/* wait for semaphore s */
void wait_s(void)
{
    semaphore_wait(sem_s);
}
/* signal for semaphore s */
void signal_s(void)
{
    semaphore_signal(sem_s);
}
/* wait for semaphore n */
void wait_n(void)
{
    semaphore_wait(sem_n);
}
/* signal for semaphore n */
void signal_n(void)
{
    semaphore_signal(sem_n);
}
/* wait for semaphore e */
void wait_e(void)
{
    semaphore_wait(sem_e);
}
/* signal for semaphore e */
void signal_e(void)
{
    semaphore_signal(sem_e);
}
/* semaphore_p changes the semaphore by -1 (waiting). */

static void semaphore_wait(int sem_id)
{
    struct sembuf sem_b;
    
    sem_b.sem_num = 0;
    sem_b.sem_op = -1; /* P() */
    sem_b.sem_flg = 0;
    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "%d: semaphore wait failed\n", getpid());
        exit(EXIT_FAILURE);
    }
}

/* semaphore_v is similar except for setting the sem_op part of the sembuf structure to 1,
 so that the semaphore becomes available. */

static void semaphore_signal(int sem_id)
{
    struct sembuf sem_b;
    
    sem_b.sem_num = 0;
    sem_b.sem_op = 1; /* V() */
    sem_b.sem_flg = 0;
    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "%d: semaphore signal failed\n", getpid());
        exit(EXIT_FAILURE);
    }
}

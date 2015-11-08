CC = gcc
DEBUG= NODEBUG
CFLAGS = -Wall -c -D$(DEBUG)
LFLAGS = -Wall -D$(DEBUG)

all: producer consumer
producer: producer.o semaphore.o shared_memory.o
	$(CC) $(LFLAGS) producer.o semaphore.o shared_memory.o -o producer
consumer: consumer.o semaphore.o shared_memory.o
	$(CC) $(LFLAGS) consumer.o semaphore.o shared_memory.o -o consumer
semaphore.o: semaphore.c semaphore.h
	$(CC) $(CFLAGS) semaphore.c
shared_memory.o: shared_memory.c shared_memory.h
	$(CC) $(CFLAGS) shared_memory.c
producer.o: producer.c semaphore.h shared_memory.h
	$(CC) $(CFLAGS) producer.c
consumer.o: consumer.c semaphore.h shared_memory.h
	$(CC) $(CFLAGS) consumer.c
clean:
	rm -rf *.o
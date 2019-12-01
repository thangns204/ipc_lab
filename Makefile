CC=gcc
CFLAGS=-Wall -Werror -c -Wextra

all: part1 

part1: task1 

task1: task1_server task1_client task2_server task2_client task3_server task3_client

task1_server: task1_server.o
	$(CC) $^ -o $@
		
task1_server.o: task1_server.c
	$(CC) $(CFLAGS) $^ -o $@

task1_client: task1_client.o
	$(CC) $^ -o $@
		
task1_client.o: task1_client.c
	$(CC) $(CFLAGS) $^ -o $@

task2_server: task2_server.o
	$(CC) $^ -o $@
		
task2_server.o: task2_server.c
	$(CC) $(CFLAGS) $^ -o $@

task2_client: task2_client.o
	$(CC) $^ -o $@
		
task2_client.o: task2_client.c
	$(CC) $(CFLAGS) $^ -o $@

task3_server: task3_server.o
	$(CC) $^ -o $@
		
task3_server.o: task3_server.c
	$(CC) $(CFLAGS) $^ -o $@

task3_client: task3_client.o
	$(CC) $^ -o $@
		
task3_client.o: task3_client.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o task{1..1}{_client,_server,''} 


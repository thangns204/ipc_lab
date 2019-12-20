TARGET = client server
normal: $(TARGET)
server: server.c
	gcc -pthread server.c -lpthread -o server

client: client.c
	gcc client.c -o client
clean:
	$(RM) $(TARGET)
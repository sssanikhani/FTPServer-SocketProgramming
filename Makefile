CC = g++
SERVER_DIR = Server/
CLIENT_DIR = Client/
UTILS_DIR = Utils/

all: server client

server: server.o utils.o
	$(CC) -w -o $(SERVER_DIR)server $(SERVER_DIR)server.o $(UTILS_DIR)utils.o

server.o: utils.o
	$(CC) -w -c $(SERVER_DIR)server.cpp -o $(SERVER_DIR)server.o

client: client.o utils.o
	$(CC) -w -o $(CLIENT_DIR)client $(CLIENT_DIR)client.o $(UTILS_DIR)utils.o

client.o:
	$(CC) -w -c $(CLIENT_DIR)client.cpp -o $(CLIENT_DIR)client.o

utils.o:
	$(CC) -w -c $(UTILS_DIR)utils.cpp -o $(UTILS_DIR)utils.o


clean:
	rm -f $(SERVER_DIR)*.o
	rm -f $(CLIENT_DIR)*.o
	rm -f $(UTILS_DIR)*.o
	rm -f $(SERVER_DIR)server
	rm -f $(CLIENT_DIR)client
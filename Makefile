CC = g++
ISO = --std=c++17
SERVER_DIR = Server/
CLIENT_DIR = Client/
UTILS_DIR = Utils/


all: server client

server: server.o utils.o user.o server/client.o database.o commands.o responses.o
	$(CC) $(ISO) -o $(SERVER_DIR)server \
					$(SERVER_DIR)server.o \
					$(SERVER_DIR)user.o \
					$(SERVER_DIR)client.o \
					$(SERVER_DIR)database.o \
					$(SERVER_DIR)commands.o \
					$(SERVER_DIR)responses.o \
					$(UTILS_DIR)utils.o

server.o: utils.o user.o database.o commands.o responses.o
	$(CC) $(ISO) -c $(SERVER_DIR)server.cpp -o $(SERVER_DIR)server.o

client: client.o utils.o
	$(CC) $(ISO) -o $(CLIENT_DIR)client $(CLIENT_DIR)client.o $(UTILS_DIR)utils.o

client.o:
	$(CC) $(ISO) -c $(CLIENT_DIR)client.cpp -o $(CLIENT_DIR)client.o

utils.o:
	$(CC) $(ISO) -c $(UTILS_DIR)utils.cpp -o $(UTILS_DIR)utils.o

user.o:
	$(CC) $(ISO) -c $(SERVER_DIR)User.cpp -o $(SERVER_DIR)user.o

server/client.o:
	$(CC) $(ISO) -c $(SERVER_DIR)Client.cpp -o $(SERVER_DIR)client.o

database.o: user.o server/client.o
	$(CC) $(ISO) -c $(SERVER_DIR)DataBase.cpp -o $(SERVER_DIR)database.o

commands.o: database.o responses.o user.o server/client.o
	$(CC) $(ISO) -c $(SERVER_DIR)CommandHandler.cpp -o $(SERVER_DIR)commands.o

responses.o:
	$(CC) $(ISO) -c $(SERVER_DIR)Responses.cpp -o $(SERVER_DIR)responses.o


clean:
	rm -f *.o
	rm -f $(SERVER_DIR)*.o
	rm -f $(CLIENT_DIR)*.o
	rm -f $(UTILS_DIR)*.o
	rm -f $(SERVER_DIR)server
	rm -f $(CLIENT_DIR)client
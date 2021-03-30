CC = g++
ISO = --std=c++17


all: server client

server: server.o utils.o User.o Client.o DataBase.o CommandHandler.o Responses.o
	$(CC) $(ISO) -o server \
					server.o \
					User.o \
					Client.o \
					DataBase.o \
					CommandHandler.o \
					Responses.o \
					utils.o

server.o:
	$(CC) $(ISO) -c server.cpp -o server.o

client: client.o utils.o
	$(CC) $(ISO) -o client client.o utils.o

client.o:
	$(CC) $(ISO) -c client.cpp -o client.o

utils.o:
	$(CC) $(ISO) -c utils.cpp -o utils.o

User.o:
	$(CC) $(ISO) -c User.cpp -o User.o

Client.o:
	$(CC) $(ISO) -c Client.cpp -o Client.o

DataBase.o:
	$(CC) $(ISO) -c DataBase.cpp -o DataBase.o

CommandHandler.o:
	$(CC) $(ISO) -c CommandHandler.cpp -o CommandHandler.o

Responses.o:
	$(CC) $(ISO) -c Responses.cpp -o Responses.o


clean:
	rm -f *.o
	rm -f server
	rm -f client
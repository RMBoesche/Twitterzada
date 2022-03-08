all: client server

client:
	g++ -o client.exe src/client/client.cpp src/client/modules/communication.cpp src/client/modules/interface.cpp src/client/modules/validation.cpp src/client/modules/receiver.cpp -std=c++11 -lpthread

server:
	g++ -o server.exe src/server/server.cpp -std=c++11 -lpthread
	
clean:
	rm *.exe
all: client server

client:
	g++ -o client.exe src/client/client.cpp src/client/modules/communication.cpp src/client/modules/interface.cpp src/client/modules/validation.cpp src/client/modules/receiver.cpp -std=c++11 -lpthread

server:
	g++ -o server.exe src/server/server.cpp  src/server/modules/mainConnection.cpp src/server/modules/userSession.cpp src/server/modules/dbManager.cpp -std=c++11 -lpthread
	
clean:
	rm *.exe
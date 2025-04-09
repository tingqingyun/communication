all:server client
server:test_server.cpp server.cpp server.h
	g++ -std=c++11 test_server.cpp server.cpp -o server -lpthread -lmysqlclient
client:test_client.cpp client.cpp client.h
	g++ -std=c++11 test_client.cpp client.cpp -o client -lpthread 
clean:
	rm server
	rm client

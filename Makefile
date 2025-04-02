
test_server:test_server.cpp server.cpp server.h
	g++ -std=c++11 test_server.cpp server.cpp -o server -lpthread
test_client:test_client test_client.cpp client.cpp -lpthread
	g== -std=c++11 test_client.cpp client.cpp -o client -lpthread
clean:
	rm server
	rm client

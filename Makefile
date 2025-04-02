
test_server:test_server.cpp server.cpp server.h
	g++ -std=c++11 test_server.cpp server.cpp -o server -lpthread
clean:
	rm server

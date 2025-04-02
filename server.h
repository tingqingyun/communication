//if have loaded the server.h,then donet need load it again
#ifndef SERVER_H
#define SERVER_H
#include<iostream>
#include<vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <thread>
using namespace std;
class server
{
	private:
	//port
	int server_port;
	//taojiezi 
	int server_sockfd;
	//ip
	string server_ip;
	//save all taojiezi
	vector<int> sock_arr;
	public:
	server(int port,string ip);
	~server();
	void run();
	static void RecvMsg(int conn);
};
#endif

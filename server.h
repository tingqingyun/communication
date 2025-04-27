//if have loaded the server.h,then donet need load it again
#ifndef SERVER_H
#define SERVER_H

#include "headers.h"
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
	static std::vector<bool> sock_arr;
	public:
	server(int port,string ip);
	~server();
	void run();
	static void RecvMsg(int conn);
	static void HandleRequest(int conn,string str);
};
#endif

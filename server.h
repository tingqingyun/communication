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
	//name and taojiezi 
	static unordered_map<string,int> name_sock_map;
	//hu chi suo
	static pthread_mutex_t name_sock_mutx;
	static unordered_map<int,set<int>>group_map;
	static pthread_mutex_t group_mutx;//lock group_map
	static pthread_mutex_t from_mutex;//lock from_map
	public:
	server(int port,string ip);
	~server();
	void run();
	static void RecvMsg(int conn);
	static void HandleRequest(int conn,string str,tuple<bool,string,string,int,int>&info);
	static void setnonblocking(int conn);
};
#endif

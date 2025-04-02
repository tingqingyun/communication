//if have loaded the server.h,then donet need load it again
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
	static vector<bool> sock_arr;
	public:
	server(int port,string ip);
	~server();
	void run();
	static void RecvMsg(int conn);
};
#endif

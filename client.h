#include "headers.h"

class client
{
	private:
		int server_port;
		string server_ip;
		int sock;
	public:
		client(int port,string ip);
		~client();
		void run();
		static void SendMsg(int conn);
		static void RecvMsg(int conn);
		void HandleClient(int conn);
};

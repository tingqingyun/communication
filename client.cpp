#include "client.h"

void client::run()
{
	//define sockfd taojiezi ,header is <sys/socket.h>
	int sock_cli = socket(AF_INET, SOCK_STREAM, 0);
	//define server sockaddr_in,hearder is <netinet/in.h>
	struct sockaddr_in servaddr;
	//initialize struct,header is <string.h>
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	//server port,header is <arpa/inet.h>
	servaddr.sin_port = htons(8023);
	//server ipaddress,header is <arpa/inet.h>
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//connect the client with server
	if(connect(sock_cli, (struct sockaddr*)&servaddr,sizeof(servaddr))<0)
	{
		perror("connect");
		exit(1);
	}
	cout<<"connect server succussfully\n";
	//claim send thread and recv thread
	thread send_t(SendMsg,sock),recv_t(RecvMsg,sock);
	
	send_t.detach();
	recv_t.detach();
	return;
}

void client::SendMsg(int conn)
{
	
	//define send buff
	//char sendbuf[100];
	string input;
	//define receive buff
	//char recbuff[100];
	//send message
	while(1)
	{
		//clean snedbuff
		//memset(sendbuf,0,sizeof(sendbuf));
		//cin.ignore();
		//input message
		getline(cin,input);
		cout<<"sending message: "<<input<<"\n";
		//send the input
		send(sock_cli,input.c_str(),input.size(),0);
		cout<<"sended message"<<input<<"\n";
		//exit fiction
		if(input == "exit")
			break;
	}

void client::RecvMsg(int conn)
{
	char buffer[1000];
	while(1)
	{
		memset(buffer,0,sizeof(buffer));
		int len = recv(conn,buffer,sizeof(buffer),0);
		if(len<=0)
			break;
		cout<<"receive serfer:"<<buffer<<endl;
	}
}
 

#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
using namespace std;

int main()
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
	//close the connect
	close(sock_cli);
	return 0;
}

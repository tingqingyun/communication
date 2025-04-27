#include "client.h"

client::client(int port,string ip):server_port(port),server_ip(ip){}
client::~client()
{
	close(sock);
}

void client::run()
{
	//define sockfd taojiezi ,header is <sys/socket.h>
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	//define server sockaddr_in,hearder is <netinet/in.h>
	struct sockaddr_in servaddr;
	//initialize struct,header is <string.h>
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	//server port,header is <arpa/inet.h>
	servaddr.sin_port = htons(server_port);
	//server ipaddress,header is <arpa/inet.h>
	servaddr.sin_addr.s_addr = inet_addr(server_ip.c_str());
	//connect the client with server
	if(connect(sock, (struct sockaddr*)&servaddr,sizeof(servaddr))<0)
	{
		perror("connect");
		exit(1);
	}
	else
	cout<<"connect server succussfully\n";
	HandleClient(sock);
	return;
}

void client::HandleClient(int conn)
{
	int choice;
	string name,pass,pass1;
	bool if_login=false;
	string login_name;
	cout<<"----------------\n";
	cout<<"input the choice\n";
	cout<<"     0:exit     \n";
	cout<<"    1:login     \n";
	cout<<"   2:register   \n";
	cout<<"----------------\n\n";
	while(1)
	{
		cin>>choice;
		if(choice==0)
		{
			break;
		}	
		if(choice==1&&!if_login)
		{
			while(1)
			{
				cout<<"name:";
				cin>>name;
				cout<<"password:";
				cin>>pass;
				string str = "login"+name;
				str+="pass:";
				str+=pass;
				send(conn,str.c_str(),str.length(),0);
				cout<<"str: "<<str<<"\n";
				char buffer[1000];
				memset(buffer,0,sizeof(buffer));
				recv(conn,buffer,sizeof(buffer),0);
				string recv_str(buffer);
				if(recv_str.substr(0,2)=="ok")
				{
					if_login=true;
					login_name = name;
					cout<<"successfully login\n";
					break;
				}
				else
				{
					cout<<"password or name error\n\n";
				}
			}
		}
		if(choice==2)
		{
			cout<<"the name: ";
			cin>>name;
			while(1)
			{
				cout<<"password: ";
				cin>>pass;
				cout<<"confrim password: ";
				cin>>pass1;
				if(pass==pass1)
				{
					break;
				}
				else
				{
					cout<<"the password is not same\n";
				}
			}	
			name = "name:"+name;
			pass = "pass:"+pass;
			string str = name + pass;
			send(conn,str.c_str(),str.length(),0);
			cout<<"register success\n";
			cout<<"please input your choice:";
		}
	if(if_login)
	{
		system("clear");
		cout<<"           welcome back,"<<login_name<<"\n";
		cout<<"|-----------------------------------|\n";
		cout<<"\n";
		cout<<"|        please choose your option  |\n";
		cout<<"|             0 : exit              |\n";	
		cout<<"|    1 : initiate a separate chat   |\n";
		cout<<"|      2 : initiate a group chat    |\n";
		cout<<"|-----------------------------------|\n\n";
	}
	}
}


void client::SendMsg(int conn)
{
	
	//define send buff
	string input;
	//send message
	while(1)
	{
		//input message
		getline(cin,input);
		//send the input
		send(conn,input.c_str(),input.size(),0);
		cout<<"sended message: "<<input<<"\n";
		//exit fiction
		if(input == "exit")
			break;
	}
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
		cout<<"server: "<<buffer<<endl;
	}
}
 

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
	
	ifstream f("cookie.txt");
	string cookie_str;
	if(f.good())
	{
		f>>cookie_str;
		f.close();
		cookie_str="cookie:"+cookie_str;
		send(sock,cookie_str.c_str(),cookie_str.length()+1,0);
		char cookie_ans[100];
		memset(cookie_ans,0,sizeof(cookie_ans));
		recv(sock,cookie_ans,sizeof(cookie_ans),0);
		string ans_str(cookie_ans);
		if(ans_str!="NULL")
		{
			if_login=true;
			login_name=ans_str;
		}
	}
	if(!if_login)
	{
 
		cout<<"----------------\n";
		cout<<"input the choice\n";
		cout<<"     0:exit     \n";
		cout<<"    1:log in     \n";
		cout<<"   2:register   \n";
		cout<<"----------------\n\n";
	}
	while(1)
	{
		if(if_login)
		{
			break;
		}
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
					string tmpstr = recv_str.substr(2);
					tmpstr="cat > cookie.txt << end \n"+tmpstr+"/nend";
					system(tmpstr.c_str());
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
	}
	while(if_login&&1)
	{

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
		cin>>choice;
		if(choice==0)
		{
			break;
		}
		else if(choice==1)
		{
			cout<<"please input the peer name\n";
			string target_name,content;
			cin>>target_name;
			string sendstr("target:"+target_name+"from:"+login_name);
			send(conn,sendstr.c_str(),sendstr.length(),0);
			cout<<"please input what you want to say\n";
			cout<<"input 'exit' to exit"<<endl;
			thread t1(client::SendMsg,conn);
			thread t2(client::RecvMsg,conn);
			t1.join();
			t2.join();
		}
		else if(choice==2)
		{
			cout<<"please input the group number: ";
			int num;
			cin>>num;
			string sendstr("group:"+to_string(num));
			send(conn,sendstr.c_str(),sendstr.length(),0);
			cout<<"please input your speak(\"exit\" to exit)"<<endl;
			thread t1(client::SendMsg,-conn);
			thread t2(client::RecvMsg,conn);
			t1.join();
			t2.join();
		}
	}
	close(sock);
}
	



void client::SendMsg(int conn)
{
	
	//define send buffi
	cin.ignore();
	string input;
	//send message
	while(1)
	{
		//input message
		getline(cin,input);
		if(conn>0)
		{
			input = "content:"+input;
		}
		else if(conn<0)
		{
			input = "gr_message:"+input;
		}
		//send the input
		int ret = send(abs(conn),input.c_str(),input.size(),0);
		cout<<"sended message: "<<input<<"\n";
		//exit fiction
		if(input == "content:exit"||ret<=0)
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
		cout<<buffer<<endl;
	}
}
 

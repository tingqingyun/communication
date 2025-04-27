#include "server.h"
using namespace std;

//use vector to save all socket, 1 means this socket have connected.
vector<bool> server::sock_arr(10000,false);
server::server(int port,string ip):server_port(port),server_ip(ip){}

//close all connect when server close
server::~server(){
	for(int i = 0;i<sock_arr.size();i++)
	{
		cout<<"client"<<sock_arr[i]<<"disconnect\n";
		if(sock_arr[i])
		{
			close(i);
		}
	}
	close(server_sockfd);
}


void server::run()
{
	//server_sockfd is taojiezi,include ip-version, protocol
	//AD_INET is ipv4, SOCK_STREAM is tcp
	server_sockfd = socket(AF_INET,SOCK_STREAM,0);
	// sockaddr_in is ipv4 struct
	struct sockaddr_in server_sockaddr;
	//sin_family is protocol
	server_sockaddr.sin_family = AF_INET;
	//sin_port is port
	server_sockaddr.sin_port = htons(server_port);
	//sin_addr.s_addr is ip address
	server_sockaddr.sin_addr.s_addr = inet_addr(server_ip.c_str());
	//bind the socket and server
	if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==1)
	{
	//output error reason
		perror("bind");
		exit(1);
	}
	//listen the taojiezi ,20 is the max number of client
	if(listen(server_sockfd,20)== -1)
	{
	//output error reason
		perror("listen");
		exit(1);
	}
	//client addr struct
	struct sockaddr_in client_addr;
	//length of client_address
	socklen_t length = sizeof(client_addr);
	while(1)
	{

		//accept can return the taojiezi that successfully connect
		int conn = accept(server_sockfd,(struct sockaddr*)&client_addr,&length);
		if(conn<0)
		{
			//output error reason
			perror("connect");
			exit(1);
		}
		//output which client
		cout<<"taojiezi "<<conn<<" client successfully connect\n";
		//push the client into sock_arr(all taojiezi)
		sock_arr.push_back(conn);
		//create thread deal the corresspond client
		//header is <thread>
		std::thread t(server::RecvMsg,conn);
		//with detach, the server will not jam up
		t.detach();
	}
}
void server::RecvMsg(int conn)
{	
	//buffer(accept input)
	char buffer[1000];
	while(1)
	{
	//accept the data that client send
		memset(buffer,0,sizeof(buffer));
		int len = recv(conn, buffer, 1000,0);
	//if client input exit or error ,exit
		
		cout<<"receicv taojiezi"<<conn<<":"<<buffer<<"\n";
		if(strcmp(buffer,"exit")==0 || len<=0)
		{
			//close corresponding conn
			close(conn);
			//reset
			sock_arr[conn] = false;
			break;
		}
		//change the buffer whih string
		string str(buffer);
		cout<<"recv_str: "<<str<<"\n";
		//send the string to sql database		
		HandleRequest(conn,str);
	}
}	 

void server::HandleRequest(int conn,string str)
{
	char buffer[1000];
	//define the database value
	string name,pass;
	//log whether the user successfully logined
	bool if_login=false;
	//log the user name
	string login_name;
	//log the name of user who you wanna talk
	string targer_name;
	//log the group id
	int group_num;

	//connect the sql database
	//init the mysql object ,header is <mysql/mysql.h>
	MYSQL *con = mysql_init(NULL);
	//connect the sql database;con is the mysql object which your init,
	//"127.0.0.1"is server ip, root is the server name,""is password
	//chatproject is the database name;
	// 0 is default port,means use 3306 port
	// NULL indicate the connection use tcp/ip
	// CLIENT_MULTI_STATTEMENTS allow client to execute multi_command
	if(mysql_real_connect(con,"127.0.0.1","root","","communication",0,NULL,CLIENT_MULTI_STATEMENTS)==nullptr)
	{
		cout<<"database connect failed:"<<mysql_error(con);
		mysql_close(con);
		return ;
	}
	//str.npos indicate didn't find,str.find(str1) indicate find str1 in str;	
	if(str.find("name:")!=str.npos)
	{
		//str.find(str1)
		//str.find return the number that first find str1
		int p1=str.find("name:"),p2=str.find("pass:");
		//str = name:qingyunpass:123
		//str.substr(str1,length)
		//str.substr fiction is extract a length of string from str1
		name = str.substr(p1+5,p2-5);
		pass = str.substr(p2+5,str.length()-p2-5);
		//insert into table
		//values();
		string search = "insert into user values(\"";
		search+=name;
		search+="\",\"";
		search+=pass;
		search+="\");";
		//endl fiction is press \n
		cout<<"sql command:"<<search<<endl<<endl;
		//use the command
		mysql_query(con,search.c_str());
	}
	else if(str.find("login")!=str.npos)
	{
		cout<<"test point 1\n";
		int p1 = str.find("login"),p2 = str.find("pass:");
		name = str.substr(p1+5,p2-5);
		pass = str.substr(p2+5,str.length()-p2-5);
		string search="select * from user where name = \"";
		search+=name;
		search+="\"";
		cout<<"sql commond:"<<search<<endl;
		auto search_res = mysql_query(con,search.c_str());
		auto result = mysql_store_result(con);
		int col = mysql_num_fields(result);
		int row = mysql_num_rows(result);
		if(search_res==0&&row!=0)
		{
			cout<<"successfully find\n";
			auto info = mysql_fetch_row(result);
			cout<<"find name:"<<info[0]<<"password:"<<info[1]<<endl;
			if(info[1]==pass)
			{
				cout<<"password correct\n";
				string str1 = "ok";
				if_login = true;
				login_name = name;
				send(conn,str1.c_str(),str1.length()+1,0);
			}
			else
			{
				cout<<"password error\n";
				char str1[100] = "wrong";
				send(conn,str1,strlen(str1),0);
			}
		}
		else
		{
			cout<<"select error\n";
			char str1[100] = "wrong";
			send(conn,str1,strlen(str1),0);
		}
	}
}

#include "server.h"
using namespace std;

vector<bool> server::sock_arr(10000,false);
server::server(int port,string ip):server_port(port),server_ip(ip){}

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
	//bind the sockfd with ip and port
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
	}
}	 

void server::HandleRequest(int conn,string str)
{
	char buffer[1000];
	string name,pass;
	bool if_login=false;
	string login_name;
	string targer_name;
	int group_num;
	MYSQL *con = musql_init(NULL);
	mysql_real_connect(con,"127.0.0.1","root","","ChatProject",0,NULL,CLIENT_MULTI_STATEMENTS);
	if(str.find("name:")!=str.npos)
	{
		int p1=str.find("name:"),p2=str.find("pass:");
		name = str.substr(p1+5,p2-5);
		pass = str.substr(p1+5,str.length()-p2-4);
		string search = "INSERT INTO USER VALUES(\"";
		search+=name;
		search+="\",\"";
		search+=pass;
		search+="\",\"";
		cout<<"sql command:"<<search<<endl<<endl;
		mysql_query(con,search.c_str());
	}
}

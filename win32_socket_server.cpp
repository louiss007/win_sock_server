#include<Winsock2.h>
#include<stdio.h>
#pragma comment(lib,"ws2_32.lib")

void main()
{
	WSADATA wsaData;
	SOCKET sockSrv;
	SOCKADDR_IN addrSrv;
	SOCKADDR_IN addrClient;	//recieve the address info of client
	int len=sizeof(SOCKADDR);
	WORD wVersionRequested;	//store the version number of WinSock Lib
	int err;

	wVersionRequested=MAKEWORD(2,2);	//创建一个包含了请求版本号的WORD值

	err=WSAStartup(wVersionRequested,&wsaData);	//load the socket lib
	if(err!=0)	//返回值不等于0，程序退出
		return ;
	if(LOBYTE(wsaData.wVersion)!=2||
		HIBYTE(wsaData.wVersion)!=2)	//判断低字节和高字节是否都等于1
	{
		WSACleanup();	//不等于1，则调用该函数，终止对Winsock库的使用，并返回
		return ;
	}

	sockSrv=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);	//创建套接字

	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(6000);
	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));	//将套接字绑定到本地址和设定的端口号上
	listen(sockSrv,5);

	while(1)	//作为服务器，需要不断地等待客户端的连接请求的到来，所以设置为死循环
	{
		char recvBuf[100];
		char sendBuf[100];	//当前这个新连接的一个套接字描述符，保持于sockConn变量中，利用这个套接字就可以与客户端通信。
		                    //先前的套接字仍然继续监听客户端的连接请求

		//返回连接状态的套接字
		SOCKET sockConn=accept(sockSrv,(SOCKADDR*)&addrClient,&len);	//当客户端连接请求到来时，该函数接受该请求，建立连接，同时返回一个新的套接字描述符

		sprintf_s(sendBuf,"Welcome %s you",inet_ntoa(addrClient.sin_addr));
		send(sockConn,sendBuf,strlen(sendBuf)+1,0);	//用返回的套接字和客户端进行通信
		recv(sockConn,recvBuf,100,0);	//从客户端接受数据
		printf("%s\n",recvBuf);
		closesocket(sockConn);	//当前通信完成之后，需要调用closesocket()函数关闭已建立的套接字，释放为该套接字分配的资源，
								//然后进入下一个循环，等待另一个客户请求的到来；若不是死循环，此处还需调用WSACleanup()函数
	}							//终止对套接字库的使用
}



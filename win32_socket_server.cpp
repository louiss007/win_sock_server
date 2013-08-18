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

	wVersionRequested=MAKEWORD(2,2);	//����һ������������汾�ŵ�WORDֵ

	err=WSAStartup(wVersionRequested,&wsaData);	//load the socket lib
	if(err!=0)	//����ֵ������0�������˳�
		return ;
	if(LOBYTE(wsaData.wVersion)!=2||
		HIBYTE(wsaData.wVersion)!=2)	//�жϵ��ֽں͸��ֽ��Ƿ񶼵���1
	{
		WSACleanup();	//������1������øú�������ֹ��Winsock���ʹ�ã�������
		return ;
	}

	sockSrv=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);	//�����׽���

	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(6000);
	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));	//���׽��ְ󶨵�����ַ���趨�Ķ˿ں���
	listen(sockSrv,5);

	while(1)	//��Ϊ����������Ҫ���ϵصȴ��ͻ��˵���������ĵ�������������Ϊ��ѭ��
	{
		char recvBuf[100];
		char sendBuf[100];	//��ǰ��������ӵ�һ���׽�����������������sockConn�����У���������׽��־Ϳ�����ͻ���ͨ�š�
		                    //��ǰ���׽�����Ȼ���������ͻ��˵���������

		//��������״̬���׽���
		SOCKET sockConn=accept(sockSrv,(SOCKADDR*)&addrClient,&len);	//���ͻ�������������ʱ���ú������ܸ����󣬽������ӣ�ͬʱ����һ���µ��׽���������

		sprintf_s(sendBuf,"Welcome %s you",inet_ntoa(addrClient.sin_addr));
		send(sockConn,sendBuf,strlen(sendBuf)+1,0);	//�÷��ص��׽��ֺͿͻ��˽���ͨ��
		recv(sockConn,recvBuf,100,0);	//�ӿͻ��˽�������
		printf("%s\n",recvBuf);
		closesocket(sockConn);	//��ǰͨ�����֮����Ҫ����closesocket()�����ر��ѽ������׽��֣��ͷ�Ϊ���׽��ַ������Դ��
								//Ȼ�������һ��ѭ�����ȴ���һ���ͻ�����ĵ�������������ѭ�����˴��������WSACleanup()����
	}							//��ֹ���׽��ֿ��ʹ��
}



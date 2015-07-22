#include <iostream>  
#include <cstdio>  
#include <Winsock2.h>  
#include <fstream>
#include <windows.h>

#pragma comment(lib,"ws2_32.lib")
      
using namespace std;  

char img_size_char[10];
char buf[1024 * 1024 * 3];

const char LocalIP[] = "192.168.191.2";
      
SOCKET socket_get_image(char *img)  
{  
    WORD wVersionRequested;  
    WSADATA wsaData;  
    int err;  
           
    wVersionRequested = MAKEWORD( 2, 2 ); 
           
    err = WSAStartup( wVersionRequested, &wsaData );  
    if ( err != 0 ) {  
        return -1;         
    }  
           
    if ( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 ) {  
        WSACleanup( );  
        return -1;   
    }  

    SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);     
      
    SOCKADDR_IN addrSrv;  
	addrSrv.sin_addr.S_un.S_addr = inet_addr(LocalIP);
    //addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    addrSrv.sin_family = AF_INET;  
    addrSrv.sin_port = htons(4567);  
      
    bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)); // �ڶ�����Ҫǿ������ת��  
      
    listen(sockSrv, 10);  
           
    SOCKADDR_IN  addrClient;  
    int len = sizeof(SOCKADDR);  
    
	//��������ֻ�����β��ԣ����Բ�û�м�whileѭ��
	printf("���ڵȴ�Android�豸����......\n");
    SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);
	printf("��һ̨Android�豸���ϣ����ڽ���ͼƬ......\n");
    struct sockaddr_in clientAddress;//�����Ϳͻ���ͨ�ŵ��׽��ֵ�ַ
	memset(&clientAddress,0,sizeof(clientAddress));


	//���Ƚ���img��С
	memset(img_size_char, 0, sizeof(img_size_char));
	int bytes;
	char size_str;
	int size_str_len = 0;
	//ÿ�ζ��ӻ����������ȡһ���ֽڵĴ�С��һֱ������\n��ʾ����
	while(1)
	{
		if((bytes=recv(sockConn, &size_str,sizeof(char),0)) == SOCKET_ERROR){
			printf("����ͼƬ��Сʧ��!\n");
			exit(-1);
		}
		else
		{
			if(size_str != '\n')
				img_size_char[size_str_len++] = size_str;
			else
			{
				img_size_char[size_str_len] = 0;
				break;
			}
		}
	}

	//��������������img
	int Length = atoi(img_size_char);

	memset(buf, 0, sizeof(buf));
	int nRecvLen =0; 
	while ( nRecvLen < Length ) 
	{ 
		int len;
		if(Length - nRecvLen >= 1024)
			len = recv(sockConn, buf + nRecvLen, 1024, 0);
		else
		{
			len = recv(sockConn, buf + nRecvLen, Length - nRecvLen, 0);
			nRecvLen += len;
			break;
		}
		nRecvLen += len;
	} 

	printf("Message from %s\n�����Ϣ�Ѿ����浽�ļ�����\n",inet_ntoa(clientAddress.sin_addr));
	//if(send(sockConn,buf,bytes,0)==SOCKET_ERROR){

	//�����ĵ��������õ��ļ�
	ofstream f1(img, ios::binary);
	f1.write(buf, Length);
	f1.close();

	//return the result
	//char result[] = "The species leaf belongs to is Wutong";
	//send(sockConn, result, strlen(result), 0);
			
	//closesocket(sockConn); 
	//closesocket(sockSrv);
      
    return sockConn;  
}  
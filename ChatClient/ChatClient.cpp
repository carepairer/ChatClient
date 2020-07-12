#include <iostream>
using namespace std;
//包含通信的头文件和库
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

int main()
{
	//1，初始化网络库
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaData)) {
		cout << "初始化失败" << endl;
		return 0;
	}

	//2.创建socket套接字
	SOCKET s = socket(
		AF_INET, //INET协议族
		SOCK_STREAM, //表示使用的是TCP协议
		0
	);
	if (s == INVALID_SOCKET) {
		cout << "创建socket失败！" << endl;
		return 0;
	}

	//4. connect 连接服务器
	sockaddr_in addr;
	addr.sin_family = AF_INET; //协议族
	inet_pton(PF_INET, "127.0.0.1", &addr.sin_addr);//服务端IP
	addr.sin_port = htons(10086); //网络字节序（大尾方式）， 本地字节序（小尾方式）， 需要转换
	int nRet = connect(s, (sockaddr*)&addr, sizeof(addr));
	if (SOCKET_ERROR == nRet) {
		cout << "连接失败" << endl;
		return 0;
	}
	else {
		cout << "连接成功，等待服务器第一次发送信息...." << endl;
	}

	//5. send/recv 发送/接受消息
	while (true) {
		char szBuf[256] = { 0 };
		recv(s, szBuf, 256, 0);
		cout << "服务器" << szBuf << endl;

		cout << "请您输入需要发送的数据：";
		char sendMsg[256] = { 0 };
		cin.getline(sendMsg, 256);
		send(s, sendMsg, strlen(sendMsg) + 1, 0);
		cout << "数据发送成功，等待对面回复。。。。" << endl;
	}

	//6.关闭socket  
	closesocket(s);

	//7.反初始化操作
	WSACleanup();

	return 1;
}
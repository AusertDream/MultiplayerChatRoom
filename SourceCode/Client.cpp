#pragma once
#include "framework.h"
#include "InitWindows.h"
#include "Client.h"
using namespace std;
typedef long long ll;


//客户端文件



extern void StartClient() {
	//1.确定协议版本
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		MessageBox(hMainWindow, "通信版本非2.2", "Error", MB_OK);
		//清理版本信息
		WSACleanup();
		return;
	}
	//2.创建socket
	//采用流传输模式，TCP协议。AF_INET表示使用的传输协议为TCP,UDP之类的协议。
	SOCKET sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == sSocket) {
		//如果创建失败输出报错信息

		MessageBox(hMainWindow, "创建socket失败", "Error", MB_OK);
		return;
	}
	//3.确定服务器的协议地址簇
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET; //使用的是什么协议与创建SOCKET时候写的须保持一致
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); //服务器的ip地址
	addr.sin_port = htons(10086); //服务器提供服务的端口号

	//4. 连接服务器
	int r = connect(sSocket, (sockaddr*)&addr, sizeof addr);
	if (r == -1) {
		//如果连接失败输出报错信息
		closesocket(sSocket); //断开链接
		WSACleanup(); //清理协议版本信息
		MessageBox(hMainWindow, "连接失败", "Error", MB_OK);
		return;
	}

	//5.通信
	string buff = "114514";
	while(true) {
		send(sSocket, buff.c_str(), buff.size(), NULL);
	}
}
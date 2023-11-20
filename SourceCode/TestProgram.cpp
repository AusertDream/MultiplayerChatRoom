#pragma once
#include "framework.h"
#include "SomeTools.h"
#include "TestProgram.h"
using namespace std;
//用于进行多用户的测试文件


//执行测试操作
void Test() {
	//1.确定协议版本
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		MessageBox(NULL, "通信版本非2.2", "Error", MB_OK);
		//清理版本信息
		WSACleanup();
		return;
	}
	//2.创建socket
	//采用流传输模式，TCP协议。AF_INET表示使用的传输协议为TCP,UDP之类的协议。
	SOCKET sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == sSocket) {
		char* ErrorMsg = " ";
		//如果创建失败输出报错信息
		sprintf(ErrorMsg, "创建socket失败：%d\n", GetLastError());
		MessageBox(NULL, ErrorMsg, "Error", MB_OK);
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
		char* ErrorMsg = " ";
		//如果连接失败输出报错信息
		sprintf(ErrorMsg, "连接失败：%d\n", GetLastError());
		closesocket(sSocket); //断开链接
		WSACleanup(); //清理协议版本信息
		MessageBox(NULL, ErrorMsg, "Error", MB_OK);
		return;
	}

	//5.通信
	string buff = "114514";
	while (true) {
		send(sSocket, buff.c_str(), buff.size(), NULL);
	}
}

static void StartTestProgram(int MaxUserNumber) {
	vector<thread> AllTestUsers(MaxUserNumber);
	for (int i = 0; i < MaxUserNumber; i++) {
		AllTestUsers[i] = thread(Test);
		AllTestUsers[i].detach();
	}
}

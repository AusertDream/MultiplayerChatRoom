#include "framework.h"
#include "InitWindows.h"
using namespace std;
typedef long long ll;
//服务端文件






static void StartServer() {

	//1.确定协议版本
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (LOBYTE(wsaData.wVersion) != 2||HIBYTE(wsaData.wVersion)!=2) {
		MessageBox(hMainWindow, "通信版本非2.2", "Error", MB_OK);
		//清理版本信息
		WSACleanup();
		return;
	}
	//2.创建socket
	//采用流传输模式，TCP协议。AF_INET表示使用的传输协议为TCP,UDP之类的协议。
	SOCKET sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == sSocket) {
		char* ErrorMsg=" ";
		//如果创建失败输出报错信息
		sprintf(ErrorMsg, "创建socket失败：%d\n", GetLastError());
		MessageBox(hMainWindow, ErrorMsg, "Error", MB_OK);
		return;
	}
	//3.确定服务器的协议地址簇
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET; //使用的是什么协议与创建SOCKET时候写的须保持一致
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); //服务器的ip地址
	addr.sin_port = htons(10086); //服务器提供服务的端口号

	//4. 绑定
	int r = bind(sSocket, (sockaddr*)&addr, sizeof addr);
	if (r == -1) {
		char* ErrorMsg = " ";
		//如果绑定失败输出报错信息
		sprintf(ErrorMsg, "绑定失败：%d\n", GetLastError());
		closesocket(sSocket); //断开链接
		WSACleanup(); //清理协议版本信息
		MessageBox(hMainWindow, ErrorMsg, "Error", MB_OK);
		return;
	}

	//5.监听
	r = listen(sSocket, 10);
	if (r == -1) {
		char* ErrorMsg = " ";
		//如果监听失败输出报错信息
		sprintf(ErrorMsg, "监听失败：%d\n", GetLastError());
		closesocket(sSocket); //断开链接
		WSACleanup(); //清理协议版本信息
		MessageBox(hMainWindow, ErrorMsg, "Error", MB_OK);
		return;
	}

	//6.等待客户端链接
	SOCKET cSocket = accept(sSocket, NULL, NULL);
	if (cSocket == SOCKET_ERROR) {
		char* ErrorMsg = " ";
		//如果等待失败说明服务器崩溃输出报错信息
		sprintf(ErrorMsg, "服务器崩溃：%d\n", GetLastError());
		closesocket(sSocket); //断开链接
		WSACleanup(); //清理协议版本信息
		MessageBox(hMainWindow, ErrorMsg, "Error", MB_OK);
		return;
	}

	//7.通信
	char buff[256] = { 0 };
	while (true) {
		r = recv(cSocket, buff, 255, NULL);
		if (r > 0) {
			buff[r] = 0;
			WriteConsole(hStdOutput, buff, strlen(buff), NULL, NULL);
		}
	}

	while (true) {

	}
}
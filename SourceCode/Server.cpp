
#include "framework.h"
#include "InitWindows.h"
#include "SomeTools.h"
#include "Server.h"
using namespace std;
typedef long long ll;
#define MAX_NUMBER  101


//存放当前在线用户
set<wstring> UserList;
//存放当前所有线程
vector<thread> allThread;
//存放当前所有socket链接
vector<SOCKET> allSocket;
//将线程ID单一映射到用户姓名上
map<thread::id, wstring> ID_UserName;

bool isStart = false; //用来表示服务端是否启动

mutex ProtectUserList; //用来保护UserList的互斥锁


//服务端文件

SOCKET InitServer() {
	allThread.resize(MAX_NUMBER);
	allSocket.resize(MAX_NUMBER);
	//1.确定协议版本
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		MessageBox(hMainWindow, L"通信版本非2.2", L"Error", MB_OK);
		//清理版本信息
		WSACleanup();
	}
	//2.创建socket
	//采用流传输模式，TCP协议。AF_INET表示使用的传输协议为TCP,UDP之类的协议。
	SOCKET sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == sSocket) {
		//如果创建失败输出报错信息
		
		MessageBox(hMainWindow, L"创建socket失败", L"Error", MB_OK);
	}
	//3.确定服务器的协议地址簇
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET; //使用的是什么协议与创建SOCKET时候写的须保持一致
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); //服务器的ip地址
	addr.sin_port = htons(10086); //服务器提供服务的端口号

	//4. 绑定
	int r = bind(sSocket, (sockaddr*)&addr, sizeof addr);
	if (r == -1) {
		
		//如果绑定失败输出报错信息
		closesocket(sSocket); //断开链接
		WSACleanup(); //清理协议版本信息
		MessageBox(hMainWindow, L"绑定失败", L"Error", MB_OK);
	}

	//5.监听
	r = listen(sSocket, 10);
	if (r == -1) {
		
		//如果监听失败输出报错信息
		
		closesocket(sSocket); //断开链接
		WSACleanup(); //清理协议版本信息
		MessageBox(hMainWindow, L"监听失败", L"Error", MB_OK);
	}
	return sSocket;
}





void OnSend(SOCKET cSocket, UserParam receivedData) {
	//创建发送的数据，并且填写信息
	UserParam SendData;
	SendData.Type = NORMAL_MSG;
	SendData.Msg = receivedData.Msg;
	SendData.Sender = L"Server";
	SendData.Receiver = L"ALL";
	//将结构体序列化为字符数组，然后使用send发送出去
	char buffer[sizeof(UserParam)];
	serialize(SendData, buffer);
	//发送给所有客户端
	for (int i = 0; i < MAX_NUMBER; i++) {
		send(allSocket[i], buffer, sizeof buffer, NULL);
	}
}

void OnLogIn(SOCKET cSocket, UserParam receivedData) {
	ProtectUserList.lock();
	//如果已经存在该用户则返回false
	if (UserList.find(receivedData.Sender) != UserList.end()) {
		UserParam res;
		res.Type = LOGIN_RESULT;
		res.Sender = L"Server";
		res.res = false;
		char buff[sizeof(res)];
		serialize(res, buff);
		send(cSocket, buff, sizeof buff, NULL);
		ProtectUserList.unlock();
		return;
	}
	//反之，则将用户名字加入到在线用户列表中
	else {
		UserList.insert(receivedData.Sender);
		ID_UserName[this_thread::get_id()] = receivedData.Sender;
		UserParam res;
		res.Type = LOGIN_RESULT;
		res.res = true;
		res.Sender = L"Server";
		char buff[sizeof(res)];
		serialize(res, buff);
		send(cSocket, buff, sizeof buff, NULL);
		ProtectUserList.unlock();
		return;
	}
}

void OnLogOut(SOCKET cSocket, UserParam receivedData) {
	UserList.erase(receivedData.Sender);
	ID_UserName[this_thread::get_id()] = L" ";
}

void SendRecvProc(SOCKET cSocket) {
	
	int ret = 0;
	//7.通信
	char recvBuff[sizeof(UserParam)];
	while (true) {
		//接收消息
		ret = recv(cSocket, recvBuff, sizeof(UserParam), NULL);
		if (ret <= 0) {
			//如果消息接收异常
			closesocket(cSocket); //断开链接
			WSACleanup(); //清理协议版本信息
		}
		
		
		WriteConsole(hStdOutput, L"recvBuff", sizeof("recvBuff"), NULL, NULL);
		// 反序列化接收到的数据
		UserParam receivedData;
		deserialize(recvBuff, receivedData);
		//如果接收到的数据类型不是登录请求或者登出请求或者普通消息则不处理
		if (receivedData.Type != LOGIN_REQUEST && receivedData.Type != LOGOUT_REQUEST&& receivedData.Type!= NORMAL_MSG&&receivedData.Type!= LOGIN_RESULT) {
			continue;
		}
		//根据数据类型来处理消息内容
		switch (receivedData.Type) {
		case NORMAL_MSG:
			OnSend(cSocket, receivedData);
			break;
		case LOGIN_REQUEST:
			OnLogIn(cSocket,receivedData);
			break;
		case LOGOUT_REQUEST:
			OnLogOut(cSocket, receivedData);
			break;
		}
	}
}

void StartServer() {
	//初始化
	SOCKET sSocket = InitServer();
	UserList.insert(L"ALL");
	UserList.insert(L"Server");
	//创建多线程处理收发消息
	for (int i = 0; i < MAX_NUMBER; i++) {
		//6.等待客户端链接
		SOCKET cSocket = accept(sSocket, NULL, NULL);
		if (cSocket == SOCKET_ERROR) {
			//如果等待失败说明服务器崩溃输出报错信息
			closesocket(sSocket); //断开链接
			WSACleanup(); //清理协议版本信息
			MessageBox(hMainWindow, L"服务器崩溃", L"Error", MB_OK);
			return;
		}
		allSocket[i] = cSocket;
		allThread[i] = thread(SendRecvProc, cSocket);
		allThread[i].detach();
	}
}
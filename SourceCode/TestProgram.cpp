#pragma once
#include "framework.h"
#include "SomeTools.h"
#include "TestProgram.h"
using namespace std;
//用于进行多用户的测试文件
#define MAX_BUFFER_SIZE 1024
mutex ProtectLogOutUserNumber;//用于保护LogOutUserNumber的互斥锁
int LogOutUserNumber = 0;//退出的用户数量

//执行测试操作
void User() {
	//2.创建socket
	//采用流传输模式，TCP协议。AF_INET表示使用的传输协议为TCP,UDP之类的协议。
	SOCKET sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == sSocket) {
		//如果创建失败直接退出
		ProtectLogOutUserNumber.lock();
		LogOutUserNumber++;
		ProtectLogOutUserNumber.unlock();
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
		//如果连接失败直接退出
		closesocket(sSocket); //断开链接
		ProtectLogOutUserNumber.lock();
		LogOutUserNumber++;
		ProtectLogOutUserNumber.unlock();
		return;
	}
	//随机产生用户名
	string UserName;
	int UserNameLength = MyRandom(1, 10);
	UserName.resize(UserNameLength);
	for (int i = 0; i < UserNameLength; i++) {
		UserName[i] = MyRandom('a', 'z');
	}
	//尝试登录
	string LoginBuffer;
	LoginBuffer.push_back(LOGIN_REQUEST);
	LoginBuffer.push_back(' ');
	LoginBuffer.append(UserName);
	LoginBuffer.push_back(' ');
	LoginBuffer.append("Server");
	LoginBuffer.push_back(' ');
	LoginBuffer.append("loginrequest");
	LoginBuffer.push_back(' ');
	LoginBuffer.push_back('\0');
	//发送登录请求
	send(sSocket,LoginBuffer.c_str(), LoginBuffer.size(), 0);
	//接收登录结果
	char LoginResultBuffer[MAX_BUFFER_SIZE];
	recv(sSocket, LoginResultBuffer, MAX_BUFFER_SIZE, 0);
	USES_CONVERSION;
	wstring wLoginResultBuffer = A2W(LoginResultBuffer);
	UserParam LoginResult;
	//解包
	openTheBuff(wLoginResultBuffer, LoginResult);
	if (LoginResult.res == false) {
		//登录失败，直接退出程序
		closesocket(sSocket); //断开链接
		ProtectLogOutUserNumber.lock();
		LogOutUserNumber++;
		ProtectLogOutUserNumber.unlock();
		return;
	}
	else {
		//登录成功，开始测试
		//测试仅测试向多人聊天室发送消息，同时不支持私信的回应。
		while (true) {
			//每次开始前进行一次判定，是否要退出聊天室
			if (MyRandom(1, 100) <= 10) {
				break;
			}
			//随机选择一个消息接收者，两个聊天室的选择对半开概率
			string MsgReceiver;
			if (MyRandom(1, 100) <= 50) {
				MsgReceiver = "GLOBAL";
			}
			else {
				MsgReceiver = "CN";
			}
			//产生一个信息字母表，然后根据字母表产生随机消息
			vector<char> alphabet;
			for (int i = 0; i < 26; i++) {
				alphabet.push_back(char('a' + i));
			}
			for (int i = 0; i < 26; i++) {
				alphabet.push_back(char('A' + i));
			}
			for (int i = 0; i < 10; i++) {
				alphabet.push_back(char('0' + i));
			}
			string MsgContent;
			int MsgContentLen = MyRandom(1, 25);
			MsgContent.resize(MsgContentLen);
			for (int i = 0; i < MsgContentLen; i++) {
				int idx = MyRandom(0, (int)alphabet.size() - 1);
				MsgContent[i] = alphabet[idx];
			}
			string sendMsgBuffer;
			sendMsgBuffer.push_back(NORMAL_MSG);
			sendMsgBuffer.push_back(' ');
			sendMsgBuffer.append(UserName);
			sendMsgBuffer.push_back(' ');
			sendMsgBuffer.append(MsgReceiver);
			sendMsgBuffer.push_back(' ');
			sendMsgBuffer.append(MsgContent);
			sendMsgBuffer.push_back(' ');
			sendMsgBuffer.push_back('\0');
			send(sSocket, sendMsgBuffer.c_str(), sendMsgBuffer.size(), 0);
			//sleep一个随机的时间
			Sleep(MyRandom(800, 5000));
		}
		//发送登出请求
		string LogoutBuffer;
		LogoutBuffer.push_back(LOGOUT_REQUEST);
		LogoutBuffer.push_back(' ');
		LogoutBuffer.append(UserName);
		LogoutBuffer.push_back(' ');
		LogoutBuffer.append("Server");
		LogoutBuffer.push_back(' ');
		LogoutBuffer.append("logoutrequest");
		LogoutBuffer.push_back(' ');
		LogoutBuffer.push_back('\0');
		send(sSocket, LogoutBuffer.c_str(), LogoutBuffer.size(), 0);
		closesocket(sSocket); //断开链接
		ProtectLogOutUserNumber.lock();
		LogOutUserNumber++;
		ProtectLogOutUserNumber.unlock();
	}
}

void StartTestProgram(int MaxUserNumber) {
	//1.确定协议版本
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		MessageBox(NULL, "通信版本非2.2", "Error", MB_OK);
		//清理版本信息
		WSACleanup();
		return;
	}
	vector<thread> UserThreads(MaxUserNumber);
	for (int i = 0; i < MaxUserNumber; i++) {
		UserThreads[i] = thread(User);
		UserThreads[i].detach();
	}
	//等待所有用户退出
	while (true) {
		ProtectLogOutUserNumber.lock();
		if (LogOutUserNumber == MaxUserNumber) {
			ProtectLogOutUserNumber.unlock();
			break;
		}
		else {
			ProtectLogOutUserNumber.unlock();
			Sleep(1000);
		}
	}
	WSACleanup();
	return;
}

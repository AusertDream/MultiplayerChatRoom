
#include "framework.h"
#include "SomeTools.h"
using namespace std;
typedef long long ll;
#define MAX_NUMBER  101

#define MAX_NUMBER  101
#define MAX_BUFFER_SIZE 1024

//初始化网络
 SOCKET InitServer();
 SOCKET Initsocket;
//处理收发的函数
 void SendRecvProc();
//发送函数
 void OnSend(SOCKET, UserParam);
//处理登录函数
 void OnLogIn(SOCKET, UserParam);
//处理登出函数
 void OnLogOut(SOCKET, UserParam);
//处理发送聊天记录函数
void OnSendChatRecord(SOCKET, UserParam);
//处理发送用户列表函数
void OnSendUserList(SOCKET, UserParam);
//存放当前在线用户
set<wstring> UserList;
//存放当前所有线程
vector<thread> allThread;
//存放当前所有socket链接
vector<SOCKET> allSocket;
//将线程ID单一映射到用户姓名上
map<thread::id, wstring> ID_UserName;

//群聊的聊天记录
vector<UserMessage> ChatRecordGlobal;
vector<UserMessage> ChatRecordCN;

bool isStart = false; //用来表示服务端是否启动

mutex ProtectUserList; //用来保护UserList的互斥锁

//传输文件格式为  消息类型 发送者 接受者 消息内容 消息根据空格分开

//服务端文件
int main() {
	//初始化
	Initsocket = InitServer();
	UserList.insert(L"GLOBAL");
	UserList.insert(L"CN");
	UserList.insert(L"Server");
	cout<<"启动成功"<<endl;
	//创建多线程处理收发消息
	for (int i = 0; i < MAX_NUMBER; i++) {
		allThread[i] = thread(SendRecvProc);
		allThread[i].detach();
	}
	cout << "输入finish结束服务器主进程" << endl;
	string s;
	while (true) {
		cin >> s;
		if (s == "finish") {
			break;
		}
		else if (s == "showuserlist") {
			ProtectUserList.lock();
			cout << "当前用户总人数：" << UserList.size() << endl;
			cout << "当前在线用户有：" << endl;
			for (auto e : UserList) {
				USES_CONVERSION;
				cout<<string(W2A(e.c_str()))<<endl;
			}
			ProtectUserList.unlock();
		}
		else {
			cout << "未知指令" << endl;
		}
	}
	WSACleanup();
	return 0;
}


SOCKET InitServer() {
	allThread.resize(MAX_NUMBER);
	//1.确定协议版本
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		cout<<"协议版本不是2.2"<<endl;
		//清理版本信息
		return NULL;
	}
	//2.创建socket
	//采用流传输模式，TCP协议。AF_INET表示使用的传输协议为TCP,UDP之类的协议。
	SOCKET sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == sSocket) {
		//如果创建失败输出报错信息
		cout<<"创建socket失败"<<endl;
		return NULL;
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
		cout<<"绑定失败"<<endl;
		return NULL;
	}
	//5.监听
	r = listen(sSocket, 10);
	if (r == -1) {
		//如果监听失败输出报错信息
		closesocket(sSocket); //断开链接
		cout<<"监听失败"<<endl;
		return NULL;
	}
	return sSocket;
}





void OnSend(SOCKET cSocket, UserParam receivedData) {
	//创建发送的数据，并且填写信息
	UserParam SendData;
	SendData.Type = NORMAL_MSG;
	SendData.Msg = receivedData.Msg;
	SendData.Sender = receivedData.Sender;
	SendData.Receiver = receivedData.Receiver;
	//将结构体化为字符数组，然后使用send发送出去
	wstring buffer;
	buffer.push_back(SendData.Type);
	buffer.push_back(L' ');
	buffer.append(SendData.Sender);
	buffer.push_back(L' ');
	buffer.append(SendData.Receiver);
	buffer.push_back(L' ');
	buffer.append(SendData.Msg);
	buffer.push_back(L'\0');
	USES_CONVERSION;
	string s(W2A(buffer.c_str()));
	s.push_back('\0');
	//发送给所有客户端
	for (int i = 0; i < allSocket.size(); i++) {
		send(allSocket[i], s.c_str(), s.size(), NULL);
	}
}

void OnLogIn(SOCKET cSocket, UserParam receivedData) {
	ProtectUserList.lock();
	//如果已经存在该用户则返回false
	USES_CONVERSION;
	if (UserList.find(receivedData.Sender) != UserList.end()) {
		wstring buffer;
		buffer.push_back(LOGIN_RESULT);
		buffer.push_back(L' ');
		buffer.append(L"Server");
		buffer.push_back(L' ');
		buffer.push_back(L' ');
		buffer.push_back(L'false');
		string s(W2A(buffer.c_str()));
		s.push_back('\0');
		send(cSocket, s.c_str(), s.size(), NULL);
		ProtectUserList.unlock();
		return;
	}
	//反之，则将用户名字加入到在线用户列表中
	else {
		UserList.insert(receivedData.Sender);
		ID_UserName[this_thread::get_id()] = receivedData.Sender;
		wstring buffer;
		buffer.push_back(LOGIN_RESULT);
		buffer.push_back(L' ');
		buffer.append(L"Server");
		buffer.push_back(L' ');
		buffer.push_back(L' ');
		buffer.push_back(L'true');
		string s(W2A(buffer.c_str()));
		s.push_back('\0');
		send(cSocket, s.c_str(), s.size(), NULL);
		//通知全部用户，有新用户登录
		s.clear();
		s.push_back(NEWUSERLOGIN);
		s.push_back(' ');
		s.append("Server");
		s.push_back(' ');
		s.append("ALL");
		s.push_back(' ');
		s.append(W2A(receivedData.Sender.c_str()));
		s.push_back('\0');
		for (int i = 0; i < allSocket.size(); i++) {
			send(allSocket[i], s.c_str(), s.size(), NULL);
		}
		ProtectUserList.unlock();
		return;
	}
}

void OnLogOut(SOCKET cSocket, UserParam receivedData) {
	UserList.erase(receivedData.Sender);
	ID_UserName[this_thread::get_id()] = L" ";
	string buffer;
	buffer.push_back(LOGOUT_RESULT);
	buffer.push_back(' ');
	buffer.append("Server");
	buffer.push_back(' ');
	buffer.append("ALL");
	buffer.push_back(' ');
	USES_CONVERSION;
	buffer.append(W2A(receivedData.Sender.c_str()));
	for (int i = 0; i < allSocket.size(); i++) {
		send(allSocket[i], buffer.c_str(), buffer.size(), NULL);
	}
	closesocket(cSocket);
}


//处理发送聊天记录函数
void OnSendChatRecord(SOCKET cSocket, UserParam receivedData) {

}
//处理发送用户列表函数
void OnSendUserList(SOCKET cSocket, UserParam receivedData) {

}

void SendRecvProc() {
	//6.等待客户端链接
	SOCKET cSocket = accept(Initsocket, NULL, NULL);
	if (cSocket == SOCKET_ERROR) {
		//如果等待失败说明服务器崩溃输出报错信息
		closesocket(cSocket); //断开链接
		cout << "服务器崩溃" << endl;
		return;
	}
	allSocket.push_back(cSocket);
	int ret = 0;
	//7.通信
	char recvBuff[MAX_BUFFER_SIZE];
	while (true) {
		//接收消息
		ret = recv(cSocket, recvBuff, MAX_BUFFER_SIZE, NULL);
		if (ret <= 0) {
			//如果消息接收异常
			closesocket(cSocket); //断开链接
			break;
		}
		//将接收到的数据转换为宽字符
		USES_CONVERSION;
		wchar_t* wRecvBuff = A2W(recvBuff);
		wstring buffer(wRecvBuff);
		//将接受到的数据解包，分析。
		UserParam receivedData;
		openTheBuff(buffer, receivedData);
		
		cout << "接收到用户信息：";
		switch (receivedData.Type)
		{
		case NORMAL_MSG:
			cout << "普通消息 ";
			break;
		case LOGIN_REQUEST:
			cout<<"登录请求 ";
			break;
		case LOGOUT_REQUEST:
			cout<<"登出请求 ";
			break;
		case USERLIST_REQUEST:
			cout<<"用户列表请求 ";
			break;
		case LOGIN_RESULT:
			cout<<"登录结果 ";
			break;
		case LOGOUT_RESULT:
			cout<<"登出结果 ";
			break;
		case NEWUSERLOGIN:
			cout<<"新用户登录信息 ";
			break;
		case GLOBALRECORD_REQUEST:
			cout<<"全球聊天室聊天记录请求 ";
			break;
		case GLOBALRECORD_RESULT:
			cout<<"全球聊天室聊天记录结果 ";
			break;
		case CNRECORD_REQUEST:
			cout<<"中国聊天室聊天记录请求 ";
			break;
		case CNRECORD_RESULT:
			cout<<"中国聊天室聊天记录结果 ";
			break;
		case USERLIST_RESULT:
			cout<<"用户列表结果 ";
			break;
		default:
			cout << "未知消息类型 ";
			break;
		}
		cout << "发送者：";
		cout << string(W2A(receivedData.Sender.c_str()))<<' ';
		cout << "接收者：";
		cout << string(W2A(receivedData.Receiver.c_str()))<<' ';
		cout << " 消息内容: ";
		cout << string(W2A(receivedData.Msg.c_str()))<<" ";
		cout<< endl;
		//如果接收到的数据类型不是登录请求或者登出请求或者普通消息则不处理
		if (recvBuff[0] != LOGIN_REQUEST && recvBuff[0] != LOGOUT_REQUEST && recvBuff[0] != NORMAL_MSG && recvBuff[0] != LOGIN_RESULT) {
			continue;
		}
		//根据数据类型来处理消息内容
		switch (recvBuff[0]) {
		case NORMAL_MSG:
			OnSend(cSocket, receivedData);
			break;
		case LOGIN_REQUEST:
			OnLogIn(cSocket, receivedData);
			break;
		case LOGOUT_REQUEST:
			OnLogOut(cSocket, receivedData);
			break;
		case USERLIST_REQUEST:
			OnSendUserList(cSocket, receivedData);
			break;
		default:
			cout << "未知消息: "<<recvBuff[0] << " 不处理  " << endl;
			break;
		}
		cout << "完毕" << endl;
	}
}
#pragma once
#include "framework.h"
using namespace std;
typedef long long ll;



#define NORMAL_MSG  1
#define LOGIN_REQUEST  2
#define LOGOUT_REQUEST 3
#define LOGIN_RESULT 4

template<typename T> //使用函数模板的MyRandom函数
T MyRandom(T min, T max);

typedef struct UserMessage {
    string UserName;
    vector<string> Msg;
    CTime timeInfo;
};


//定义用户使用socket发送的数据包的格式
typedef struct UserParam
{
    int Type;
    string Sender;
    string Receiver;
    UserMessage Msg;
    bool res;
};


// 序列化函数：将结构体数据复制到字符数组中 方便使用socket发送数据
extern void serialize(const UserParam& data, char* buffer);

// 反序列化函数：将字符数组转换为结构体数据 解包接受到的数据
extern void deserialize(const char* buffer, UserParam& data);

/*
将string类型转化为wchar_t*方法

1.先将string转化为wstring
2.然后将wstring转化为wchar_t*

string s;
wstring ss(s.begin(),s.end());
wchar_t* res=ss.c_str();
*/
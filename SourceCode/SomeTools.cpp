#pragma once
#include "framework.h"
#include "SomeTools.h"
using namespace std;
typedef long long ll;



#define NORMAL_MSG  1
#define LOGIN_REQUEST  2
#define LOGOUT_REQUEST 3
#define LOGIN_RESULT 4

template<typename T> //使用函数模板的MyRandom函数
T MyRandom(T min, T max) { //采用C++提供的随机数生成引擎来生成随机数
    random_device seed;//硬件生成随机数种子
    ranlux48 engine(seed());//利用种子生成随机数引擎
    uniform_int_distribution<> distrib(min, max);//设置随机数范围，随机数分布方式为均匀分布
    T res = distrib(engine);//随机数
    return res;
}






// 序列化函数：将结构体数据复制到字符数组中 方便使用socket发送数据
void serialize(const UserParam& data, char* buffer) {
    memcpy(buffer, &data, sizeof(UserParam));
}

// 反序列化函数：将字符数组转换为结构体数据 解包接受到的数据
void deserialize(const char* buffer, UserParam& data) {
    memcpy(&data, buffer, sizeof(UserParam));
}

/*
将string类型转化为wchar_t*方法

1.先将string转化为wstring
2.然后将wstring转化为wchar_t*

string s;
wstring ss(s.begin(),s.end());
wchar_t* res=ss.c_str();
*/
#include<iostream>
#include<random>
using namespace std;
typedef long long ll;

template<typename T> //使用函数模板的MyRandom函数
T MyRandom(T min, T max) { //采用C++提供的随机数生成引擎来生成随机数
    random_device seed;//硬件生成随机数种子
    ranlux48 engine(seed());//利用种子生成随机数引擎
    uniform_int_distribution<> distrib(min, max);//设置随机数范围，随机数分布方式为均匀分布
    T res = distrib(engine);//随机数
    return res;
}

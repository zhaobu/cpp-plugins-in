#ifndef REGISTER_H
#define REGISTER_H

#include "dlload.h"
#include "dllobject.h"

#include <map>

class RegisterM {
public:
    enum MethodType { Method, Constructor };      // 函数类型
    enum Access { Private, Protected, Public };   // 访问方式
    enum CallType { Asynchronous, Synchronous };  // 函数调用方式
    enum SetType { SetVal, getVal };              // 属性值设置
public:
    RegisterM(){};
    ~RegisterM(){};
    // 注册类库
    int        registerObject(const char* objectName, const char* libpath);
    // 注销类库
    bool       unregisterObject(const char* objectName);
    // 创建实例类
    create_t*  getInstance(const char* objectName);
    // 析构实例类
    destroy_t* rmInstance(const char* objectName);
    // 函数调用
    void*      getFunc(const char* objectName, char* funcName);

private:
    MODULE_HANDLE index(const char* Name);

private:
    std::map<char*, MODULE_HANDLE> libmap;
};

#endif
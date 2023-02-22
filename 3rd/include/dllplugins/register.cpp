#include "register.h"

#include "utils/logger.h"

#include <iostream>
#include <stdio.h>

int RegisterM::registerObject(const char* objectName, const char* libpath) {
    MODULE_HANDLE load_handle = gdl_Open(libpath);
    if(!load_handle) {
        SPDLOG_ERROR("Cannot load library: {}, Error:{}", libpath, gdl_GetLastError());
        return -1;
    }
    SPDLOG_INFO("libname :{} register success", objectName);
    libmap[const_cast<char*>(objectName)] = load_handle;
    return 1;
}

bool RegisterM::unregisterObject(const char* objectName) {
    std::map<char*, MODULE_HANDLE>::iterator it = libmap.find(const_cast<char*>(objectName));
    if(it != libmap.end()) {
        gdl_Close(it->second);
        libmap.erase(it);
        SPDLOG_INFO("libname :{} unregister success", objectName);
        return true;
    }
    SPDLOG_WARN("libname :{} unregister failed", objectName);
    return false;
}

create_t* RegisterM::getInstance(const char* objectName) {
    MODULE_HANDLE _handle = index(objectName);
    if(NULL != _handle) {
        create_t*   create_instance = (create_t*)gdl_GetProc(_handle, "create");
        const char* dlsym_error     = gdl_GetLastError();
        if(dlsym_error) {
            SPDLOG_ERROR("Cannot load symbol create error: {}", dlsym_error);
            return NULL;
        } else {
            SPDLOG_INFO("libname :{} getInstance success", objectName);
            return create_instance;
        }
    }
    return NULL;
}

destroy_t* RegisterM::rmInstance(const char* objectName) {
    MODULE_HANDLE _handle = index(objectName);
    if(NULL != _handle) {
        destroy_t*  destroy_instance = (destroy_t*)gdl_GetProc(_handle, "destroy");
        const char* dlsym_error      = gdl_GetLastError();
        if(dlsym_error) {
            SPDLOG_ERROR("Cannot load symbol create: {}", dlsym_error);
            return NULL;
        } else {
            SPDLOG_INFO("libname :{} rmInstance success", objectName);
            return destroy_instance;
        }
    }
    return NULL;
}

void* RegisterM::getFunc(const char* objectName, char* funcName) {
    MODULE_HANDLE _handle = index(objectName);
    if(NULL != _handle) {
        void*       ret         = gdl_GetProc(_handle, funcName);
        const char* dlsym_error = gdl_GetLastError();
        if(dlsym_error) {
            SPDLOG_ERROR("Cannot load symbol create: {}", dlsym_error);
            return NULL;
        } else {
            SPDLOG_INFO("libname :{} getfunc:{} success", objectName, funcName);
            return ret;
        }
    }
    return NULL;
}

MODULE_HANDLE RegisterM::index(const char* Name) {
    std::map<char*, MODULE_HANDLE>::iterator it = libmap.find(const_cast<char*>(Name));
    if(it != libmap.end()) {
        return it->second;
        SPDLOG_INFO("index:{} find success", Name);
    } else {
        SPDLOG_ERROR("Cannot find library: {}", Name);
    }
    return NULL;
}
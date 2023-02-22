#pragma once

#ifdef WIN32
#ifdef __cplusplus
#define EXPORT_DLL extern "C" __declspec(dllexport)
#else
#define EXPORT_DLL __declspec(dllexport)
#endif
#else
#define EXPORT_DLL extern "C"
#endif

class DllObject {
public:
    DllObject(){};
    virtual ~DllObject(){};

    virtual int init(void* arg) {
        return 0;
    };

    virtual int deinit() {
        return 0;
    };

    virtual int run(void* arg) = 0;
};

//
typedef DllObject* create_t();
typedef void       destroy_t(DllObject*);
// typedef _declspec(dllimport) void destroy_t(DllObject*); //window

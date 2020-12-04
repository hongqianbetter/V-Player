//
// Created by hongqian.better on 2020/11/24.
//

#ifndef VPLAYER_JNICALLBACK_H
#define VPLAYER_JNICALLBACK_H


#include <jni.h>
#include "macro.h"
#include <android/log.h>
class JNICallback {

public:
    JNICallback(JavaVM * javaVm , JNIEnv * env,jobject instance);

    ~JNICallback();

    //具体回调给上层的函数
    void onPrepared(int thread_mode);

    void onErrorAction(int thread_mode,int error_code);



private:
    JavaVM *javaVm=0;
    JNIEnv * env=0;
    jobject  instance;  //jobject不是指针 不能赋值为0
    jmethodID  jmd_prepared;
    jmethodID  jmd_error;
};


#endif //VPLAYER_JNICALLBACK_H

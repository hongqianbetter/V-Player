//
// Created by hongqian.better on 2020/11/24.
//

#include "JNICallback.h"



JNICallback::JNICallback(JavaVM *javaVm, JNIEnv *env, jobject instance) {

    this->javaVm = javaVm;
    this->env = env;

    //instance 有可能被另一端被释放掉 这个时候就需要全局引用了
    this->instance = env->NewGlobalRef(instance); //全局引用

    jclass myPlayerClass = env->GetObjectClass(this->instance);

    const char *sig = "()V";
    //回调给上层的JNI函数
    this->jmd_prepared = env->GetMethodID(myPlayerClass, "onPrepared", sig);

    sig = "(I)V";
    this->jmd_error = env->GetMethodID(myPlayerClass, "onError", sig);


}

/**
 * 析构函数：专门完成释放的工作
 */
JNICallback::~JNICallback() {
    this->javaVm = 0;
    env->DeleteGlobalRef(this->instance);
    this->instance = 0;
    env = 0;

}


void JNICallback::onPrepared(int thread_mode) {

    if (thread_mode == THREAD_MAIN) { //主线程
        //主线程 env
        this->env->CallVoidMethod(this->instance, this->jmd_prepared);   //调用上层
    } else if (thread_mode == THREAD_CHILD) {
        //子线程 env不能用了 开始附加线程
        JNIEnv *jniEnv = nullptr;
        jint ret = this->javaVm->AttachCurrentThread(&jniEnv, 0);
        if (ret != JNI_OK) {
            return;
        }

        jniEnv->CallVoidMethod(this->instance, this->jmd_prepared);

        //解除附加 释放工作
        this->javaVm->DetachCurrentThread();
    }

}

void JNICallback::onErrorAction(int thread_mode, int error_code) {


    if (thread_mode == THREAD_MAIN) { //主线程
        //主线程 env
        this->env->CallVoidMethod(this->instance, this->jmd_error);   //调用上层
    } else if (thread_mode == THREAD_CHILD) {
        //子线程 env不能用了 开始附加线程
        JNIEnv *jniEnv = nullptr;
        int ret = this->javaVm->AttachCurrentThread(&jniEnv, 0);
        if (ret != JNI_OK) {
            return;
        }

        jniEnv->CallVoidMethod(this->instance, this->jmd_error,error_code);

        //解除附加 释放工作
        this->javaVm->DetachCurrentThread();
    }
}



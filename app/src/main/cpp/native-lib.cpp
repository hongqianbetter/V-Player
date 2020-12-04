#include <jni.h>
#include <string>
#include "MyPlayer.h"
#include "JNICallback.h"
#include <android/log.h>
#include <android/native_window_jni.h> // 是为了 渲染到屏幕支持的

extern "C" {
#include <libavutil/avutil.h>
}

JavaVM *javaVm = 0;
MyPlayer *myPlayer = 0;

ANativeWindow * nativeWindow = 0; // 为什么定义到上面 就是为了和 Surface 对象关联
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // 静态初始化 互斥锁



int JNI_OnLoad(JavaVM *javaVm, void *pVoid) {
    ::javaVm = javaVm;
    return JNI_VERSION_1_6;//注意这里必须要返回一样 ,否则报错
}

/**
 * 专门渲染的函数
 * @param src_data 解码后的视频 rgba 数据
 * @param width 宽信息
 * @param height 高信息
 * @param src_liinesize 行数size相关信息
 */
void renderFrame(uint8_t * src_data, int width, int height, int src_linesize) {
    pthread_mutex_lock(&mutex);



    if (!nativeWindow) {
        pthread_mutex_unlock(&mutex);
    }
    __android_log_print(ANDROID_LOG_ERROR, "player","aaaaaaaaa--------");
    // 设置窗口属性
    ANativeWindow_setBuffersGeometry(nativeWindow, width, height , WINDOW_FORMAT_RGBA_8888);

    ANativeWindow_Buffer windowBuffer; // 缓冲区

    if (ANativeWindow_lock(nativeWindow, &windowBuffer, 0)) {
        ANativeWindow_release(nativeWindow);
        nativeWindow = 0;
        pthread_mutex_unlock(&mutex);
        return;
    }

    // 填数据到buffer，其实就是修改数据
    uint8_t * dst_data = static_cast<uint8_t *>(windowBuffer.bits);
    int lineSize = windowBuffer.stride * 4; // RGBA 相当于是 每一个像素点 * rgba

    // 下面就是逐行Copy了
    for (int i = 0; i < windowBuffer.height; ++i) {
        // 一行 一行 的 Copy 到 Android屏幕上
        memcpy(dst_data + i * lineSize, src_data + i * src_linesize, lineSize);
    }

    ANativeWindow_unlockAndPost(nativeWindow);
    pthread_mutex_unlock(&mutex);
}



extern "C"
JNIEXPORT void JNICALL
Java_com_example_vplayer_MyPlayer_prepareNative(JNIEnv *env, jobject thiz, jstring data_source_) {

    JNICallback *jniCallback = new JNICallback(::javaVm, env, thiz);

    const char *dataSource = env->GetStringUTFChars(data_source_, NULL);
    //面向对象 创建MyPlayer
    myPlayer = new MyPlayer(dataSource, jniCallback);

    myPlayer->setRenderCallback(renderFrame); // 设置函数指针-- 的具体函数

    __android_log_print(ANDROID_LOG_ERROR, "player", "prepareNative--");
    myPlayer->prepare();
    env->ReleaseStringUTFChars(data_source_, dataSource);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_vplayer_MyPlayer_startNative(JNIEnv *env, jobject thiz) {
    if (myPlayer) {
        myPlayer->start();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_vplayer_MyPlayer_stopNative(JNIEnv *env, jobject thiz) {
    // TODO: implement stopNative()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_vplayer_MyPlayer_releaseNative(JNIEnv *env, jobject thiz) {
    // TODO: implement releaseNative()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_vplayer_MyPlayer_setSurfaceNative(JNIEnv *env, jobject thiz, jobject surface) {

    pthread_mutex_lock(&mutex);

    if (nativeWindow) {
        ANativeWindow_release(nativeWindow);
        nativeWindow = 0;
    }

    // 创建新的窗口用于视频显示，关联起来
    nativeWindow = ANativeWindow_fromSurface(env, surface);

    pthread_mutex_unlock(&mutex);
    __android_log_print(ANDROID_LOG_ERROR, "player","native window设置了");
}
//extern "C"
//JNIEXPORT jstring JNICALL
//Java_com_example_musicplayer_MainActivity_getFFmpegVersion(JNIEnv *env, jobject thiz) {
//    // TODO: implement getFFmpegVersion()
//}



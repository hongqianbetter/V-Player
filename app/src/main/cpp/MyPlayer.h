//
// Created by hongqian.better on 2020/11/23.
//

#ifndef VPLAYER_MYPLAYER_H
#define VPLAYER_MYPLAYER_H

#include <cstring>
#include <pthread.h>
#include "VideoChannel.h"
#include "AudioChannel.h"
#include "JNICallback.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/time.h>
}

#include "malloc.h"
#include "safe_queue.h"
//#include <android/log.h>

class MyPlayer {

public:

    MyPlayer( );
    MyPlayer(const char *string, JNICallback *jniCallback);

    virtual ~MyPlayer();

    void prepare();

    void prepare_();

    void start();

    void start_();

    void setRenderCallback(RenderCallback renderCallback);

private:
    char *data_source=0;

    pthread_t pid_prepare = 0;

    AVFormatContext *avFormatContext = 0; //媒体上下文

    AudioChannel *audioChannel = 0;
    VideoChannel *videoChannel = 0;

    JNICallback *jniCallback = 0;

    int isPlaying;

    pthread_t pid_start = 0;

    // native-lib.cpp prepareNative函数执行的时候，会把"具体函数"传递到此处
    RenderCallback  renderCallback;
};


#endif //VPLAYER_MYPLAYER_H

//
// Created by hongqian.better on 2020/11/24.
//

#ifndef VPLAYER_VIDEOCHANNEL_H
#define VPLAYER_VIDEOCHANNEL_H

#include "BaseChannel.h"
#include "AudioChannel.h"
extern "C" {
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>

}


typedef void (*RenderCallback)(uint8_t *, int, int, int);

class VideoChannel : public BaseChannel {

public:

    VideoChannel(int stream_index, AVCodecContext *avCodecContext, AVRational rational, int fpsValue);

    virtual ~VideoChannel();

    void start();

    void stop();


    void video_decode();

    void video_player();

    void setAudioChannel(AudioChannel *audioChannel);
    void setRenderCallback(RenderCallback renderCallback);

private:


    pthread_t pid_video_decode = 0;
    pthread_t pid_video_player = 0;


    // native-lib.cpp prepareNative函数执行的时候，会把"具体函数"传递到此处
    RenderCallback renderCallback;

    int fpsValue; // fps 是视频独有的东西
    AudioChannel *  audioChannel = 0;
};


#endif //VPLAYER_VIDEOCHANNEL_H

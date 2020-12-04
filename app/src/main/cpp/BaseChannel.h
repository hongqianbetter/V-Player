//
// Created by hongqian.better on 2020/11/24.
//

#ifndef VPLAYER_BASECHANNEL_H
#define VPLAYER_BASECHANNEL_H

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/time.h>
}

#include "safe_queue.h"
#include <android/log.h>

class BaseChannel {


public:
    int stream_index;

    BaseChannel(int stream_index, AVCodecContext *avCodecContext, AVRational time_base) {
        this->stream_index = stream_index;
        this->avCodecContext=avCodecContext;
        this->time_base = time_base;
        packets.setReleaseCallback(releaseAVPacket);
        frames.setReleaseCallback(releaseAVFrame);
    }

    // 注意：由于是父类，析构函数，必须是虚函数
    virtual ~BaseChannel() {
        packets.clearQueue();
        frames.clearQueue();
    }

    /**
     * 释放AVPacket 队列
     * @param avPacket
     */
    static void releaseAVPacket(AVPacket ** avPacket) {
        if (avPacket) {
            av_packet_free(avPacket);
            *avPacket = 0;
        }
    }

    /**
     * 释放AVFrame 队列
     * @param avFrame
     */
    static void releaseAVFrame(AVFrame ** avFrame) {
        if (avFrame) {
            av_frame_free(avFrame);
            *avFrame = 0;
        }
    }

    //音频 aac 视频 h264
    SafeQueue<AVPacket *> packets;

    //音频 pcm 视频 yuv
    SafeQueue<AVFrame *> frames; //可以直接渲染 和播放的

    bool isPlaying = 1;

    AVCodecContext *avCodecContext;
    // 时间基 FFmpeg内部的时间
    // 例如：以视频为例：24fps == 一秒钟24帧，那么用时间基表示就是 每一帧 == 时间基(1/24)
    AVRational time_base;
    double audioTime; // 每一帧，音频计算后的时间值
    double videoTime; // 每一帧，视频计算后的时间值
};


#endif //VPLAYER_BASECHANNEL_H

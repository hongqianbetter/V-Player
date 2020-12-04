//
// Created by hongqian.better on 2020/11/23.
//

#include "MyPlayer.h"

//函数指针
void *customTaskPrepareThread(void *pVoid) {
    MyPlayer *myPlayer = static_cast<MyPlayer *>(pVoid);

    myPlayer->prepare_();

    return 0; //必须返回 有坑
}

void *customTaskStartThread(void *pVoid) {
    MyPlayer *myPlayer = static_cast<MyPlayer *>(pVoid);
    myPlayer->start_();
    return 0;
}

MyPlayer::MyPlayer( ) {}

MyPlayer::MyPlayer(const char *data_source, JNICallback *pCallback) {
    //有坑 长度不对  copy一下 防止悬空指针
    this->data_source = new char[strlen(data_source) + 1];
    strcpy(this->data_source, data_source);
    this->jniCallback = pCallback;

}

MyPlayer::~MyPlayer() {
    if (this->data_source) {
        delete this->data_source;
        this->data_source = 0;
    }
}




//准备工作是去 "demuxer" 拆包裹 音频流 视频流 字幕流

void MyPlayer::prepare() { //当前依然在主线程 开线程

    pthread_create(&pid_prepare, 0, customTaskPrepareThread, this);
}

//异步的函数  解封装
void MyPlayer::prepare_() {
    avformat_network_init();// todo 初始化网络
    this->avFormatContext = avformat_alloc_context();

    AVDictionary *dictionary = 0;
    av_dict_set(&dictionary, "timeout", "500000000", 0);//注意是微秒
    //字典能够决定我们打开的需求
    int ret = avformat_open_input(&this->avFormatContext, this->data_source, 0, &dictionary);

//    @@@ 内存泄漏相关
    av_dict_free(&dictionary);//释放字典
    if (ret) {//0成功 其他为false
        // ... 写JNI回调 告诉Java层 通知用户 你的播放流是损坏的
        if (this->jniCallback) {
            this->jniCallback->onErrorAction(THREAD_CHILD, FFMPEG_CAN_NOT_OPEN_URL);
        }
        __android_log_print(ANDROID_LOG_ERROR, "player", "1--");
        return;
    }

//   寻找 媒体格式中的 音频 视频 字幕  媒体上下文初始化流信息
    ret = avformat_find_stream_info(this->avFormatContext, 0); //为什么不给字典 是因为 不需要额外设置

    if (ret < 0) {
        // ... 写JNI回调 告诉Java层 通知用户 你的播放流是损坏的
        if (this->jniCallback) {
            this->jniCallback->onErrorAction(THREAD_CHILD, FFMPEG_CAN_NOT_FIND_STREAMS);
        }
        __android_log_print(ANDROID_LOG_ERROR, "player", "2--");
        return;
    }

    //媒体上下文就有丰富的值了 avFormatContext

    //视频压缩格式H264 音频压缩格式AAC
    //循环遍历 媒体格式里面的 流1==音频 流2==字幕 流0==视频
    for (int i = 0; i < this->avFormatContext->nb_streams; ++i) {

        AVStream *stream = this->avFormatContext->streams[i];

        //得到解码器ID ,为了获取解码器  从stream流中获取解码这段流的参数信息,区分到底是音频 还是视频
        AVCodecParameters *codecParameters = stream->codecpar;

        //拿到解码器
        AVCodec *codec = avcodec_find_decoder(codecParameters->codec_id);
        if (!codec) { //虽然在第六步 已经拿到当前流的解码器了 但可能不支持解码这种解码方式
            if (this->jniCallback) { //如果为空 就代表 解码器不支持
                // ... 写JNI回调 告诉Java层 通知用户 你的播放流是损坏的
                this->jniCallback->onErrorAction(THREAD_CHILD, FFMPEG_FIND_DECODER_FAIL);
            }
            __android_log_print(ANDROID_LOG_ERROR, "player", "3--");
            return;
        }

        //通过解码器 获取解码器上下文
        AVCodecContext *codecContext = avcodec_alloc_context3(codec);

        if (!codecContext) {
            if (this->jniCallback) {
                // ... 写JNI回调 告诉Java层 通知用户 你的播放流是损坏的
                this->jniCallback->onErrorAction(THREAD_CHILD, FFMPEG_ALLOC_CODEC_CONTEXT_FAIL);
            }
            __android_log_print(ANDROID_LOG_ERROR, "player", "4--");
            return;
        }
        //还需要给解码器上下文设置参数
        ret = avcodec_parameters_to_context(codecContext, codecParameters);
        if (ret < 0) {
            // ... 写JNI回调 告诉Java层 通知用户 你的播放流是损坏的
            if (this->jniCallback) {
                this->jniCallback->onErrorAction(THREAD_CHILD,
                                                 FFMPEG_CODEC_CONTEXT_PARAMETERS_FAIL);
            }
            __android_log_print(ANDROID_LOG_ERROR, "player", "5--");
            return;
        }

        //打开解码器
        ret = avcodec_open2(codecContext, codec, 0);
        if (ret) {
            // ... 写JNI回调 告诉Java层 通知用户 你的播放流是损坏的
            this->jniCallback->onErrorAction(THREAD_CHILD, FFMPEG_OPEN_DECODER_FAIL);
            __android_log_print(ANDROID_LOG_ERROR, "player", "6--");
            return;
        }

        // AVStream媒体流中就可以拿到时间基 (音视频同步)
        AVRational time_base = stream->time_base;

        //解码器打开成功 就能去使用解码器了
        //区分音频/视频

        //音频流
        if (codecParameters->codec_type == AVMEDIA_TYPE_AUDIO) { //音频流
            this->audioChannel = new AudioChannel(i, codecContext,time_base);
            //视频流 目前很多字幕流 都放在视频轨道中了
        } else if (codecParameters->codec_type == AVMEDIA_TYPE_VIDEO) {
            // 获取视频相关的 fps
            // 平均帧率 == 时间基
            AVRational frame_rate = stream->avg_frame_rate;
            int fpsValue = av_q2d(frame_rate);
            this->videoChannel = new VideoChannel(i, codecContext, time_base, fpsValue);
            this->videoChannel->setRenderCallback(renderCallback);

        }

        if (!audioChannel && !videoChannel) {
            if (this->jniCallback) {
                this->jniCallback->onErrorAction(THREAD_CHILD, FFMPEG_NOMEDIA);
            }
            return;
        }



    }

    //告诉Java层 我准备好了
    if (this->jniCallback) {
        this->jniCallback->onPrepared(THREAD_CHILD);
        __android_log_print(ANDROID_LOG_ERROR, "player","jni prepared--------");
    }

}




//开始播放 需要异步
void MyPlayer::start() {
    isPlaying = 1; //播放的状态
    //开始播放 需要异步
    //让音频和视频两个通道运行起来
    __android_log_print(ANDROID_LOG_ERROR, "player","start   start--------");
    if (this->videoChannel) {
        this->videoChannel->setAudioChannel(audioChannel);
        this->videoChannel->start();
    }

    if (this->audioChannel) {
        this->audioChannel->start();
    }

    //开启线程 (读包: 把未解码的流数据 音频 视频 )放入队列
    pthread_create(&pid_start, 0, customTaskStartThread, this);

}


void MyPlayer::start_() {
    //循环读取 视频包
    while (this->isPlaying) {

//         由于操作是在异步线程 那就好办了 等待 先让消费者消费掉 然后再生产
        //内存泄漏点 1 解决方案 控制队列大小
        if (videoChannel && videoChannel->packets.queueSize() > 100) {
            av_usleep(10 * 1000);   //休眠 等待队列中的数据被消费
            continue;
        }

        if (audioChannel && audioChannel->packets.queueSize() > 100) {
            av_usleep(10 * 1000);   // 休眠 等待队列中的数据被消费
            continue;
        }


        //未解码的格式 是保存在AVPacket里面的
        AVPacket *packet = av_packet_alloc();
        int ret = av_read_frame(this->avFormatContext, packet);// 这行代码一执行完毕，packet就有（音视频数据）
//
//        if (ret){
//            return;
//        }
        if (!ret) {
            //把已经得到的packet放入队列中
            //先要判断是音频还是视频
            if (videoChannel && videoChannel->stream_index == packet->stream_index) {
                //说明是视频包了
                this->videoChannel->packets.push(packet);
            } else if (audioChannel && audioChannel->stream_index == packet->stream_index) {
                //说明是音频包了
                this->audioChannel->packets.push(packet);
            }
        } else if (ret == AVERROR_EOF) {//文件末尾 ,读完了
            //todo 后续处理
        } else {
            //代表失败了 有问题
            break;
        }


    } //end while

    // 最后做释放的工作
    isPlaying = 0; // 标记清零
    videoChannel->stop();
    audioChannel->stop();

}

void MyPlayer::setRenderCallback(RenderCallback renderCallback) {
    this->renderCallback = renderCallback;
}

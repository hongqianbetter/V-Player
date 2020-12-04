package com.example.vplayer;

public class Flags {

    //打不开媒体数据源
    // #define FFMPEG_CAN_NOT_OPEN_URL (ERROR_CODE_FFMPEG_PREPARE - 1)
    public static final int FFMPEG_CAN_NOT_OPEN_URL = -1;

    //找不到媒体流信息
    // #define FFMPEG_CAN_NOT_FIND_STREAMS (ERROR_CODE_FFMPEG_PREPARE - 2)
    public static final int FFMPEG_CAN_NOT_FIND_STREAMS = -2;

    //找不到解码器
    // #define FFMPEG_FIND_DECODER_FAIL (ERROR_CODE_FFMPEG_PREPARE - 3)
    public static final int FFMPEG_FIND_DECODER_FAIL = -3;

    //无法根据解码器创建上下文
    // #define FFMPEG_ALLOC_CODEC_CONTEXT_FAIL (ERROR_CODE_FFMPEG_PREPARE - 4)
    public static final int FFMPEG_ALLOC_CODEC_CONTEXT_FAIL = -4;

    //根据流信息 配置上下文参数失败
    // #define FFMPEG_CODEC_CONTEXT_PARAMETERS_FAIL (ERROR_CODE_FFMPEG_PREPARE - 5)
    public static final  int FFMPEG_CODEC_CONTEXT_PARAMETERS_FAIL = -5;

    //打开解码器失败
    // #define FFMPEG_OPEN_DECODER_FAIL (ERROR_CODE_FFMPEG_PREPARE - 6)
    public static final int FFMPEG_OPEN_DECODER_FAIL = -6;

    //没有音视频
    // #define FFMPEG_NOMEDIA (ERROR_CODE_FFMPEG_PREPARE - 7)
    public static final int FFMPEG_NOMEDIA = -7;

    //读取媒体数据包失败
    // #define FFMPEG_READ_PACKETS_FAIL (ERROR_CODE_FFMPEG_PLAY - 8)
    public static final int FFMPEG_READ_PACKETS_FAIL = -8;

    public static String getErrorStrByCode(int errorCode) {
        String str = null;
        switch (errorCode){
            case Flags.FFMPEG_ALLOC_CODEC_CONTEXT_FAIL:

                str = "无法根据解码器创建上下文";
                break;

            case Flags.FFMPEG_CAN_NOT_FIND_STREAMS:

                str = "找不到媒体流信息";
                break;

            case Flags.FFMPEG_CAN_NOT_OPEN_URL:

                str = "打不开媒体数据";
                break;

            case Flags.FFMPEG_CODEC_CONTEXT_PARAMETERS_FAIL:

                str = "根据流信息,配置上下文参数失败";
                break;

            case Flags.FFMPEG_FIND_DECODER_FAIL:

                str = "找不到解码器";
                break;

            case Flags.FFMPEG_NOMEDIA:

                str = "木有音视频";
                break;

            case Flags.FFMPEG_READ_PACKETS_FAIL:

                str = "获取媒体数据包失败";
                break;
            default:

                str = "未知错误,自己去检测你的垃圾代码...";
                break;
        }

        return str;


    }


}

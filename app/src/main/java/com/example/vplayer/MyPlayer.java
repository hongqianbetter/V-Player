package com.example.vplayer;

import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import androidx.annotation.NonNull;

/**
 * Created by hongqian.better@outlook.com
 * on 2020/11/23
 */
public class MyPlayer implements SurfaceHolder.Callback {
    private String dataSource; //rtmp直播流 本地媒体文件

    private SurfaceHolder holder;

    static {
        System.loadLibrary("v-player");
    }

    public MyPlayer() {
    }

    public void setDataSource(String dataSource) {
        this.dataSource = dataSource;
    }

    public void setSurfaceView(SurfaceView surfaceView) {
        //每次设置SurfaceView的时候,判断holder释放被清除

        if (null != holder) {
            Log.e("player","java remove--");
            holder.removeCallback(this);
        }

        holder = surfaceView.getHolder();
        holder.addCallback(this);
        Log.e("player","java add callback--");

    }

    //Surface 创建回调

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder holder) {
        Log.e("player","java surfacing---prepared--");
    }


    //Surface发生改变回调
    @Override
    public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {
        Log.e("player","java surfacing-----");
        setSurfaceNative(holder.getSurface());
    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder holder) {
        Log.e("player","java surfacing---destory--");
    }

    public void prepare() {
        prepareNative(this.dataSource);
    }

    public void start() {
        startNative();
    }

    public void stop() {
        stopNative();
    }

    public void release() {
        releaseNative();
    }

    private native void prepareNative(String dataSource);

    private native void startNative();

    private native void stopNative();

    private native void releaseNative();

    private native void setSurfaceNative(Surface surface); //告诉底层 上层可以怎么去渲染 其实底层是操纵Surface


    OnPreparedListener listener;

    public void setPreparedListener(OnPreparedListener listener) {
        this.listener = listener;
    }

    /**
     * 给native调用的函数
     */

    public void onPrepared() {
        if (listener != null) {
            listener.onPrepared();
        }

    }

    public void onError(int errorCode) {

        if (listener != null) {
            listener.onError(Flags.getErrorStrByCode(errorCode));
        }
    }

    interface OnPreparedListener {
        void onPrepared();

        void onError(String errorText);
    }

}

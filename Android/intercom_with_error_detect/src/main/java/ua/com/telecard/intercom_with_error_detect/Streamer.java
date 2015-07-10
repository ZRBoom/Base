package ua.com.telecard.intercom_with_error_detect;

import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.util.Log;
import android.widget.ProgressBar;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;

/**
 * Created by Roman on 12.05.2015.
 */
public class Streamer {
    Streamer(int Porrt, int ID, float LevelOfNoise){
        sampleRate = 8000;
        channelConfig = AudioFormat.CHANNEL_CONFIGURATION_MONO;
        audioFormat = AudioFormat.ENCODING_PCM_16BIT;
        minInternalBufferSize = AudioRecord.getMinBufferSize(sampleRate, channelConfig, audioFormat);
        myBufferSize = minInternalBufferSize;
        bufferSend = new byte[6][myBufferSize+2];//TODO

        MyID = ID;
        NoiseLevel = LevelOfNoise;
        NoiseLevel = LevelOfNoise;

        createAudioRecorder();
        createMulticastSender(Porrt);

        /*recordStart();
        readStart();*/

    }

    int sampleRate;
    int channelConfig;
    int audioFormat;
    int minInternalBufferSize;
    int myBufferSize;
    float totalAbsValue = 0.0f;

    float NoiseLevel;

    AudioRecord audioRecord;
    Thread RecAndSend;

    byte [][] bufferSend;
    InetAddress ia;
    MulticastSocket ms;
    int port;
    int MyID;

    final String LOG_TAG = "myLogs";

    //Создание Аудио рекордреа
    void createAudioRecorder(){
        Log.d(LOG_TAG, "minInternalBufferSize = " + minInternalBufferSize
                + ", myBufferSize = " + myBufferSize);

        audioRecord = new AudioRecord(MediaRecorder.AudioSource.DEFAULT, sampleRate, channelConfig,
                audioFormat, minInternalBufferSize);
    }

    //Создание Сокета для отправки мультикастовых сообщений
    void createMulticastSender(int Porrt){
        ia = null;
        ms = null;
        port = Porrt;
        try {
            ia = InetAddress.getByName("224.2.76.24");
            ms = new MulticastSocket(port);
            ms.setTimeToLive(3);
            ms.joinGroup(ia);
        }
        catch (IOException se){
            System.err.println(se);
        }
    }

    //Фунция для закрытия сокета
    void closeSocketSender(){
        try{
            ms.leaveGroup(ia);
        }
        catch (IOException se){
            System.err.println(se);
        }
    }

    //Функция для начала записи аудиопотока с микрофона
    void recordStart(){
        Log.d(LOG_TAG, "record start");
        audioRecord.startRecording();
        int recordingState = audioRecord.getRecordingState();
        Log.d(LOG_TAG, "recordingState = " + recordingState);
    }

    //Чтение запсаного аудиопотока в буфер для отправки и его отправка
    void readStart(){
        Log.d(LOG_TAG, "read start");
        RecAndSend = new Thread(new Runnable() {
            @Override
            public void run() {
                if(audioRecord == null)
                    return;
                int readCount = 0;
                int totalCount = 0;
                int i = 0;
                while (!RecAndSend.isInterrupted()){
                    readCount = audioRecord.read(bufferSend[i], 2, myBufferSize);
                    if(CheckTheAmplitude(bufferSend[i], myBufferSize)){
                        bufferSend[i][0] = (byte)(MyID&0xFF);
                        bufferSend[i][1] = (byte)(i&0xFF);
                        totalCount+= readCount;
                        Log.d(LOG_TAG, "readcount = " + readCount + ", totalCount = " + totalCount);

                        DatagramPacket dp = new DatagramPacket(bufferSend[i],bufferSend[i].length, ia, port);
                        Log.d(LOG_TAG, "Send: " + i + " packet");
                        try {
                            ms.send(dp);
                        }
                        catch (IOException se){
                            System.err.println(se);
                        }
                    }
                    if(i == 5) i = 0;
                    else i++;
                }
            }
        });

    }

    boolean CheckTheAmplitude(byte[] buffer, int syze){
        totalAbsValue = 0.0f;
        float sample        = 0;
        for( int i=2; i<syze + 2; i+=2 )
        {
            sample = (float)( (buffer[i]) | buffer[i + 1] << 8 );
            totalAbsValue += Math.abs(sample) / (syze/2);
        }

        Log.d(LOG_TAG, Float.toString(totalAbsValue));
        /*if(totalAbsValue > max)
        {
            max = totalAbsValue;
            Log.d(TAG, "New Max: " + Float.toString(max));
        }*/
        //prBar.setProgress((int)(Math.log(totalAbsValue+1) / Math.log(2.0))-1);
        if(totalAbsValue > NoiseLevel)return true;
            else return false;
    }

    //Остановка записи звука
    void recordStop(){
        Log.d(LOG_TAG, "record stop");
        audioRecord.stop();
    }
}

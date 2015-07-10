package ua.com.telecard.intercom_with_error_detect;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioRecord;
import android.media.AudioTrack;
import android.util.Log;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.Semaphore;

/**
 * Created by Roman on 25.05.2015.
 */
public class VoiceReciver {
    VoiceReciver() {
        Log.d(LOG_TAG, "Конструктор класса");
        sampleRate = 8000;
        channelConfig = AudioFormat.CHANNEL_CONFIGURATION_MONO;
        audioFormat = AudioFormat.ENCODING_PCM_16BIT;
        minInternalBufferSize = AudioRecord.getMinBufferSize(sampleRate, channelConfig, audioFormat);
        myBufferSize = minInternalBufferSize;
        bufferWrite = new byte[6][myBufferSize];
        //ReceivedPackets = new boolean[6];
        CountReceivedPackets = 0;
        mSemaphore = new Semaphore(1);
        RawOfPackets = new MyQueue();

    }
    int sampleRate;
    int channelConfig;
    int audioFormat;
    int minInternalBufferSize;
    int myBufferSize;
    MyQueue RawOfPackets;

    Thread playThread;
    byte bufferWrite[][];
    //boolean ReceivedPackets[];
    int CountReceivedPackets;
    public Semaphore mSemaphore;
    boolean FirstPacket = false;
    boolean isReading = false;

    final String LOG_TAG = "myLogs";

    public void PlayBuffer(final int number){
        playThread = new Thread(new Runnable() {
            public void run() {
                AudioTrack speaker;
                speaker = new AudioTrack(AudioManager.STREAM_MUSIC, sampleRate, channelConfig,
                        audioFormat, myBufferSize, AudioTrack.MODE_STREAM);
                speaker.play();

                Log.d(LOG_TAG, "minOutInternalBufferSize = " + minInternalBufferSize
                        + ", myBufferSize = " + myBufferSize);

                int i = number;
                byte buffer[] = new byte[704];

                while(!playThread.isInterrupted()) {
                    if (isReading & (CountReceivedPackets > 0)) {

                        try {               //Сверка текущего номера пакета и следующего к воспроизведению
                            mSemaphore.acquire();
                            if(RawOfPackets.isEmpty()){
                                Log.d(LOG_TAG, "Error in queue");
                            }
                            int nextPacket = RawOfPackets.get();
                            if(nextPacket != i){
                                i = nextPacket;
                            }
                        }
                        catch (InterruptedException ie){
                            ie.printStackTrace();
                        }
                        finally {
                            mSemaphore.release();
                        }

                        //Копирование из буффера следующего пакета для воспроизведения
                        Log.d(LOG_TAG, "Write this part of buffer: " + i);
                        try {
                            mSemaphore.acquire();
                            buffer = Arrays.copyOfRange(bufferWrite[i], 0, 704);
                            CountReceivedPackets--;
                        }
                        catch (InterruptedException ie){
                            ie.printStackTrace();
                        }
                        finally {
                            mSemaphore.release();
                        }
                        //Воспроизведение пакета
                        speaker.write(buffer, 0, myBufferSize);

                        if(i == 5) i = 0;
                        else i++;
                    }
                }
            }
        });
        //Запуск потока Считывания и Воспроизведения
        playThread.start();
        isReading = true;
    }

    //Класс реализующий стек для запси порядка прихождения пакетов из сети
    /*public class MyStack {
        private final int[] arr = new int[6];
        private int count;
        public void push(int elem) {
            if(count <6) arr[count++] = elem;
        }
        public int pop() {
            if(count > 0 )
            return arr[--count];
            else return -1;
        }
        public boolean isEmpty() {
            return count == 0;
        }
        public int indexOf(int elem) {
            for (int i = 0; i < count; i++) {
                if (arr[i] == elem)
                    return i;
            }
            return -1;
        }
    }*/

    public class MyQueue {

        private List<Integer> queue = new ArrayList();

        public Boolean put(int value){
            return queue.add(value);
        }

        public int get(){
            int result = queue.get(0);
            queue.remove(0);
            return result;
        }

        public int getCount(){
            return queue.size();
        }

        public boolean isEmpty(){
            return queue.size() == 0;
        }

        public Boolean hasNext(){
            return queue.size() > 0;
        }
    }
}

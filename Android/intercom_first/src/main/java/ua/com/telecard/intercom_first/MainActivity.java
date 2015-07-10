package ua.com.telecard.intercom_first;

import android.content.SharedPreferences;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioRecord;
import android.media.AudioTrack;
import android.media.MediaRecorder;
import android.media.SoundPool;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.util.Arrays;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;


public class MainActivity extends ActionBarActivity implements View.OnClickListener {

    //----------------------------------------------------------------------------------------------
    /*Работа с ID*/
    EditText edID;
    Button btnAccept, btnCheck;

    SharedPreferences sPref;

    final String ID = "ID";
    final String LOG_TAG = "myLogs";
    int MyID = 0;
    //----------------------------------------------------------------------------------------------
    /*Запись и отправка звука*/
    int sampleRate = 8000;
    int channelConfig = AudioFormat.CHANNEL_CONFIGURATION_MONO;
    int audioFormat = AudioFormat.ENCODING_PCM_16BIT;
    int minInternalBufferSize = AudioRecord.getMinBufferSize(sampleRate, channelConfig, audioFormat);
    int myBufferSize = minInternalBufferSize;
    AudioRecord audioRecord;
    Thread RecAndSend;
    byte [][] bufferSend;
    InetAddress ia;
    MulticastSocket ms;
    int port;
    //----------------------------------------------------------------------------------------------
    /*Прием и воспроизведение пакетов*/
    VoiceReceiver MasOfReceivers[];
    Thread receivingThread;
    MulticastSocket mr;

    //----------------------------------------------------------------------------------------------

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Log.d(LOG_TAG, "Create");
        edID = (EditText)findViewById(R.id.edID);
        btnAccept = (Button)findViewById(R.id.btnAccept);
        btnCheck = (Button)findViewById(R.id.btnCheck);

        btnAccept.setOnClickListener(this);
        btnCheck.setOnClickListener(this);
        loadID();
        bufferSend = new byte[6][myBufferSize+2];

        MasOfReceivers = new VoiceReceiver[256];
        for(int i =0; i< 256; i++) {
            MasOfReceivers[i] = new VoiceReceiver();
            Log.d(LOG_TAG, Integer.toString(i));
        }
        createAudioRecorder();
        Log.d(LOG_TAG, "Create audiorecorder");
        createMulticastSender(5500);
        Log.d(LOG_TAG, "Create ms ");
        createMulticastReceiver(5500);
        Log.d(LOG_TAG, "Create mr");


    }

    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.btnAccept:
                saveID();
                break;
            case R.id.btnCheck:
                Toast.makeText(this, Integer.toString(MyID), Toast.LENGTH_SHORT).show();
                MasOfReceivers[5].FirstPacket = true;
                break;
        }
    }
    /*Сохранение ID в файл*/
    void saveID() {
        sPref = getPreferences(MODE_PRIVATE);
        SharedPreferences.Editor ed = sPref.edit();
        ed.putString(ID, edID.getText().toString());
        ed.commit();
        Toast.makeText(this, "ID saved", Toast.LENGTH_SHORT).show();
    }

    /*Чтение ID с файла*/
    void loadID() {
        sPref = getPreferences(MODE_PRIVATE);
        String savedText = sPref.getString(ID, "99");
        Log.d(LOG_TAG, savedText);
        if(savedText != "")
        {
            MyID = Integer.parseInt(savedText);
        }
        if(MyID > 255){
            MyID = MyID%255;
        }
        edID.setText(savedText);
        Toast.makeText(this, "ID loaded", Toast.LENGTH_SHORT).show();
    }

    @Override
    public void onResume(){
        super.onResume();

        /*try {
            TimeUnit.SECONDS.sleep(5);
        }
        catch (InterruptedException ie){
            ie.printStackTrace();
        }*/
        if(MyID == 0){
            MyID = 99;
        }
        recordStart();
        readStart();
        RecAndSend.start();
        PacketReceiver();
    }

    @Override
    public void onPause(){
        super.onPause();
        RecAndSend.interrupt();
        recordStop();
        closeSocketSender();
        closeSocketReceiver();

        receivingThread.isInterrupted();
        for(int i = 0; i < 256; i++){
            if(!MasOfReceivers[i].FirstPacket) {
                MasOfReceivers[i].playThread.isInterrupted();
                MasOfReceivers[i].isReading = false;
            }
        }
    }
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

    void createMulticastReceiver(int Porrt){
        ia = null;
        mr = null;
        port = Porrt;
        try {
            ia = InetAddress.getByName("224.2.76.24");
            mr = new MulticastSocket(port);
            //ms.joinGroup(ia);

        }
        catch (IOException se){
            System.err.println(se);
        }
    }

    void closeSocketReceiver(){
        try{
            mr.leaveGroup(ia);
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
        //isReading = true;
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
                    if(i == 5) i = 0;
                    else i++;
                }
            }
        });

    }

    //Остановка записи звука
    void recordStop(){
        Log.d(LOG_TAG, "record stop");
        audioRecord.stop();
    }

    //Класс для работы и приема голоса
    public class VoiceReceiver {
        VoiceReceiver() {
            Log.d(LOG_TAG, "Конструктор класса");
            bufferWrite = new byte[6][myBufferSize];
            //ReceivedPackets = new boolean[6];
            CountReceivedPackets = 0;
            mSemaphore = new Semaphore(1);
        }
        Thread playThread;
        byte bufferWrite[][];
        //boolean ReceivedPackets[];
        int CountReceivedPackets;
        public Semaphore mSemaphore;
        boolean FirstPacket = false;
        boolean isReading = false;

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
                            speaker.write(buffer, 0, myBufferSize);

                            if(i == 5) i = 0;
                            else i++;
                        }
                    }
                }
            });
            playThread.start();
            isReading = true;
        }
    }
    /*Функция для запуска потока приема голосовых сообщений*/
    public void PacketReceiver() {
        receivingThread = new Thread(new Runnable() {
            @Override
            public void run() {
                try {

                    /*AudioTrack speaker;
                    speaker = new AudioTrack(AudioManager.STREAM_MUSIC, sampleRate, channelConfig,
                            audioFormat, myBufferSize, AudioTrack.MODE_STREAM);
                    speaker.play();*/

                    byte[] buffer = new byte[706];
                    DatagramPacket dp = new DatagramPacket(buffer, buffer.length, ia, port);
                    //assert(mSemaphore != null);

                    mr.joinGroup(ia);
                    while (!receivingThread.isInterrupted()){
                        mr.receive(dp);
                        Log.d(LOG_TAG, "Receive some packet1");
                        if(((buffer[0]<<0)&0x000000ff) != MyID){
                            Log.d(LOG_TAG, "Receive some packet2");
                            if(!MasOfReceivers[(buffer[0]<<0)&0x000000ff].FirstPacket){
                                Log.d(LOG_TAG, "After if");
                                MasOfReceivers[(buffer[0]<<0)&0x000000ff].FirstPacket = true;
                                Log.d(LOG_TAG, "First packet");
                                Log.d(LOG_TAG, "First packet of " + Integer.toString((buffer[0]<<0)&0x000000ff));
                                MasOfReceivers[(buffer[0]<<0)&0x000000ff].PlayBuffer((buffer[1]<<0)&0x000000ff);
                            }

                            try{
                                MasOfReceivers[buffer[0]&0x000000ff].mSemaphore.acquire();

                                MasOfReceivers[buffer[0]&0x000000ff].bufferWrite[(buffer[1] << 0)&0x000000ff] = Arrays.copyOfRange(dp.getData(), 2, 706);
                                MasOfReceivers[buffer[0]&0x000000ff].CountReceivedPackets++;
                            }
                            catch (InterruptedException ie){
                                ie.printStackTrace();
                            }
                            finally {
                                MasOfReceivers[buffer[0]&0x000000ff].mSemaphore.release();
                            }
                            Log.d(LOG_TAG, "Receive: " + ((buffer[1] << 0)&0x000000ff) + "packet");
                        }
                    }

                } catch (IOException e) {
                    Log.d(LOG_TAG, "SocketException in receive");
                    e.printStackTrace();
                }

            }
        });
        receivingThread.start();
    }
}

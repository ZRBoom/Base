package ua.com.telecard.intercom_with_error_detect;

import android.media.AudioManager;
import android.media.SoundPool;
import android.os.Handler;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.Toast;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.util.Arrays;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.TimeUnit;


public class MainActivity extends ActionBarActivity implements View.OnClickListener {

    //----------------------------------------------------------------------------------------------
    /*Работа с ID*/
    EditText edID;
    Button btnAccept, btnCheck;
    final String LOG_TAG = "myLogs";
    IDRelations ID;        //Присвоение идентификационного номера
    //----------------------------------------------------------------------------------------------
    /*Запись и отправка пакетов*/
    Streamer MultiSender;  //Создание объекта для считывания и отправки голоса
    int port = 5500;
    //----------------------------------------------------------------------------------------------
    /*Прием и воспроизведение пакетов*/
    VoiceReciver MasOfReceivers[];
    Thread receivingThread;
    MulticastSocket mr;
    InetAddress ia;
    //----------------------------------------------------------------------------------------------
    /*Отрисовка уровня голоса на прогресс баре*/
    ProgressBar pbBar;
    Timer timer;
    TimerTask timerTask;
    final Handler handler = new Handler();
    //----------------------------------------------------------------------------------------------

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        try{
            TimeUnit.SECONDS.sleep(5);
        }
        catch (InterruptedException e){
            e.printStackTrace();
        }
        Log.d(LOG_TAG, "Create");
        edID = (EditText)findViewById(R.id.edID);
        btnAccept = (Button)findViewById(R.id.btnAccept);
        btnCheck = (Button)findViewById(R.id.btnCheck);
        pbBar = (ProgressBar)findViewById(R.id.pbBar);

        btnAccept.setOnClickListener(this);
        btnCheck.setOnClickListener(this);

        ID = new IDRelations(MainActivity.this);
        edID.setText(ID.loadID());
        MultiSender = new Streamer(port, ID.MyID, 20.0f);

        MasOfReceivers = new VoiceReciver[256];
        for(int i =0; i< 256; i++) {
            MasOfReceivers[i] = new VoiceReciver();
            Log.d(LOG_TAG, Integer.toString(i));
        }
        createMulticastReceiver(port);

        /*Создание таймера, инициализация и запуск таймер таска на отображение уровня громкости микрофона*/
        timer = new Timer();
        initializeTimerTask();
        timer.schedule(timerTask, 5000, 2000);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.btnAccept:
                ID.saveID(edID.getText().toString());
                edID.setText(ID.loadID());
                break;
            case R.id.btnCheck:
                Toast.makeText(this, Integer.toString(ID.MyID), Toast.LENGTH_SHORT).show();
                break;
        }
    }

    @Override
    public void onPause(){
        super.onPause();
        MultiSender.RecAndSend.isInterrupted();
        MultiSender.recordStop();
        MultiSender.closeSocketSender();
        closeSocketReceiver();

        receivingThread.isInterrupted();
        for(int i = 0; i < 256; i++){
            if(!MasOfReceivers[i].FirstPacket) {
                MasOfReceivers[i].playThread.isInterrupted();
                MasOfReceivers[i].isReading = false;
            }
        }
    }

    @Override
    public void onResume(){
        super.onResume();
        try{
            TimeUnit.SECONDS.sleep(5);
        }
        catch (InterruptedException e){
            e.printStackTrace();
        }
        MultiSender.recordStart();
        MultiSender.readStart();
        MultiSender.RecAndSend.start();
        PacketReceiver();
    }

    /*Создание сокета для приема мультикастовых пакетов*/
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

    /*Закрытие сокета для приема мультикастовых пакетов*/
    void closeSocketReceiver(){
        try{
            mr.leaveGroup(ia);
        }
        catch (IOException se){
            System.err.println(se);
        }
    }

    /*Функция для запуска потока приема голосовых сообщений*/
    public void PacketReceiver() {
        receivingThread = new Thread(new Runnable() {
            @Override
            public void run() {
                try {

                    byte[] buffer = new byte[706];
                    DatagramPacket dp = new DatagramPacket(buffer, buffer.length, ia, port);

                    mr.joinGroup(ia);
                    while (!receivingThread.isInterrupted()){
                        mr.receive(dp);
                        //Log.d(LOG_TAG, "Receive some packet1");
                        if(((buffer[0]<<0)&0x000000ff) != ID.MyID){
                            //Log.d(LOG_TAG, "Receive some packet2");
                            if(!MasOfReceivers[(buffer[0]<<0)&0x000000ff].FirstPacket){
                                //Log.d(LOG_TAG, "After if");
                                MasOfReceivers[(buffer[0]<<0)&0x000000ff].FirstPacket = true;
                                //Log.d(LOG_TAG, "First packet");
                                Log.d(LOG_TAG, "First packet of " + Integer.toString((buffer[0]<<0)&0x000000ff));
                                MasOfReceivers[(buffer[0]<<0)&0x000000ff].PlayBuffer((buffer[1]<<0)&0x000000ff);

                            }

                            try{
                                MasOfReceivers[buffer[0]&0x000000ff].mSemaphore.acquire();

                                MasOfReceivers[buffer[0]&0x000000ff].bufferWrite[(buffer[1] << 0)&0x000000ff] = Arrays.copyOfRange(dp.getData(), 2, 706);
                                MasOfReceivers[buffer[0]<<0&0x000000ff].RawOfPackets.put((buffer[1] << 0) & 0x000000ff);
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

    /*Таймер таск отображающий уровень громкости на экране*/
    public void initializeTimerTask(){
        timerTask = new TimerTask() {
            @Override
            public void run() {
                handler.post(new Runnable() {
                    @Override
                    public void run() {
                        pbBar.setProgress((int)(Math.log(MultiSender.totalAbsValue+1) / Math.log(2.0))-1);
                    }
                });
                try {
                    MultiSender.ms.joinGroup(InetAddress.getByName("224.2.76.24"));
                    mr.joinGroup(InetAddress.getByName("224.2.76.24"));
                }
                catch (IOException se){
                    System.err.println(se);
                }
            }
        };
    }
}

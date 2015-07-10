package ua.com.telecard.commandpromt;

import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import java.io.DataOutputStream;
import java.io.IOException;


public class MainActivity extends ActionBarActivity implements View.OnClickListener{

    Button btnScript;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btnScript = (Button)findViewById(R.id.btnScript);
        btnScript.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        //Toast.makeText(this, "Start script", Toast.LENGTH_SHORT).show();
        try{
            Process su = Runtime.getRuntime().exec("su");
            DataOutputStream outputStream = new DataOutputStream(su.getOutputStream());

            outputStream.writeBytes("/data/data/alsa_restore.sh &\n");
            outputStream.flush();

            outputStream.writeBytes("exit\n");
            outputStream.flush();
            su.waitFor();
        }catch(IOException e){
            e.printStackTrace();
        }catch(InterruptedException e){
            e.printStackTrace();
        }
        //Toast.makeText(this, "Stop script", Toast.LENGTH_SHORT).show();
    }
}

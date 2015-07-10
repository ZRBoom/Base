package ua.com.telecard.intercom_with_error_detect;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

/**
 * Created by Roman on 09.07.2015.
 */
public class App_Reciver extends BroadcastReceiver {

    @Override
    public void onReceive(Context context, Intent intent) {
        Log.d("myLogs", intent.getAction());

        if ((intent.getAction() != null) && (intent.getAction().equals("android.intent.action.BOOT_COMPLETED"))) {

            Log.d("myLogs", "auto restart true");

            Intent i = new Intent(context, MainActivity.class);
            i.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            context.startActivity(i);
        }
    }

}
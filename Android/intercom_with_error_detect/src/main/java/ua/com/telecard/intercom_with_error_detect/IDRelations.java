package ua.com.telecard.intercom_with_error_detect;

import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;


/**
 * Created by Roman on 12.05.2015.
 */
public class IDRelations {
    IDRelations(Context Main){
        this.c = Main;
        this.sPref = PreferenceManager.getDefaultSharedPreferences(c);
    }
    Context c;
    SharedPreferences sPref;
    final String ID = "ID";
    final String LOG_TAG = "myLogs";
    int MyID = 0;

    /*Сохранение ID в файл*/
    void saveID(String id) {
        sPref = c.getSharedPreferences("ua.com.telecard.intercom_with_error_detect", c.MODE_PRIVATE);
        SharedPreferences.Editor ed = sPref.edit();
        ed.putString(ID, id);
        ed.commit();
        Toast.makeText(c, "ID saved", Toast.LENGTH_SHORT).show();
        MyID = Integer.parseInt(id);
        if(MyID > 255){
            MyID = MyID%255;
        }
    }

    /*Чтение ID с файла*/
    String loadID() {
        sPref = c.getSharedPreferences("ua.com.telecard.intercom_with_error_detect", c.MODE_PRIVATE);
        String savedText = sPref.getString(ID, "99");
        Log.d(LOG_TAG, savedText);
        if(savedText != "")
        {
            MyID = Integer.parseInt(savedText);
        }
        if(MyID > 255){
            MyID = MyID%255;
        }
        Toast.makeText(c, "ID loaded", Toast.LENGTH_SHORT).show();
        return String.valueOf(MyID);
    }

}

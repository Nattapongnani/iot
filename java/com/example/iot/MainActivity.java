package com.example.iot;

import androidx.appcompat.app.AppCompatActivity;

import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import org.json.JSONException;
import org.json.JSONObject;
import org.json.JSONTokener;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Timer;
import java.util.TimerTask;

/*
    <WebView
        android:id="@+id/tempGraph"
        android:layout_width="match_parent"
        android:layout_height="match_parent" />
*/


public class MainActivity extends AppCompatActivity {

    TextView temperatureView;
    TextView humidityView;
    WebView tempGraphView;

    private static final String TEMPERATURE_URL = "https://api.thingspeak.com/channels/2014343/field/1/last.json";
    private static final String HUMIDITY_URL = "https://api.thingspeak.com/channels/2014343/field/2/last.json";
    //add
    private static final String TEMPERATURE_GRAPH_URL = "https://thingspeak.com/channels/2014343/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // lab 7
        int orientation = getResources().getConfiguration().orientation;
        if(orientation == Configuration.ORIENTATION_LANDSCAPE){ //landscape
            temperatureView = findViewById(R.id.tempGraph);
            tempGraphView.loadUrl(TEMPERATURE_GRAPH_URL);
            tempGraphView.getSettings().setJavaScriptEnabled(true);
        } else { // portrait

        }

        temperatureView = findViewById(R.id.temp);
        humidityView = findViewById(R.id.humid);

        new Timer().scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                FetchThingspeakTask field1 = new FetchThingspeakTask();
                field1.execute(TEMPERATURE_URL);
                FetchThingspeakTask field2 = new FetchThingspeakTask();
                field2.execute(HUMIDITY_URL);
            }
        }, 0, 10000);



        Switch switch1 = (Switch) findViewById(R.id.sw1);
        Switch switch2 = (Switch) findViewById(R.id.sw2);

        //Boolean switch1State = switch1.isChecked();
        //Boolean switch2State = switch1.isChecked();
    }

    private class FetchThingspeakTask extends AsyncTask<String, Void, String>{
        protected String doInBackground(String... urls){
            try {
                URL url = new URL(urls[0]);
                HttpURLConnection urlConnection = (HttpURLConnection) url.openConnection();
                try {
                    BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(urlConnection.getInputStream()));
                    StringBuilder stringBuilder = new StringBuilder();
                    String line;
                    while((line = bufferedReader.readLine()) != null){
                        stringBuilder.append(line).append("\n");
                    }
                    bufferedReader.close();
                    return stringBuilder.toString();
                } finally {
                    urlConnection.disconnect();
                }
            }catch(Exception e){
                Log.e("ERROR", e.getMessage(), e);
                return null;
            }
        }
        protected void onPostExecute(String response){
            if(response == null){
                Toast.makeText(MainActivity.this, "There was an error", Toast.LENGTH_SHORT).show();
                return;
            }
            try{
                JSONObject channel = (JSONObject) new JSONTokener(response).nextValue();
                if (channel.has("field1")){
                    String status = channel.getString("field1");
                    if(!status.equals("null")){
                        String message = getText(R.string.temperature) + " " + status + " " + getText(R.string.celsius);
                        temperatureView.setText(message);
                    }
                    //String message = getText(R.string.temperature) + " " + channel.getString("field1") + " " + getText(R.string.celsius);
                    //temperatureView.setText(message);
                }
                if(channel.has("field2")){
                    String status = channel.getString("field2");
                    if(!status.equals("null")){
                        String message = getText(R.string.temperature) + " " + status + " " + getText(R.string.celsius);
                        humidityView.setText(message);
                    }
                    //String message = getText(R.string.humdity) + " " + channel.getString("field2") + " " + getText(R.string.percent);
                    //humidityView.setText(message);
                }
            } catch(JSONException e){
                e.printStackTrace();
            }
        }
    }

    public void onSW1Clicked(View v){
        boolean on = ((Switch) v).isChecked();

        if(on){
            Toast.makeText(getApplicationContext(), R.string.on1, Toast.LENGTH_SHORT).show();
        }else {
            Toast.makeText(getApplicationContext(), R.string.off1, Toast.LENGTH_SHORT).show();
        }
    }

    public void onSW2Clicked(View v){
        boolean on = ((Switch) v).isChecked();

        if(on){
            Toast.makeText(getApplicationContext(), R.string.on2, Toast.LENGTH_SHORT).show();
        }else {
            Toast.makeText(getApplicationContext(), R.string.off2, Toast.LENGTH_SHORT).show();
        }
    }
}
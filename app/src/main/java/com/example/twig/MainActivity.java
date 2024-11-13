package com.example.twig;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class MainActivity extends AppCompatActivity {


    Button Login_Button;
    Button Create_Button;
    EditText Email_Text;
    EditText Password_Text;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_main);
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.RegisterBtn), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom);

            Login_Button = findViewById(R.id.LoginBtn);
            Create_Button = findViewById(R.id.RegisterBtn);
            Email_Text = findViewById(R.id.LoginEmailTxt);
            Password_Text = findViewById(R.id.LoginPasswordTxt);

            Login_Button.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    String emailInput = Email_Text.getText().toString();
                    String passwordInput = Password_Text.getText().toString();

                    Email_Text.setText("");
                    Password_Text.setText("");

                    // Run network operation in a background thread
                    ExecutorService executor = Executors.newSingleThreadExecutor();
                    executor.execute(new Runnable() {
                        @Override
                        public void run() {
                            HttpURLConnection connection = null;
                            DataOutputStream wr = null;
                            InputStream is = null;

                            try {
                                URL url = new URL("http://192.168.68.92:8080/login");
                                connection = (HttpURLConnection) url.openConnection();
                                connection.setDoOutput(true);
                                connection.setRequestMethod("POST");
                                connection.setRequestProperty("Content-Type", "application/json; charset=UTF-8");

                                // Create JSON object and write it to the request body
                                JSONObject jsonParam = new JSONObject();
                                jsonParam.put("email", emailInput);
                                jsonParam.put("password", passwordInput);

                                wr = new DataOutputStream(connection.getOutputStream());
                                wr.writeBytes(jsonParam.toString());
                                wr.flush();

                                // Get response code and read the response
                                int responseCode = connection.getResponseCode();
                                is = (responseCode >= 200 && responseCode <= 399)
                                        ? connection.getInputStream()
                                        : connection.getErrorStream();

                                // Read the response from the input stream
                                BufferedReader reader = new BufferedReader(new InputStreamReader(is, "utf-8"));
                                StringBuilder response = new StringBuilder();
                                String line;
                                while ((line = reader.readLine()) != null) {
                                    response.append(line.trim());
                                }

                                // Log the response
                                Log.d("HTTP Response", "Response Code: " + responseCode);
                                Log.d("HTTP Response", "Response Body: " + response.toString());


                                JSONObject jsonResponse = new JSONObject(response.toString());
                                String message = jsonResponse.getString("message");



                                if ("Login successful".equals(message)) {
                                    Log.d("test", "Ingelogd");
                                    //Intent intent = new Intent(MainActivity.this, MainActivity.class);
                                    //startActivity(intent);
                                }
                                else {
                                    Log.e("HTTP Response", "Login failed");
                                    Email_Text.setError("Login gefaald");
                                }


                            } catch (Exception e) {
                                Log.e("HTTP Error", "Error occurred", e);

                            } finally {
                                // Close resources safely
                                try {
                                    if (wr != null) {
                                        wr.close(); // Close the DataOutputStream
                                    }
                                    if (is != null) {
                                        is.close(); // Close the InputStream
                                    }
                                    if (connection != null) {
                                        connection.disconnect(); // Close the connection
                                    }
                                } catch (IOException e) {
                                    Log.e("HTTP Error", "Error closing resources", e);
                                }
                            }
                        }
                    });
                }
            });


            Create_Button.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    //Intent intent = new Intent(LoginPage.this, Registory.class);
                    //startActivity(intent);
                }
            });






            return insets;
        });
    }
}
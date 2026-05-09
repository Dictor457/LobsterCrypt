package com.dictor.lobstercrypt;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.content.Context;
import android.content.Intent;
import android.content.ClipboardManager;
import android.content.ClipData;
import android.os.Vibrator;
import android.os.VibrationEffect;
import android.os.Build;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("lobster_crypt");
    }

    public native String encryptData(String input, String pin);
    public native String decryptData(String input, String pin);

    private Vibrator vibrator;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        vibrator = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);

        final EditText inputField = findViewById(R.id.inputField);
        final EditText pinInput = findViewById(R.id.pinInput);
        final TextView outputField = findViewById(R.id.outputField);
        
        final Button btnEncrypt = findViewById(R.id.btnEncrypt);
        final Button btnDecrypt = findViewById(R.id.btnDecrypt);
        final Button btnClear = findViewById(R.id.btnClear);
        final Button btnCopy = findViewById(R.id.btnCopy);
        final Button btnShare = findViewById(R.id.btnShare);

        btnEncrypt.setOnClickListener(v -> {
            String text = inputField.getText().toString();
            String pin = pinInput.getText().toString();
            if (!text.isEmpty() && !pin.isEmpty()) {
                vibrate(50);
                outputField.setText(encryptData(text, pin));
            }
        });

        btnDecrypt.setOnClickListener(v -> {
            String hex = inputField.getText().toString();
            String pin = pinInput.getText().toString();
            if (!hex.isEmpty() && !pin.isEmpty()) {
                vibrate(50);
                outputField.setText(decryptData(hex, pin));
            }
        });

        btnClear.setOnClickListener(v -> {
            vibrate(100);
            inputField.setText("");
            pinInput.setText("");
            outputField.setText("");
        });

        btnCopy.setOnClickListener(v -> {
            String text = outputField.getText().toString();
            if (!text.isEmpty()) {
                vibrate(40);
                ClipboardManager clipboard = (ClipboardManager) getSystemService(Context.CLIPBOARD_SERVICE);
                ClipData clip = ClipData.newPlainText("Lobster", text);
                clipboard.setPrimaryClip(clip);
                Toast.makeText(this, "Текст скопирован", Toast.LENGTH_SHORT).show();
            }
        });

        btnShare.setOnClickListener(v -> {
            String text = outputField.getText().toString();
            if (!text.isEmpty()) {
                vibrate(40);
                Intent sendIntent = new Intent();
                sendIntent.setAction(Intent.ACTION_SEND);
                sendIntent.putExtra(Intent.EXTRA_TEXT, text);
                sendIntent.setType("text/plain");
                startActivity(Intent.createChooser(sendIntent, "Поделиться"));
            }
        });
    }

    private void vibrate(int duration) {
        try {
            if (vibrator != null && vibrator.hasVibrator()) {
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                    vibrator.vibrate(VibrationEffect.createOneShot(duration, VibrationEffect.DEFAULT_AMPLITUDE));
                } else {
                    vibrator.vibrate(duration);
                }
            }
        } catch (Exception ignored) {}
    }
}

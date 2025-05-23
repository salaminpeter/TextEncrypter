package com.momosoft.imageviewer;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;

import java.io.FileNotFoundException;
import java.io.InputStream;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("native-lib");
    }

    private static final int REQUEST_CODE_PICK_IMAGE = 1;
    private EditText m_TextEditor;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        m_TextEditor = findViewById(R.id.editTextTextMultiLine);

        Button PickImageBtn = findViewById(R.id.button);
        PickImageBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openImagePicker();
            }
        });

    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        //load image
        if (requestCode == REQUEST_CODE_PICK_IMAGE && resultCode == RESULT_OK && data != null) {
            Uri imageUri = data.getData();

            InputStream Stream = null;
            try {
                Stream = getContentResolver().openInputStream(imageUri);
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            }
            Bitmap LoadedImage = BitmapFactory.decodeStream(Stream);
            Bitmap ConvertedImage = LoadedImage.copy(Bitmap.Config.ARGB_8888, false);

            byte[] ByteData = new byte[LoadedImage.getWidth() * LoadedImage.getHeight() * 3];
            int[] PixelData = new int[LoadedImage.getWidth() * LoadedImage.getHeight()];

            ConvertedImage.getPixels(PixelData, 0, ConvertedImage.getWidth(), 0, 0, ConvertedImage.getWidth(), ConvertedImage.getHeight());

            for (int i = 0; i < PixelData.length; i++) {
                int Color = PixelData[i];
                byte r = (byte) ((Color >> 16) & 0xFF);
                byte g = (byte) ((Color >> 8) & 0xFF);
                byte b = (byte) (Color & 0xFF);
                ByteData[i * 3] = r;
                ByteData[i * 3 + 1] = g;
                ByteData[i * 3 + 2] = b;
            }

            setByteData(ByteData, ByteData.length);
            String Message = getMessage();
            m_TextEditor.setText(Message);

            ImageView imageView = findViewById(R.id.imageView);
            imageView.setImageURI(imageUri);
        }
    }

    private void openImagePicker() {
        Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        intent.setType("image/*");
        intent.addCategory(Intent.CATEGORY_OPENABLE);
        startActivityForResult(intent, REQUEST_CODE_PICK_IMAGE);
    }

    public native void setByteData(byte[] data, int size);
    public native String getMessage();
}
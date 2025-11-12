package com.sharan.rnd_opencv_viewer; // Make sure this package name matches yours

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.content.pm.PackageManager;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.os.Bundle;
import android.util.Log;
import android.view.TextureView;
import android.widget.Toast;

import java.io.IOException;

public class MainActivity extends AppCompatActivity implements TextureView.SurfaceTextureListener {

    private static final String TAG = "MainActivity";
    private static final int CAMERA_REQUEST_CODE = 100;

    private TextureView textureView;
    private Camera camera;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        textureView = findViewById(R.id.textureView);
        textureView.setSurfaceTextureListener(this);

        // Check for camera permission
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.CAMERA)
                != PackageManager.PERMISSION_GRANTED) {
            // Request permission
            ActivityCompat.requestPermissions(this,
                    new String[]{Manifest.permission.CAMERA},
                    CAMERA_REQUEST_CODE);
        }
    }

    // This is called when the TextureView is ready
    @Override
    public void onSurfaceTextureAvailable(@NonNull SurfaceTexture surface, int width, int height) {
        Log.d(TAG, "Surface Texture is available");
        startCamera(surface);
    }

    private void startCamera(SurfaceTexture surface) {
        // Check permission again, just in case
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.CAMERA)
                == PackageManager.PERMISSION_GRANTED) {
            try {
                camera = Camera.open(); // Open the back camera
                camera.setPreviewTexture(surface);
                camera.setDisplayOrientation(90); // Adjust for portrait mode
                camera.startPreview(); // Start the live feed!
                Log.d(TAG, "Camera preview started");
            } catch (IOException e) {
                Log.e(TAG, "Error setting camera preview", e);
            } catch (RuntimeException e) {
                Log.e(TAG, "Error opening camera", e);
            }
        } else {
            Toast.makeText(this, "Camera permission is required", Toast.LENGTH_SHORT).show();
        }
    }

    // This is called when the TextureView is destroyed
    @Override
    public boolean onSurfaceTextureDestroyed(@NonNull SurfaceTexture surface) { // <-- 1. Changed void to boolean
        Log.d(TAG, "Surface Texture is destroyed");
        if (camera != null) {
            camera.stopPreview();
            camera.release();
            camera = null;
        }
        return true; // <-- 2. Added this line
    }

    // --- Other required listener methods ---
    @Override
    public void onSurfaceTextureSizeChanged(@NonNull SurfaceTexture surface, int width, int height) {
        // Can be left empty for this simple app
    }

    @Override
    public void onSurfaceTextureUpdated(@NonNull SurfaceTexture surface) {
        // This is called for every new frame. We will use this in the next phase.
    }

    // This handles the result of the permission request
    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == CAMERA_REQUEST_CODE) {
            if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                // Permission was granted, start camera when surface is ready
                if (textureView.isAvailable()) {
                    startCamera(textureView.getSurfaceTexture());
                }
            } else {
                Toast.makeText(this, "Camera permission is required to run this app", Toast.LENGTH_LONG).show();
                finish();
            }
        }
    }
}
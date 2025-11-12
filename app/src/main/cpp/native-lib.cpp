#include <string>
#include <jni.h>
// Add this line to test
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <android/log.h> // <-- Add this for logging

// Define a tag for our logs
#define LOG_TAG "Native-Lib"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

cv::Mat processedMat;

extern "C" JNIEXPORT jstring JNICALL
Java_com_sharan_rnd_1opencv_1viewer_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {

    // Add this line to test
    cv::Mat testMat;
    LOGD("OpenCV testMat created successfully.");

    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

// --- ADD THIS NEW FUNCTION ---
// This is the C++ side of our 'processFrame' native method

extern "C" JNIEXPORT void JNICALL
Java_com_sharan_rnd_1opencv_1viewer_MainActivity_processFrame(
        JNIEnv *env,
        jobject /* this */,
        jint width,
        jint height,
        jbyteArray frameData) {

    // 1. Get the raw byte data from Java
    jbyte *data = env->GetByteArrayElements(frameData, 0);

    // 2. Create a Mat from the raw camera data (NV21 format)
    // The camera data is (height + height/2) x width,
    // as it contains the Y plane (grayscale) followed by the UV plane (color)
    cv::Mat matYUV(height + height / 2, width, CV_8UC1, (void*)data);

    // 3. Extract just the grayscale (Y) plane
    // This is a "zero-copy" operation; it just points to the first part of matYUV
    cv::Mat matGray = matYUV(cv::Rect(0, 0, width, height));

    // 4. Apply Canny Edge Detection
    // We'll re-use our global 'processedMat' to store the result
    // We can also use matGray as the output to save memory, then copy
    cv::Canny(matGray, processedMat, 80, 100);

    // 5. Release the Java byte array
    env->ReleaseByteArrayElements(frameData, data, 0);
}
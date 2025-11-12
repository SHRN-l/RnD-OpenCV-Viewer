#include <string>
#include <jni.h>
// Add this line to test
#include <opencv2/core.hpp>
#include <android/log.h> // <-- Add this for logging

// Define a tag for our logs
#define LOG_TAG "Native-Lib"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

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

    // 2. JUST FOR NOW: Log a message to prove it's working
    //    In the next phase, we'll put OpenCV code here
    LOGD("C++ received a frame! Size: %d x %d", width, height);

    // 3. Release the byte data
    env->ReleaseByteArrayElements(frameData, data, 0);
}